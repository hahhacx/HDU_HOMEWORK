"""
Paillier 同态加法（教育实现，非门限）：
- 仅用于演示 Enc/Dec 与同态加法；主流程未强制依赖。
- 真正门限 Paillier 可在此接口上替换实现（DKG、t-of-n 解密）。
"""
import math, random
from typing import Tuple

def _l_function(u: int, n: int) -> int:
    return (u - 1) // n

def _invmod(a: int, m: int) -> int:
    return pow(a, -1, m)

def _gen_prime(bits: int) -> int:
    # 简易 Miller-Rabin
    import secrets
    def is_probable_prime(n, k=8):
        if n < 4: return n in (2,3)
        if n % 2 == 0: return False
        # write n-1 as 2^r * d
        r, d = 0, n-1
        while d % 2 == 0:
            r += 1; d //= 2
        for _ in range(k):
            a = secrets.randbelow(n-3) + 2
            x = pow(a, d, n)
            if x in (1, n-1):
                continue
            skip = False
            for _ in range(r-1):
                x = pow(x, 2, n)
                if x == n-1:
                    skip = True; break
            if not skip:
                return False
        return True
    while True:
        cand = secrets.randbits(bits) | 1
        if is_probable_prime(cand):
            return cand

def keygen(bits: int = 1024):
    p = _gen_prime(bits//2)
    q = _gen_prime(bits//2)
    while p == q:
        q = _gen_prime(bits//2)
    n = p*q
    lam = (p-1)*(q-1) // math.gcd(p-1, q-1)   # lcm(p-1, q-1)
    n2 = n*n
    g = n + 1
    mu = _invmod(_l_function(pow(g, lam, n2), n), n)
    pk = (n, g)
    sk = (lam, mu)
    return pk, sk

def enc(pk, m: int, r: int = None) -> int:
    n, g = pk
    n2 = n*n
    if r is None:
        import secrets
        r = secrets.randbelow(n)
        while math.gcd(r, n) != 1:
            r = secrets.randbelow(n)
    return (pow(g, m, n2) * pow(r, n, n2)) % n2

def dec(pk, sk, c: int) -> int:
    n,_ = pk; lam, mu = sk
    n2 = n*n
    u = pow(c, lam, n2)
    l = _l_function(u, n)
    return (l * mu) % n

def hom_add(pk, c1: int, c2: int) -> int:
    n,_ = pk; n2 = n*n
    return (c1 * c2) % n2

def hom_add_plain(pk, c: int, m: int) -> int:
    n,g = pk; n2 = n*n
    return (c * pow(g, m, n2)) % n2
