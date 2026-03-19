# 加法秘密分享与重构：模大素数 q 的整数域（算术共享）
from typing import List, Tuple
import secrets

# 使用 61 位梅森素数作为既快又大的模（安全演示）
Q = 2**61 - 1

def split_mod_q(value: int, n: int) -> List[int]:
    """把 value 按模 Q 拆成 n 份分享（相加 mod Q = value）。"""
    if n <= 1:
        return [value % Q]
    shares = []
    acc = 0
    for _ in range(n - 1):
        r = secrets.randbits(60) % Q
        shares.append(r)
        acc = (acc + r) % Q
    shares.append((value - acc) % Q)
    return shares

def add_mod_q(a: int, b: int) -> int:
    return (a + b) % Q

def sub_mod_q(a: int, b: int) -> int:
    return (a - b) % Q

def mul_mod_q(a: int, b: int) -> int:
    # 适用于 Q 为 2^61-1 的快速乘法也可直接用 Python 大整数
    return (a * b) % Q

def reconstruct_mod_q(shares: List[int]) -> int:
    return sum(shares) % Q
