"""
实际启用：X25519 ECDH + AES-GCM 安全通道。
若未安装 cryptography，将降级为明文，但仍有显式 WARNING（仅教学）。
"""
from typing import Tuple
import os

try:
    from cryptography.hazmat.primitives.asymmetric.x25519 import X25519PrivateKey
    from cryptography.hazmat.primitives.ciphers.aead import AESGCM
    HAVE_CRYPTO = True
except Exception:
    HAVE_CRYPTO = False

def ecdh_keypair():
    if not HAVE_CRYPTO:
        return ("PRIV", "PUB")
    sk = X25519PrivateKey.generate()
    pk = sk.public_key()
    return (sk, pk)

def ecdh_shared_key(sk, peer_pk) -> bytes:
    if not HAVE_CRYPTO:
        print("[WARN] cryptography not installed; channel runs in CLEAR TEXT.")
        return b"\x00"*32
    return sk.exchange(peer_pk)

class SecureChannel:
    """
    简化版通道：握手→派生会话密钥→send/recv 做 AES-GCM 加解密。
    注意：本项目是单进程教学模拟，仍真实调用加密原语。
    """
    def __init__(self, name: str):
        self.name = name
        self.sk, self.pk = ecdh_keypair()
        self.peer_pk = None
        self.key = None
        self._nonce_ctr = 0

    def handshake(self, peer_pk):
        self.peer_pk = peer_pk
        self.key = ecdh_shared_key(self.sk, peer_pk)

    def _nonce(self) -> bytes:
        if not HAVE_CRYPTO:
            return b"\x00"*12
        n = self._nonce_ctr
        self._nonce_ctr += 1
        return n.to_bytes(12, "big")

    def send(self, payload: bytes, aad: bytes=b"") -> Tuple[bytes, bytes]:
        if not HAVE_CRYPTO:
            return (b"", payload)
        aes = AESGCM(self.key[:32])
        nonce = self._nonce()
        ct = aes.encrypt(nonce, payload, aad)
        return (nonce, ct)

    def recv(self, nonce: bytes, ct: bytes, aad: bytes=b"") -> bytes:
        if not HAVE_CRYPTO:
            return ct
        aes = AESGCM(self.key[:32])
        return aes.decrypt(nonce, ct, aad)
