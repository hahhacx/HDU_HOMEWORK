"""
Beaver 乘法三元组（算术共享）：(a,b,c)，其中 c = a*b (mod Q)
- 这里给出“有可信经销商/预处理”的教学版实现：Dealer 生成 (a,b,c) 并拆分给两台服务器。
- 在半诚实模型下，用于计算共享数的乘法：给两方各自的 x_i, y_i，得到 z_i，使得
  z = (x1+x2)*(y1+y2) (mod Q) 且 z = z1+z2 (mod Q)
"""
from typing import Tuple, List
from .sharemath import Q, split_mod_q, add_mod_q, sub_mod_q, mul_mod_q

import hashlib, hmac, struct
from typing import Tuple, List
from .kms import KMS
import config

Q = (1 << 61) - 1  # 与 secretsharing 一致

def _prg_words(seed: bytes, count: int) -> List[int]:
    """HMAC-SHA256(counter) 作为简易PRG，输出 count 个 64bit，然后 mod Q"""
    out = []
    counter = 0
    while len(out) < count:
        counter_bytes = struct.pack(">Q", counter)
        block = hmac.new(seed, b"beaver|" + counter_bytes, hashlib.sha256).digest()
        for i in range(0, len(block), 8):
            w = int.from_bytes(block[i:i+8], "big")
            out.append(w % Q)
            if len(out) >= count: break
        counter += 1
    return out

def generate_beaver_batch(n_triples: int, batch_id: int) -> Tuple[List[int], List[int], List[int], str]:
    """
    生成 n_triples 组三元组 (a,b,c=a*b mod Q)
    返回 (A,B,C, seed_kid)；调用方可把 seed_kid 与 batch_id 记入审计链
    """
    seed_kid, seed_key = KMS.current("beaver_seed")
    seed = KMS.hkdf(seed_key, info=f"beaver|{batch_id}".encode())
    A = _prg_words(seed + b"A", n_triples)
    B = _prg_words(seed + b"B", n_triples)
    C = [(a * b) % Q for a, b in zip(A, B)]
    return A, B, C, seed_kid

class BeaverDealer:
    def __init__(self, n_servers: int = 2, pool_size: int = 256):
        self.n = n_servers
        self.pool: List[Tuple[List[int], List[int], List[int]]] = []
        for _ in range(pool_size):
            # 随机 a,b in Z_q
            import secrets
            a = secrets.randbits(60) % Q
            b = secrets.randbits(60) % Q
            c = mul_mod_q(a, b)
            a_sh = split_mod_q(a, self.n)
            b_sh = split_mod_q(b, self.n)
            c_sh = split_mod_q(c, self.n)
            self.pool.append((a_sh, b_sh, c_sh))

    def get_triple(self):
        assert self.pool, "Beaver triples exhausted"
        return self.pool.pop()

def beaver_mul_shared(x_sh: List[int], y_sh: List[int], triple):
    """
    两台服务器各持有 x_i, y_i；triple = (a_sh, b_sh, c_sh)
    返回 z_sh（两台服务器的结果份额）
    协议（半诚实）：
      d = (x - a)  （重构）
      e = (y - b)  （重构）
      z = c + d*b + e*a + d*e
    各项运算都在 mod Q。
    """
    a_sh, b_sh, c_sh = triple
    n = len(x_sh)
    assert n == 2 and len(y_sh) == 2

    # 每台服务器本地算 (x_i - a_i), (y_i - b_i)，然后重构 d,e
    from .sharemath import sub_mod_q, reconstruct_mod_q, mul_mod_q, add_mod_q
    d_sh = [sub_mod_q(x_sh[i], a_sh[i]) for i in range(n)]
    e_sh = [sub_mod_q(y_sh[i], b_sh[i]) for i in range(n)]
    d = reconstruct_mod_q(d_sh)
    e = reconstruct_mod_q(e_sh)

    # 每台服务器本地计算：
    # z_i = c_i + d*b_i + e*a_i + (i==0 ? d*e : 0)
    # 这样两份相加即等于 c + d*b + e*a + d*e
    z_sh = []
    for i in range(n):
        term = c_sh[i]
        term = add_mod_q(term, mul_mod_q(d, b_sh[i]))
        term = add_mod_q(term, mul_mod_q(e, a_sh[i]))
        if i == 0:
            term = add_mod_q(term, mul_mod_q(d, e))
        z_sh.append(term)
    return z_sh
