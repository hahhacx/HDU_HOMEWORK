import psutil


def show_app_memory_usage():
    """显示应用程序内存占用情况（按占用降序）"""
    # 存储进程内存信息：{进程名: [示例PID, 总占用内存(MB), 最大内存使用率(%)]}
    process_mem = {}

    # 遍历所有进程，过滤无权限/无效进程
    for proc in psutil.process_iter(['pid', 'name', 'memory_info', 'memory_percent']):
        try:
            if proc.info['pid'] == 0:  # 跳过系统空闲进程
                continue
            proc_name = proc.info['name'].lower()  # 统一小写避免重复
            # 物理内存占用（转换为MB，rss=实际使用的物理内存）
            mem_mb = round(proc.info['memory_info'].rss / 1024 / 1024, 2)
            mem_percent = round(proc.info['memory_percent'], 2)

            # 合并同一应用的多进程（如Chrome多个进程）
            if proc_name in process_mem:
                process_mem[proc_name][1] += mem_mb
                process_mem[proc_name][2] = max(process_mem[proc_name][2], mem_percent)
            else:
                process_mem[proc_name] = [proc.info['pid'], mem_mb, mem_percent]
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            continue

    # 按内存占用降序排序
    sorted_procs = sorted(process_mem.items(), key=lambda x: x[1][1], reverse=True)

    # 格式化输出结果
    print("=" * 80)
    print(f"{'应用程序名':<20} {'PID(示例)':<10} {'占用内存(MB)':<15} {'内存使用率(%)':<10}")
    print("=" * 80)
    for name, info in sorted_procs:
        if info[1] > 0.1:  # 过滤无效小进程
            print(f"{name[:18]:<20} {info[0]:<10} {info[1]:<15} {info[2]:<10}")


if __name__ == "__main__":
    print("正在获取应用程序内存占用...\n")
    show_app_memory_usage()