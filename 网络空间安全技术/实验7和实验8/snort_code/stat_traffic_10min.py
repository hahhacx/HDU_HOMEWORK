import psutil
import time
from datetime import datetime, timedelta


def get_system_net_io():
    """获取Windows系统级总网络IO（发送/接收字节）- 适配Windows"""
    # Windows下psutil.net_io_counters()返回系统总网络IO
    net_io = psutil.net_io_counters()
    # 返回：(发送字节, 接收字节)
    return (net_io.bytes_sent, net_io.bytes_recv)


def get_network_processes():
    """获取当前有网络连接的进程（进程名+端口）- 辅助标识应用"""
    proc_list = {}
    # 遍历所有网络连接，关联进程
    for conn in psutil.net_connections(kind='inet'):
        if conn.pid is None:
            continue
        try:
            proc = psutil.Process(conn.pid)
            proc_name = proc.name().lower()
            # 记录进程名和连接信息（本地端口:远程端口）
            conn_info = f"{conn.laddr.port}:{conn.raddr.port if conn.raddr else '0'}"
            if proc_name not in proc_list:
                proc_list[proc_name] = [conn_info]
            else:
                if conn_info not in proc_list[proc_name]:
                    proc_list[proc_name].append(conn_info)
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            continue
    return proc_list


def stat_10min_traffic_windows():
    """Windows下统计10分钟系统总流量+有网络连接的应用（适配实验要求）"""
    # 1. 记录起始时间和系统初始IO
    start_time = datetime.now()
    # 快捷测试：改为seconds=30（30秒），验证后改回minutes=10
    end_time = start_time + timedelta(minutes=10)
    initial_sent, initial_recv = get_system_net_io()

    # 获取初始有网络连接的进程
    initial_procs = get_network_processes()
    print(f"===== Windows流量统计（10分钟）=====")
    print(f"开始时间：{start_time.strftime('%Y-%m-%d %H:%M:%S')}")
    print(f"结束时间：{end_time.strftime('%Y-%m-%d %H:%M:%S')}")
    print(f"初始有网络连接的应用：{list(initial_procs.keys())}")
    print("\n期间可操作网络（如telnet、浏览网页、打开QQ）...\n")

    # 2. 等待10分钟，每60秒打印进度
    elapsed_min = 0
    while datetime.now() < end_time:
        time.sleep(60)
        elapsed_min += 1
        print(f"进度：已统计 {elapsed_min} 分钟，剩余 {10 - elapsed_min} 分钟")

    # 3. 记录结束时的系统IO，计算总流量差值（转换为MB）
    final_sent, final_recv = get_system_net_io()
    total_send_mb = round((final_sent - initial_sent) / 1024 / 1024, 2)
    total_recv_mb = round((final_recv - initial_recv) / 1024 / 1024, 2)
    total_all_mb = round(total_send_mb + total_recv_mb, 2)

    # 获取结束时有网络连接的进程
    final_procs = get_network_processes()
    # 合并所有有网络活动的应用（初始+结束）
    all_procs = list(set(list(initial_procs.keys()) + list(final_procs.keys())))

    # 4. 格式化输出结果（Windows下无法精准到进程级流量，统计系统总流量+关联应用）
    print("\n" + "=" * 80)
    print(f"【Windows系统10分钟总流量】")
    print(f"发送流量：{total_send_mb} MB | 接收流量：{total_recv_mb} MB | 总流量：{total_all_mb} MB")
    print("=" * 80)
    print(f"【期间有网络活动的应用】")
    print(f"{'应用程序名':<20} {'网络连接（本地:远程端口）':<40}")
    print("-" * 80)
    for proc_name in all_procs:
        conn_info = final_procs.get(proc_name, initial_procs.get(proc_name, []))
        conn_str = "; ".join(conn_info[:3])  # 最多显示3个连接
        print(f"{proc_name[:18]:<20} {conn_str:<40}")

    # 补充实验说明
    print("\n📌 说明：Windows系统下无法通过psutil获取精准的进程级流量，")
    print("        以上为系统总流量 + 期间有网络活动的应用（实验等效满足要求）。")


if __name__ == "__main__":
    # 管理员权限运行！
    stat_10min_traffic_windows()