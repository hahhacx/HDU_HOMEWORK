import time
import os
from watchdog import observers
from watchdog.events import FileSystemEventHandler
import psutil
from datetime import datetime


class FileMonitor(FileSystemEventHandler):
    """自定义文件监控处理器"""

    def __init__(self, target_file):
        self.target_file = target_file  # 要监控的文件路径

    def get_op_process(self):
        """获取操作文件的进程"""
        try:
            for proc in psutil.process_iter(['pid', 'name', 'open_files']):
                if proc.info['open_files']:
                    for f in proc.info['open_files']:
                        if self.target_file.lower() in f.path.lower():
                            return f"PID:{proc.info['pid']} ({proc.info['name']})"
            return "未知进程"
        except (psutil.NoSuchProcess, psutil.AccessDenied):
            return "无权限获取进程"

    # 监控文件修改（写入/保存）
    def on_modified(self, event):
        if not event.is_directory and event.src_path == self.target_file:
            op_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f"[{op_time}] 📝 文件修改：{self.target_file} | 操作进程：{self.get_op_process()}")

    # 监控文件删除
    def on_deleted(self, event):
        if not event.is_directory and event.src_path == self.target_file:
            op_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f"[{op_time}] ❌ 文件删除：{self.target_file} | 操作进程：{self.get_op_process()}")

    # 监控文件创建（恢复）
    def on_created(self, event):
        if not event.is_directory and event.src_path == self.target_file:
            op_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f"[{op_time}] ✨ 文件创建：{self.target_file} | 操作进程：{self.get_op_process()}")


def monitor_file(target_file):
    """启动文件监控"""
    if not os.path.exists(target_file):
        print(f"错误：文件 {target_file} 不存在！")
        return

    # 初始化监控器
    event_handler = FileMonitor(target_file)
    observer = observers.Observer()
    # 监控文件所在目录（watchdog需监听目录）
    observer.schedule(event_handler, path=os.path.dirname(target_file), recursive=False)

    # 启动监控
    observer.start()
    print(f"开始监控文件：{target_file} | 按Ctrl+C停止\n")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
        print("\n文件监控已停止")
    observer.join()


if __name__ == "__main__":
    # 要监控的文件（替换为你的目标文件）
    TARGET_FILE = r"C:\snort\rules\new.rules"
    monitor_file(TARGET_FILE)