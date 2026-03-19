"""
两人安全比较（谁的分数更高），不泄露具体分数：
- 思想：把分数映射为长度 L=101 的一热向量 x,y（x[j]=1 表示分数=j），并做算术秘密分享（mod Q）。
- 目标：计算 GT = 1{ X > Y }，仅输出 {>,<,==}。
- 公式：GT = sum_j x[j] * (sum_{k<j} y[k])   （若 X=j 且 存在 k<j 使 y[k]=1，则 j>k）
- 用 Beaver 三元组实现“共享乘法”，两台服务器 S1,S2 计算，编排者只接收 GT 的重构值（0/1）。
"""
from typing import Tuple, List
import config
from .sharemath import Q, split_mod_q, add_mod_q, reconstruct_mod_q
from .beaver import BeaverDealer, beaver_mul_shared

def one_hot_index(score: int) -> int:
    s = max(config.SCORE_MIN, min(config.SCORE_MAX, int(score)))
    return s - config.SCORE_MIN

def shares_one_hot(score: int, n_servers: int) -> List[List[int]]:
    L = config.SCORE_MAX - config.SCORE_MIN + 1
    idx = one_hot_index(score)
    vec = [0]*L
    vec[idx] = 1
    # 向量每个分量独立拆分（mod Q）
    per_server = [ [0]*L for _ in range(n_servers) ]
    for j in range(L):
        sh = split_mod_q(vec[j], n_servers)
        for i in range(n_servers):
            per_server[i][j] = sh[i]
    return per_server

def secure_compare(score1: int, score2: int, dealer: BeaverDealer) -> Tuple[int,int,int]:
    """
    返回 (gt, lt, eq) 其中 gt/lt/eq ∈ {0,1} 且恰有一个为1。
    """
    n = config.N_SERVERS
    assert n == 2, "当前实现假设两台服务器"

    # 生成两人的一热向量秘密分享
    x_sh = shares_one_hot(score1, n)  # shape: [2][L]
    y_sh = shares_one_hot(score2, n)

    L = config.SCORE_MAX - config.SCORE_MIN + 1

    # 前缀和：pref_y[j] = sum_{k<j} y[k]
    # 在共享域内前缀相加（mod Q）
    pref_y_sh = [ [0]*L for _ in range(n) ]
    running = [0, 0]
    for j in range(L):
        # y_sh[*][j] 加到 running 里，pref_y[j+1] 用于下一步；但我们要的是 sum_{k<j}
        for i in range(n):
            pref_y_sh[i][j] = running[i]
        # update running += y[j]
        for i in range(n):
            running[i] = add_mod_q(running[i], y_sh[i][j])

    # 逐 j 计算 prod_j = x[j] * pref_y[j]（共享乘法）
    prod_sh_total = [0, 0]
    for j in range(L):
        triple = dealer.get_triple()
        xj_sh = [x_sh[0][j], x_sh[1][j]]
        py_sh = [pref_y_sh[0][j], pref_y_sh[1][j]]
        prod_sh = beaver_mul_shared(xj_sh, py_sh, triple)
        # 汇总 prod_sh 到总和
        for i in range(n):
            prod_sh_total[i] = add_mod_q(prod_sh_total[i], prod_sh[i])

    # 重构 GT = sum_j x[j] * (sum_{k<j} y[k])
    GT = reconstruct_mod_q(prod_sh_total) % Q
    GT = 1 if GT != 0 else 0

    # 同理可得 LT（交换 x,y 再算一次），EQ = 1 - GT - LT
    # 为省时，这里直接按值判断：若 GT=0 且 交换后也为0 则 EQ=1
    # —— 再算一遍（轻量）
    dealer2 = dealer  # 复用池
    prod_sh_total2 = [0, 0]
    # pref_x
    pref_x_sh = [ [0]*L for _ in range(n) ]
    running2 = [0, 0]
    for j in range(L):
        for i in range(n):
            pref_x_sh[i][j] = running2[i]
        for i in range(n):
            running2[i] = add_mod_q(running2[i], x_sh[i][j])
    for j in range(L):
        triple = dealer2.get_triple()
        yj_sh = [y_sh[0][j], y_sh[1][j]]
        px_sh = [pref_x_sh[0][j], pref_x_sh[1][j]]
        prod_sh = beaver_mul_shared(yj_sh, px_sh, triple)
        for i in range(n):
            prod_sh_total2[i] = add_mod_q(prod_sh_total2[i], prod_sh[i])
    LT = reconstruct_mod_q(prod_sh_total2) % Q
    LT = 1 if LT != 0 else 0

    EQ = 1 - GT - LT
    return (GT, LT, EQ)
