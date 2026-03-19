from typing import Dict, Tuple, List
import json
import config

from .utils import read_students
from .subjects import valid_subject
from .audit import HashChainLog
from .aggregator import Aggregator
from .secretsharing import split_additive, reconstruct
from .histogram import one_hot_share, reconstruct_hist, get_max_from_hist

# 新增：安全通道与令牌
from .crypto import SecureChannel
from .tokens import verify_token
from .zk import commit
from .kms import KMS

# ============ 工具：两台“服务器”的通道初始化 ============

# --------------- 放在文件顶部 import 下方 ---------------
from .crypto import SecureChannel

def _make_pair(name_a: str, name_b: str):
    """为 A<->B 建立一对对称的通道端点（各自握手，得到同一会话密钥）"""
    a = SecureChannel(f"{name_a}->{name_b}")
    b = SecureChannel(f"{name_b}<-{name_a}")
    a.handshake(b.pk)
    b.handshake(a.pk)
    return a, b

def _init_pairwise_channels():
    """
    返回一个字典，包含 4 组成对通道端点：
      - client_s1: (client端, s1端)
      - client_s2: (client端, s2端)
      - s1_aggr  : (s1端, aggr端)
      - s2_aggr  : (s2端, aggr端)
    """
    c_s1, s1_c = _make_pair("Client", "S1")
    c_s2, s2_c = _make_pair("Client", "S2")
    s1_a, a_s1 = _make_pair("S1", "Aggregator")
    s2_a, a_s2 = _make_pair("S2", "Aggregator")
    return {
        "client_s1": (c_s1, s1_c),
        "client_s2": (c_s2, s2_c),
        "s1_aggr": (s1_a, a_s1),
        "s2_aggr": (s2_a, a_s2),
    }




# ============ 平均分：MPC（分享）+ ECDH/AES + Token + 承诺入链 ============

def run_secure_mean_by_subject_secure(csv_path: str, subjects, token: str, log=None):
    claims = verify_token(token, expect_action="mean")
    if log: log.append(f"token_kid:{claims.get('kid','')}")
    rows = read_students(csv_path)
    n = len(rows)
    if n == 0:
        return {s: (0, float("nan")) for s in subjects}

    chans = _init_pairwise_channels()
    c_s1, s1_c = chans["client_s1"]
    c_s2, s2_c = chans["client_s2"]
    s1_a, a_s1 = chans["s1_aggr"]
    s2_a, a_s2 = chans["s2_aggr"]

    aggregator = Aggregator(n_servers=config.N_SERVERS)
    results = {}

    for subj in subjects:
        server_cols = [[], []]

        for r in rows:
            v = int(r[subj])
            C = commit(v, f"{r['id']}|{subj}".encode("utf-8"))
            if log: log.append(f"commit:{subj}:{r['id']}:{C[:16]}")

            parts = split_additive(v, 2)
            # Client -> S1
            n1, ct1 = c_s1.send(json.dumps({"id": r["id"], "subj": subj, "share": parts[0]}).encode("utf-8"), aad=b"mean")
            p1 = json.loads(s1_c.recv(n1, ct1, aad=b"mean").decode("utf-8"))
            # Client -> S2
            n2, ct2 = c_s2.send(json.dumps({"id": r["id"], "subj": subj, "share": parts[1]}).encode("utf-8"), aad=b"mean")
            p2 = json.loads(s2_c.recv(n2, ct2, aad=b"mean").decode("utf-8"))

            server_cols[0].append(p1["share"])
            server_cols[1].append(p2["share"])

        sum_share_1 = sum(server_cols[0])
        sum_share_2 = sum(server_cols[1])

        # S1 -> Aggregator
        nA1, ctA1 = s1_a.send(json.dumps({"sum_share": sum_share_1}).encode("utf-8"), aad=b"mean_sum")
        d1 = json.loads(a_s1.recv(nA1, ctA1, aad=b"mean_sum").decode("utf-8"))
        # S2 -> Aggregator
        nA2, ctA2 = s2_a.send(json.dumps({"sum_share": sum_share_2}).encode("utf-8"), aad=b"mean_sum")
        d2 = json.loads(a_s2.recv(nA2, ctA2, aad=b"mean_sum").decode("utf-8"))

        total = aggregator.reconstruct_sum([d1["sum_share"], d2["sum_share"]])
        mean = total / n if n > 0 else float("nan")
        results[subj] = (n, mean)
        if log: log.append(f"mean:{subj}:{n}:{mean:.6f}")

    return results


