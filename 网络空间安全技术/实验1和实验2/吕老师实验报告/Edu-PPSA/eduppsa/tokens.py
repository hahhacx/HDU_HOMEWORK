# eduppsa/tokens.py
import time, json, hmac, base64, hashlib
from typing import Dict, Any, Optional
from .kms import KMS

def _b64u(data: bytes) -> str:
    return base64.urlsafe_b64encode(data).decode().rstrip("=")
def _b64ud(s: str) -> bytes:
    pad = "=" * (-len(s) % 4)
    return base64.urlsafe_b64decode(s + pad)

DEFAULT_TTL = 300  # 5分钟

def issue_token(payload: Dict[str, Any], ttl: int = DEFAULT_TTL) -> str:
    now = int(time.time())
    claims = dict(payload)
    claims.setdefault("iat", now)
    claims.setdefault("exp", now + ttl)
    kid, key = KMS.current("token")
    claims["kid"] = kid
    body = json.dumps(claims, separators=(",", ":")).encode()
    sig = hmac.new(key, body, hashlib.sha256).hexdigest()
    return _b64u(body) + "." + sig

# eduppsa/tokens.py（节选）
def verify_token(token: str, expect_action: Optional[str] = None) -> Dict[str, Any]:
    try:
        body_b64, sig_hex = token.rsplit(".", 1)
        body = _b64ud(body_b64)
        claims = json.loads(body)
        kid = claims["kid"]
        key = KMS.get("token", kid)
        good = hmac.compare_digest(sig_hex, hmac.new(key, body, hashlib.sha256).hexdigest())
        if not good:
            raise PermissionError("bad signature")
        now = int(time.time())
        if claims.get("exp", 0) < now:
            raise PermissionError("token expired")
        if expect_action and claims.get("action") != expect_action:
            raise PermissionError(f"action mismatch: got {claims.get('action')}, expect {expect_action}")
        return claims
    except PermissionError:
        raise
    except Exception as e:
        raise PermissionError(f"invalid token: {repr(e)}")


