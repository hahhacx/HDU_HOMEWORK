SUBJECTS = ["chinese", "math", "english", "physics", "chemistry", "biology"]
SCORE_MIN = 0
SCORE_MAX = 100
SCALE = 1
AUTO_GEN = True
RNG_SEED = 42

# MPC 服务器数量（两台不串通的计算服务器）
N_SERVERS = 2

# Beaver 三元组池大小（两人比较时用到）
BEAVER_POOL = 256

# PBT（用途令牌）配置
TOKEN_SECRET = b"ppsa-demo-secret"  # 生产环境请放到 KMS
TOKEN_TTL_SECONDS = 3600

# 通道加密：未安装 cryptography 时会降级为明文（仅演示）
