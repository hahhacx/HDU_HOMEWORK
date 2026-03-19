from scapy.all import *

# 读取捕获的telnet数据包文件
packets = rdpcap("C:/snort/bin/log/telnet.pcap")

# 遍历每个数据包，提取三层字段
for i, pkt in enumerate(packets):
    print(f"===== 解析第 {i+1} 个数据包 =====")
    # 1. 提取链路层字段（Ethernet）
    if Ether in pkt:
        print(f"【链路层】源MAC: {pkt[Ether].src} | 目标MAC: {pkt[Ether].dst} | 帧类型: {hex(pkt[Ether].type)}")
    # 2. 提取网络层字段（IP+TCP）
    if IP in pkt and TCP in pkt:
        print(f"【网络层】源IP: {pkt[IP].src} | 目标IP: {pkt[IP].dst} | TTL: {pkt[IP].ttl}")
        print(f"【传输层】源端口: {pkt[TCP].sport} | 目标端口: {pkt[TCP].dport} | TCP标志位: {pkt[TCP].flags}")
    # 3. 提取应用层字段（telnet，若有数据则显示）
    if Raw in pkt:
        app_data = pkt[Raw].load.hex()  # 转16进制，避免乱码
        print(f"【应用层】telnet数据（16进制）: {app_data[:50]}...")  # 只显示前50位
    print("-" * 50)