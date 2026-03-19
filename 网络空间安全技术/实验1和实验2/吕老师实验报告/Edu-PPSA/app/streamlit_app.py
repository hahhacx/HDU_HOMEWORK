# -*- coding: utf-8 -*-
import os
import json
import time
from pathlib import Path

import streamlit as st
import pandas as pd

# === 你的后端模块（保持原有实现） ===
import config
from eduppsa.utils import ensure_demo_students, read_students
from eduppsa.subjects import valid_subject
from eduppsa.audit import HashChainLog
from eduppsa.tokens import issue_token, verify_token
from eduppsa.protocols import (
    run_secure_mean_by_subject_secure,
    run_secure_max_by_subject_secure,
    run_secure_compare_two_v1_secure,
)
from eduppsa.kms import KMS
with st.expander("🔐 KMS 状态", expanded=False):
    st.json({
        "token_kids": KMS.list_versions("token"),
        "salt_master_kids": KMS.list_versions("salt_master"),
        "beaver_seed_kids": KMS.list_versions("beaver_seed"),
    })

# 可选：显示通道加密是否启用
try:
    from eduppsa.crypto import HAVE_CRYPTO
except Exception:
    HAVE_CRYPTO = False


# =============== 小工具 ===============
def load_ids(csv_path):
    try:
        rows = read_students(csv_path)
        return [r["id"] for r in rows]
    except Exception:
        return []

def save_uploaded_file(uploaded_file, dst_path):
    with open(dst_path, "wb") as f:
        f.write(uploaded_file.getbuffer())


# =============== UI 开始 ===============
st.set_page_config(
    page_title="Edu-PPSA · Privacy-preserving Student Analytics",
    page_icon="🔒",
    layout="wide",
)

st.title("🔒 Edu-PPSA · Privacy-preserving Student Analytics")
st.caption("在 **不泄露个人分数** 的前提下：科目均值、科目最高分、两人私密比较（MPC + ECDH/AES + Token + 审计链）")

# ====== 侧边栏：数据与状态 ======
st.sidebar.header("数据源")
data_dir = Path("data")
data_dir.mkdir(exist_ok=True)
default_csv = data_dir / "students.csv"

uploaded = st.sidebar.file_uploader("上传 CSV（可选）", type=["csv"])
if uploaded is not None:
    save_uploaded_file(uploaded, default_csv)
    st.sidebar.success("已覆盖保存到 data/students.csv")

col1, col2 = st.sidebar.columns(2)
with col1:
    if st.button("生成演示数据", use_container_width=True):
        ensure_demo_students(str(default_csv), n=50, seed=getattr(config, "RNG_SEED", 42))
        st.sidebar.success("已生成 50 条演示数据")
with col2:
    if st.button("刷新数据", use_container_width=True):
        st.rerun()

# 展示数据快照
if default_csv.exists():
    try:
        df_preview = pd.read_csv(default_csv)
        st.sidebar.dataframe(df_preview.head(), use_container_width=True, height=180)
        st.sidebar.caption(f"数据路径：{default_csv}")
    except Exception as e:
        st.sidebar.error(f"读取 CSV 失败：{e}")
else:
    st.sidebar.warning("未检测到 data/students.csv，可在侧边生成演示数据或上传。")

# 安全状态提示
with st.sidebar.expander("安全状态", expanded=True):
    st.write("**通道加密**：", "✅ 已启用 AES-GCM（cryptography）" if HAVE_CRYPTO else "⚠️ 未安装 cryptography，通道为明文（仅教学）")
    st.write("**威胁模型**：两台互不串通的 MPC 服务器 + 半诚实（honest-but-curious）")
    st.caption("提示：本界面与协议在同一进程内模拟，但仍真实调用加密/校验原语与审计链。")

# ====== 主体：三个协议卡片 ======
log = HashChainLog()

# 令牌签发（界面每次操作都新签一个，避免过期）
import time
if "tokens" not in st.session_state:
    st.session_state.tokens = {}

def get_token(action: str, scope: str, ttl: int = 600) -> str:
    k = (action, scope)
    now = int(time.time())
    info = st.session_state.tokens.get(k)
    # 提前 5 秒续期
    if (not info) or (now >= info["exp"] - 5):
        tok = issue_token({"action": action, "scope": scope}, ttl=ttl)
        st.session_state.tokens[k] = {"token": tok, "exp": now + ttl}
    return st.session_state.tokens[k]["token"]


st.markdown("---")
st.subheader("① 科目均值（SecureMean）")
c1, c2 = st.columns([2, 3])

with c1:
    subjects = st.multiselect(
        "选择科目",
        options=config.SUBJECTS,
        default=list(config.SUBJECTS),
        help="仅发布 {count, mean}，不泄露个人分数",
    )
    run_mean = st.button("计算均值", type="primary", use_container_width=True)
