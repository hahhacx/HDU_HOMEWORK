from typing import Dict, List, Tuple
from .utils import read_students, to_int
from .subjects import valid_subject
from .secretsharing import split_additive
from .histogram import one_hot_share
import config

class StudentParty:
    """
    站在“每个学生在本地”的视角：
    - 给定科目 s，生成“自己的分数”的秘密分享（用于求和/均值）
    - 生成自己的“一热直方图分享”（用于最高分）
    """
    def __init__(self, csv_path: str):
        # 简化：载入到内存，实际可只读自己的行
        self.rows = read_students(csv_path)

    def get_score(self, sid: str, subj: str) -> int:
        if not valid_subject(subj): return None
        for r in self.rows:
            if r["id"] == sid:
                return int(r[subj])
        return None

    def shares_for_sum(self, subj: str, n_servers: int = 2) -> List[List[int]]:
        # 返回：每个服务器一个列表（该学生的“分数秘密分享”）
        # v0：演示聚合，所以把所有学生都视为“本地可读”；真实部署每人只生成自己的分享
        shares_per_server: List[List[int]] = [[] for _ in range(n_servers)]
        for r in self.rows:
            v = int(r[subj])
            parts = split_additive(to_int(v), n_servers)
            for i in range(n_servers):
                shares_per_server[i].append(parts[i])
        return shares_per_server

    def shares_for_hist(self, subj: str, n_servers: int = 2) -> List[List[int]]:
        # 返回：每个服务器一个“直方图向量份额”
        L = config.SCORE_MAX - config.SCORE_MIN + 1
        # 累加每个人的一热份额（本地）
        acc = [ [0]*L for _ in range(n_servers) ]
        for r in self.rows:
            v = int(r[subj])
            parts2d = one_hot_share(v, n_servers, L)
            for i in range(n_servers):
                for j in range(L):
                    acc[i][j] += parts2d[i][j]
        return acc

    # —— 两人比较（mock）：功能等价，后续切Paillier/OT —— #
    def compare_two_mock(self, subj: str, id1: str, id2: str):
        s1 = self.get_score(id1, subj)
        s2 = self.get_score(id2, subj)
        if s1 is None or s2 is None: return None
        if s1 > s2:  return (id1, ">")
        if s1 < s2:  return (id2, "<")
        return ("=", "==")
