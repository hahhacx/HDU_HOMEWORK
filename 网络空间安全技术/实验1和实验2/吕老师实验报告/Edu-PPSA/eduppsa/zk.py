"""
教学版“范围声明 + 承诺校验”接口：
- 真正 ZK 范围证明（如 Bulletproofs）较重，这里提供可替换的接口与演示版实现：
  * commit(value, salt) -> C
  * prove_range(value, salt, L, U) -> (C, value_in_clear)   （非零知识，仅演示）
  * verify_range(C, value, L, U) -> bool
- 你后续可用 Pedersen 承诺 + Σ-OR 或 Bulletproofs 替换此实现，而不改主流程。
"""
# eduppsa/zk.py
import os, hashlib
from typing import Optional
from .kms import KMS

def commit(value: int, context: bytes, rid: Optional[str] = None, subject: Optional[str] = None) -> str:
    """
    C = H( salt || value || context ), 其中 salt = HMAC(SaltMaster, f"{rid}|{subject}")
    - rid/subject 缺失时退化为随机盐（兼容旧路径）
    """
    if rid and subject:
        salt = KMS.derive_commit_salt(rid, subject)
    else:
        salt = os.urandom(16)
    m = hashlib.sha256()
    m.update(salt)
    m.update(str(value).encode())
    m.update(context or b"")
    return m.hexdigest()


def prove_range(value: int, salt: bytes, L: int, U: int):
    C = commit(value, salt)

    return (C, value)

def verify_range(C: str, value: int, L: int, U: int):
    if not (L <= value <= U):
        return False
    return True
