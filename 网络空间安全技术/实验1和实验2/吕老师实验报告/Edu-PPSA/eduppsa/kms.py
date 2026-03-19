# eduppsa/kms.py
import os, json, time, hmac, hashlib, secrets
from pathlib import Path
from typing import Dict, Tuple
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives import hashes

KMS_FILE = Path("secrets/kms_state.json")        # 加密后落盘
MASTER_ENV = "KMS_MASTER_KEY_HEX"               # 64 hex bytes 从环境/TPM注入
os.makedirs("secrets", exist_ok=True)

def _get_master() -> bytes:
    key_hex = os.environ.get(MASTER_ENV)
    if not key_hex:
        raise RuntimeError(f"Missing {MASTER_ENV}")
    return bytes.fromhex(key_hex)

def _aead(key: bytes, nonce: bytes, aad: bytes, data: bytes, enc=True) -> bytes:
    from cryptography.hazmat.primitives.ciphers.aead import AESGCM
    a = AESGCM(key)
    return (a.encrypt(nonce, data, aad) if enc else a.decrypt(nonce, data, aad))

def _seal(obj: dict) -> dict:
    raw = json.dumps(obj, sort_keys=True).encode()
    mk = _get_master()[:32]
    nonce = secrets.token_bytes(12)
    ct = _aead(mk, nonce, b"kms", raw, enc=True)
    return {"nonce": nonce.hex(), "ct": ct.hex()}

def _open(blob: dict) -> dict:
    mk = _get_master()[:32]
    raw = _aead(mk, bytes.fromhex(blob["nonce"]), b"kms", bytes.fromhex(blob["ct"]), enc=False)
    return json.loads(raw)

def _load_state() -> dict:
    if not KMS_FILE.exists(): return {}
    return _open(json.loads(KMS_FILE.read_text()))

def _save_state(state: dict):
    KMS_FILE.write_text(json.dumps(_seal(state)))

def _hkdf(ikm: bytes, info: bytes, salt: bytes=b"") -> bytes:
    hkdf = HKDF(algorithm=hashes.SHA256(), length=32, salt=salt, info=info)
    return hkdf.derive(ikm)

class KMS:
    """
    版本化密钥仓：支持类别：token, id_ed25519, salt_master, beaver_seed, (optional) paillier
    """
    @staticmethod
    def list_versions(cat: str):
        st = _load_state(); return list(st.get(cat, {}).keys())

    @staticmethod
    def current(cat: str) -> Tuple[str, bytes]:
        st = _load_state(); vv = st.get(cat, {})
        if not vv: raise RuntimeError(f"No key for {cat}")
        kid = max(vv.keys(), key=lambda k: vv[k]["created"])
        return kid, bytes.fromhex(vv[kid]["key"])

    @staticmethod
    def get(cat: str, kid: str) -> bytes:
        st = _load_state(); v = st.get(cat, {}).get(kid)
        if not v: raise KeyError(f"{cat}:{kid}")
        return bytes.fromhex(v["key"])

    @staticmethod
    def rotate(cat: str, size: int=32) -> str:
        st = _load_state()
        kid = f"{int(time.time())}"
        key = secrets.token_bytes(size).hex()
        st.setdefault(cat, {})[kid] = {"key": key, "created": int(time.time())}
        _save_state(st)
        return kid

    @staticmethod
    def hkdf(ikm: bytes, info: bytes, salt: bytes=b"") -> bytes:
        return _hkdf(ikm, info, salt)

    @staticmethod
    def derive_commit_salt(record_id: str, subject: str) -> bytes:
        # salt = HMAC(SaltMaster, f"{record_id}|{subject}")
        kid, sm = KMS.current("salt_master")
        msg = f"{record_id}|{subject}".encode()
        return hmac.new(sm, msg, hashlib.sha256).digest()
