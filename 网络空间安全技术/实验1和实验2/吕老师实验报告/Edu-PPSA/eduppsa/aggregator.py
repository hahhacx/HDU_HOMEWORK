from typing import List
from .secretsharing import reconstruct

# 中继/编排节点：只重构“聚合结果”，不接触单条
class Aggregator:
    def __init__(self, n_servers: int = 2):
        self.n_servers = n_servers

    def reconstruct_sum(self, server_shares: List[int]) -> int:
        # server_shares: 来自各服务器的“某一聚合值”的份额
        return reconstruct(server_shares)
