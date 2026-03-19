from typing import List, Dict, Tuple
import config
from .secretsharing import split_additive, reconstruct

# 一热直方图秘密分享：把分值x编码为长度L的一热向量，然后每个维度做加法秘密分享
def one_hot_share(score: int, n: int, L: int) -> List[List[int]]:
    # 返回 n 份分享，每份是长度为L的向量
    vec = [0]*L
    idx = max(0, min(L-1, score - config.SCORE_MIN))
    vec[idx] = 1
    shares = [ [0]*L for _ in range(n) ]
    for j,val in enumerate(vec):
        parts = split_additive(val, n)
        for i in range(n):
            shares[i][j] = parts[i]
    return shares

def aggregate_hist_shares(list_of_party_shares: List[List[int]]) -> List[int]:
    # 输入：多个参与者的“同一服务器份额”（长度L向量），输出：该服务器上的份额之和
    L = len(list_of_party_shares[0])
    acc = [0]*L
    for vec in list_of_party_shares:
        for j in range(L):
            acc[j] += vec[j]
    return acc

def reconstruct_hist(servers_shares: List[List[int]]) -> List[int]:
    # 各服务器上的直方图份额重构为总直方图
    L = len(servers_shares[0])
    res = [0]*L
    for j in range(L):
        res[j] = sum( servers_shares[i][j] for i in range(len(servers_shares)) )
    return res

def get_max_from_hist(hist: List[int]) -> Tuple[int,int]:
    # 返回 (最高分, 该最高分出现次数)
    for j in range(len(hist)-1, -1, -1):
        if hist[j] > 0:
            return (config.SCORE_MIN + j, hist[j])
    return (config.SCORE_MIN, 0)
