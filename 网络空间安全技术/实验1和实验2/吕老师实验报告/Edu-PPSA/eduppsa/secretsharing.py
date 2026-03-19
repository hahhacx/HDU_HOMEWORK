import secrets
from typing import List, Tuple

# 加法秘密分享：在整数环 Z（Python大整数）中分成 n 份，求和等于原值
def split_additive(value: int, n: int) -> List[int]:
    if n <= 1:
        return [value]
    shares = []
    acc = 0
    for _ in range(n - 1):
        r = secrets.randbelow(10**9) - 5*10**8
        shares.append(r)
        acc += r
    shares.append(value - acc)
    return shares

def reconstruct(shares: List[int]) -> int:
    return sum(shares)

# 对一组数求和的秘密分享（每个数各自拆分，再逐列相加）
def secret_share_sum(values: List[int], n: int) -> List[int]:
    cols = [0]*n
    for v in values:
        ss = split_additive(v, n)
        cols = [a + b for a, b in zip(cols, ss)]
    return cols