with c2:
    if run_mean:
        mean_token = get_token("mean", "all_subjects")
        if not default_csv.exists():
            st.error("数据文件不存在，请先上传或生成演示数据。")
        elif len(subjects) == 0:
            st.error("请至少选择一个科目。")
        else:
            try:
                res = run_secure_mean_by_subject_secure(str(default_csv), subjects, token=mean_token, log=log)
                df = pd.DataFrame(
                    [{"subject": s, "count": cnt, "mean": round(mn, 4)} for s, (cnt, mn) in res.items()]
                )
                st.success("均值计算完成（MPC + ECDH/AES + Token + 审计链）")
                st.dataframe(df, use_container_width=True, hide_index=True)
            except PermissionError as e:
                st.error(f"令牌无效：{e}")
            except Exception as e:
                st.exception(e)

st.markdown("---")
st.subheader("② 科目最高分（SecureMax）")
c3, c4 = st.columns([2, 3])

with c3:
    subjects2 = st.multiselect(
        "选择科目",
        options=config.SUBJECTS,
        default=list(config.SUBJECTS),
        key="max_subjects",
        help="仅发布 {max, frequency}；默认不公开完整直方图",
    )
    run_max = st.button("计算最高分", type="primary", use_container_width=True, key="btn_max")
with c4:
    if run_max:
        max_token = get_token("max", "all_subjects")
        if not default_csv.exists():
            st.error("数据文件不存在，请先上传或生成演示数据。")
        elif len(subjects2) == 0:
            st.error("请至少选择一个科目。")
        else:
            try:
                res = run_secure_max_by_subject_secure(str(default_csv), subjects2, token=max_token, log=log)
                df = pd.DataFrame(
                    [{"subject": s, "max": mx, "frequency": fq} for s, (mx, fq) in res.items()]
                )
                st.success("最高分计算完成（MPC + ECDH/AES + Token + 审计链）")
                st.dataframe(df, use_container_width=True, hide_index=True)
            except PermissionError as e:
                st.error(f"令牌无效：{e}")
            except Exception as e:
                st.exception(e)

st.markdown("---")
st.subheader("③ 两人私密比较（SecureCompare）")

ids = load_ids(str(default_csv)) if default_csv.exists() else []
c5, c6, c7 = st.columns(3)
with c5:
    subj_cmp = st.selectbox("科目", options=config.SUBJECTS if len(config.SUBJECTS)>0 else ["math"])
with c6:
    id1 = st.selectbox("学生 A", options=ids)
with c7:
    id2 = st.selectbox("学生 B", options=[i for i in ids if i != id1])

cmp_token = issue_token({"action": "compare", "scope": subj_cmp})
if st.button("开始比较", type="primary", use_container_width=True, key="btn_cmp"):
    cmp_token = get_token("compare", subj_cmp)
    if not default_csv.exists():
        st.error("数据文件不存在，请先上传或生成演示数据。")
    elif id1 == id2:
        st.error("请选择不同的两位学生。")
    else:
        try:
            res = run_secure_compare_two_v1_secure(str(default_csv), subj_cmp, id1, id2, token=cmp_token, log=log)
            if res is None:
                st.error("比较失败：找不到指定学生/科目")
            else:
                winner, relation = res
                if relation == "==":
                    st.info(f"结果：{id1} == {id2}（{subj_cmp}）")
                elif relation == ">":
                    st.success(f"结果：{id1} > {id2}（{subj_cmp}） → 胜者：{winner}")
                else:
                    st.success(f"结果：{id1} < {id2}（{subj_cmp}） → 胜者：{winner}")
        except PermissionError as e:
            st.error(f"令牌无效：{e}")
        except Exception as e:
            st.exception(e)

# ====== 审计与配置 ======
st.markdown("---")
with st.expander("🧾 审计（哈希链）", expanded=False):
    try:
        # 简单显示最近若干条事件的前缀与链尾 tip
        # HashChainLog 在你的实现中通常有 append(...) / tip()；这里尽量温柔地做展示
        tip = log.tip()
        st.code(f"Audit tip: {tip}", language="text")
        st.caption("说明：每次计算会把承诺前缀与聚合事件写入审计链。")
    except Exception:
        st.text("日志实现不支持查看事件详情，仅展示 tip。")

with st.expander("⚙️ 配置", expanded=False):
    st.json({
        "SUBJECTS": list(config.SUBJECTS),
        "SCORE_RANGE": [config.SCORE_MIN, config.SCORE_MAX],
        "N_SERVERS": config.N_SERVERS,
        "BEAVER_POOL": getattr(config, "BEAVER_POOL", 4096),
        "TOKEN_TTL_SECONDS": getattr(config, "TOKEN_TTL_SECONDS", 300),
        "CHANNEL_ENCRYPTION": "AES-GCM via X25519-ECDH" if HAVE_CRYPTO else "CLEAR (teaching only)",
    })

st.markdown(
    "<div style='text-align:center;color:#888'>© Edu-PPSA — MPC + ECDH/AES + Token + Audit Chain</div>",
    unsafe_allow_html=True
)
