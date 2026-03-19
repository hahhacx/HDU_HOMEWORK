# ========== 模块1：导入依赖 ==========
from scapy.all import *
import re
import smtplib
from email.mime.text import MIMEText
from email.header import Header
from datetime import datetime

# ========== 模块2：配置基础信息（必须替换为你的信息） ==========
# 1. 文件路径配置
RULE_FILE_PATH = r"C:\snort\community_rules\community.rules"  # Snort社区规则文件路径
PCAP_FILE_PATH = "telnet.pcap"  # 你的数据包文件（已复制到项目根目录）

# 2. 邮件配置
SENDER_EMAIL = "3068299425@qq.com"  # 发件人邮箱
RECEIVER_EMAIL = "3068299425@qq.com"  # 收件人邮箱（可和发件人相同）
SMTP_SERVER = "smtp.qq.com"  # QQ邮箱SMTP服务器
SMTP_PORT = 587  # SMTP端口
SMTP_AUTH_CODE = "bxnomtfopwtldggh"  # 不是登录密码，是QQ邮箱的SMTP授权码


# ========== 模块3：解析Snort社区规则 ==========
def parse_snort_rules(rule_file):
    """读取并解析Snort规则，提取检测条件"""
    rules = []
    with open(rule_file, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()
            # 跳过注释和空行
            if not line or line.startswith("#"):
                continue

            # 匹配alert类型的规则（只处理告警规则）
            if line.startswith("alert"):
                # 用正则提取规则关键字段
                # 示例规则格式：alert tcp any any -> 192.168.146.128 23 (msg:"..."; content:"root"; ...)
                rule_match = re.match(
                    r"alert (\w+) (\S+) (\S+) -> (\S+) (\S+) \((.*)\)", line
                )
                if rule_match:
                    protocol = rule_match.group(1)  # 协议（tcp/udp等）
                    src_ip = rule_match.group(2)  # 源IP（any表示任意）
                    src_port = rule_match.group(3)  # 源端口（any表示任意）
                    dst_ip = rule_match.group(4)  # 目标IP
                    dst_port = rule_match.group(5)  # 目标端口
                    options = rule_match.group(6)  # 规则选项（msg、content等）

                    # 提取规则选项中的msg、content、flags
                    msg = re.search(r'msg:"(.*?)";', options).group(1) if re.search(r'msg:"(.*?)";',
                                                                                    options) else "Unknown Alert"
                    content = re.search(r'content:"(.*?)";', options).group(1) if re.search(r'content:"(.*?)";',
                                                                                            options) else None
                    flags = re.search(r'flags:(\S+);', options).group(1) if re.search(r'flags:(\S+);',
                                                                                      options) else None

                    # 将解析后的规则存入列表
                    rules.append({
                        "protocol": protocol,
                        "src_ip": src_ip,
                        "src_port": src_port,
                        "dst_ip": dst_ip,
                        "dst_port": dst_port,
                        "msg": msg,
                        "content": content,
                        "flags": flags
                    })
    return rules


# ========== 模块4：匹配数据包与Snort规则 ==========
def match_rule(pkt, rule):
    """检查单个数据包是否匹配某条Snort规则"""
    # 1. 匹配协议（如tcp）
    if rule["protocol"].upper() == "TCP" and TCP not in pkt:
        return False
    if rule["protocol"].upper() == "UDP" and UDP not in pkt:
        return False

    # 2. 匹配目标IP（你的实验主机IP）
    if IP in pkt and rule["dst_ip"] != "any" and pkt[IP].dst != rule["dst_ip"]:
        return False

    # 3. 匹配目标端口（23端口）
    if TCP in pkt and rule["dst_port"] != "any" and str(pkt[TCP].dport) != rule["dst_port"]:
        return False

    # 4. 匹配TCP标志位（如S）
    if rule["flags"] and TCP in pkt and rule["flags"] not in pkt[TCP].flags:
        return False

    # 5. 匹配应用层内容（如"root"）
    if rule["content"] and Raw in pkt:
        app_data = pkt[Raw].load.decode(errors="ignore")  # 解码应用层数据
        if rule["content"].lower() not in app_data.lower():
            return False

    # 所有条件都满足，匹配成功
    return True


# ========== 模块5：发送邮件告警 ==========
def send_alert_email(alert_msg, attack_count):
    """极简纯文本邮件，适配QQ邮箱"""
    # 纯文本内容（无HTML，避免格式错误）
    email_content = f"""
Snort NIDS Alert
⚠️ Detected {attack_count} abnormal packets!
Alert Message: {alert_msg}
Detection Time: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
Host IP: 192.168.146.128
    """
    # 直接创建纯文本MIMEText，不调用attach
    msg = MIMEText(email_content, "plain", "utf-8")
    msg["From"] = f"Snort NIDS <{SENDER_EMAIL}>"
    msg["To"] = f"Admin <{RECEIVER_EMAIL}>"
    msg["Subject"] = Header("Snort Alert: Telnet Root Login Attempt", "utf-8")

    try:
        smtp = smtplib.SMTP("smtp.qq.com", 587, timeout=30)
        smtp.starttls()
        smtp.login(SENDER_EMAIL, SMTP_AUTH_CODE)
        smtp.sendmail(SENDER_EMAIL, [RECEIVER_EMAIL], msg.as_string())
        smtp.quit()
        print("✅  告警邮件已成功发送！")
    except Exception as e:
        print(f"❌  邮件发送失败：{e}")


# ========== 模块6：主程序（核心逻辑） ==========
if __name__ == "__main__":
    # 1. 解析Snort社区规则
    print("正在解析Snort社区规则...")
    snort_rules = parse_snort_rules(RULE_FILE_PATH)
    if not snort_rules:
        print("❌  未找到有效的Snort规则，请检查规则文件！")
        exit()
    print(f"✅  成功解析 {len(snort_rules)} 条Snort规则")

    # 2. 读取数据包文件
    print("\n正在读取数据包文件...")
    try:
        packets = rdpcap(PCAP_FILE_PATH)
        print(f"✅  成功读取 {len(packets)} 个数据包")
    except FileNotFoundError:
        print(f"❌  数据包文件不存在：{PCAP_FILE_PATH}")
        exit()

    # 3. 匹配规则与数据包，统计异常包
    print("\n正在检测异常数据包...")
    attack_count = 0
    matched_msg = ""
    for pkt in packets:
        for rule in snort_rules:
            if match_rule(pkt, rule):
                attack_count += 1
                matched_msg = rule["msg"]
                print(f"⚠️  检测到异常包！匹配规则：{rule['msg']}")

    # 4. 有异常则发送邮件
    if attack_count > 0:
        print(f"\n总计检测到 {attack_count} 个异常数据包")
        send_alert_email(matched_msg, attack_count)
    else:
        print("\n未检测到异常数据包")