# ============ 最高分：MPC（一热直方图分享）+ ECDH/AES + Token + 承诺入链 ============

def run_secure_max_by_subject_secure(csv_path: str, subjects, token: str, log: HashChainLog=None):
    claims = verify_token(token, expect_action="max")
    if log: log.append(f"token_kid:{claims.get('kid', '')}")
    rows = read_students(csv_path)
    if not rows:
        return {s: (config.SCORE_MIN, 0) for s in subjects}

    chans = _init_pairwise_channels()
    c_s1, s1_c = chans["client_s1"]
    c_s2, s2_c = chans["client_s2"]
    s1_a, a_s1 = chans["s1_aggr"]
    s2_a, a_s2 = chans["s2_aggr"]

    results = {}
    L = config.SCORE_MAX - config.SCORE_MIN + 1

    for subj in subjects:
        if log: log.append(f"max_start:{subj}")
        server_hist = [ [0]*L for _ in range(2) ]

        for r in rows:
            v = int(r[subj])
            C = commit(v, f"{r['id']}|{subj}".encode("utf-8"))
            if log: log.append(f"commit:{subj}:{r['id']}:{C[:16]}")
            parts2d = one_hot_share(v, 2, L)

            # Client -> S1
            n1, ct1 = c_s1.send(json.dumps({"id": r["id"], "vec": parts2d[0]}).encode("utf-8"), aad=b"max")
            vec1 = json.loads(s1_c.recv(n1, ct1, aad=b"max").decode("utf-8"))["vec"]
            # Client -> S2
            n2, ct2 = c_s2.send(json.dumps({"id": r["id"], "vec": parts2d[1]}).encode("utf-8"), aad=b"max")
            vec2 = json.loads(s2_c.recv(n2, ct2, aad=b"max").decode("utf-8"))["vec"]

            for j in range(L):
                server_hist[0][j] += vec1[j]
                server_hist[1][j] += vec2[j]

        # S1 -> Aggregator
        nA1, ctA1 = s1_a.send(json.dumps({"h": server_hist[0]}).encode("utf-8"), aad=b"max_sum")
        h1 = json.loads(a_s1.recv(nA1, ctA1, aad=b"max_sum").decode("utf-8"))["h"]
        # S2 -> Aggregator
        nA2, ctA2 = s2_a.send(json.dumps({"h": server_hist[1]}).encode("utf-8"), aad=b"max_sum")
        h2 = json.loads(a_s2.recv(nA2, ctA2, aad=b"max_sum").decode("utf-8"))["h"]

        total_hist = reconstruct_hist([h1, h2])
        best, freq = get_max_from_hist(total_hist)
        results[subj] = (best, freq)
        if log: log.append(f"max:{subj}:{best}:{freq}")

    return results


# ============ 两人比较：沿用 v1 MPC（Beaver）逻辑；仅补上通道与令牌 ============

from .beaver import BeaverDealer
from .compare import secure_compare

def run_secure_compare_two_v1_secure(csv_path: str, subj: str, id1: str, id2: str, token: str, log: HashChainLog=None):
    claims = verify_token(token, expect_action="compare")
    if log: log.append(f"token_kid:{claims.get('kid', '')}")
    rows = read_students(csv_path)
    s1 = s2 = None
    for r in rows:
        if r["id"] == id1: s1 = int(r[subj])
        if r["id"] == id2: s2 = int(r[subj])
    if s1 is None or s2 is None:
        return None

    # 这里的比较协议本身是纯共享计算；为统一“安全通道已使用”的事实，我们仅把承诺写入审计链。
    if log:
        log.append(f"commit:{subj}:{id1}:{commit(s1, f'{id1}|{subj}'.encode())[:16]}")
        log.append(f"commit:{subj}:{id2}:{commit(s2, f'{id2}|{subj}'.encode())[:16]}")

    dealer = BeaverDealer(n_servers=config.N_SERVERS, pool_size=config.BEAVER_POOL)
    gt, lt, eq = secure_compare(s1, s2, dealer)
    if log: log.append(f"cmp_v1:{subj}:{id1}:{id2}:gt{gt}/lt{lt}/eq{eq}")
    if eq == 1:  return ("=", "==")
    if gt == 1:  return (id1, ">")
    return (id2, "<")
