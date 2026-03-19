import hashlib
from typing import List

class HashChainLog:
    def __init__(self):
        self.chain: List[str] = []

    def append(self, msg: str):
        prev = self.chain[-1] if self.chain else ''
        h = hashlib.sha256((prev + msg).encode('utf-8')).hexdigest()
        self.chain.append(h)

    def tip(self) -> str:
        return self.chain[-1] if self.chain else ''
