import psutil
import winreg  # Windows注册表操作


def get_startup_procs():
    """获取系统开机自启进程"""
    startup_list = []

    # 1. 获取自启服务（启动类型为auto）
    try:
        for service in psutil.win_service_iter():
            s = service.as_dict()
            if s['start_type'] == 'auto' and s['status'] == 'running':
                startup_list.append({
                    "name": s['name'],
                    "path": s['binary_path'],
                    "status": "已启用（系统服务）"
                })
    except psutil.AccessDenied:
        pass

    # 2. 从注册表获取用户自启项
    reg_paths = [
        r"Software\Microsoft\Windows\CurrentVersion\Run",  # 当前用户自启
        r"SYSTEM\CurrentControlSet\Services"  # 系统服务注册表
    ]
    for path in reg_paths:
        try:
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, path)
            i = 0
            while True:
                try:
                    name, exe_path, _ = winreg.EnumValue(key, i)
                    startup_list.append({
                        "name": name,
                        "path": exe_path,
                        "status": "已启用（注册表自启）"
                    })
                    i += 1
                except OSError:
                    break
            winreg.CloseKey(key)
        except FileNotFoundError:
            continue

    # 去重（同一进程多次记录）
    unique_startup = []
    seen_names = set()
    for item in startup_list:
        if item['name'] not in seen_names and item['path']:
            seen_names.add(item['name'])
            unique_startup.append(item)
    return unique_startup


def show_startup_processes():
    """格式化显示启动进程"""
    print("正在获取系统启动进程...\n")
    startup_procs = get_startup_procs()

    print("=" * 100)
    print(f"{'进程名':<20} {'自启路径':<50} {'状态':<15}")
    print("=" * 100)
    for proc in startup_procs:
        # 截断长路径，保证格式整齐
        path = proc['path'][:48] + "..." if len(proc['path']) > 50 else proc['path']
        print(f"{proc['name'][:18]:<20} {path:<50} {proc['status']:<15}")


if __name__ == "__main__":
    show_startup_processes()