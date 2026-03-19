# Edu-PPSA (Privacy-preserving Student Analytics)

**目标**：在**不泄露任何个人分数**的前提下，完成学生成绩的隐私统计与查询：

* 按科目**均值**（仅发布 `count, mean`）
* 按科目**最高分**（仅发布 `max, frequency`）
* **两人私密比较**（只返回 `>, <, ==`；不泄露分值）

系统采用两台**互不串通**的安全计算服务器（S1/S2）+ 一个**编排/中继**（只重构聚合结果）。威胁模型为**半诚实（honest-but-curious）**。

---

## 快速开始

```bash
# （可选）启用加密通道功能
pip install cryptography

# 运行演示
python main.py
```
## 跑可视化交互界面
python -m streamlit run app\streamlit_app.py

## 密钥管理
# 设定主密钥
$env:KMS_MASTER_KEY_HEX = (python - <<'PY'
import secrets; print(secrets.token_hex(32))
PY
).Trim()

# 旋转并创建初始密钥版本（token/salt_master/beaver_seed）
python - <<'PY'
from eduppsa.kms import KMS
print("token kid      :", KMS.rotate("token"))
print("salt_master kid:", KMS.rotate("salt_master"))
print("beaver_seed kid:", KMS.rotate("beaver_seed"))
PY


> 如果 `data/students.csv` 不存在，程序会自动生成随机样本数据。

---

## 数据格式

`data/students.csv`（UTF-8）：

```csv
id,chinese,math,english,physics,chemistry,biology
stu-000,83,92,78,88,91,85
stu-001,76,84,90,71,73,80
...
```

分值默认范围：`0..100`（可在 `config.py` 配置）。

---

## 目录结构

```
eduppsa/
├─ utils.py            # 读/写CSV、自动生成demo数据
├─ audit.py            # 哈希链日志（记录每步摘要）
├─ subjects.py         # 科目枚举/校验
├─ secretsharing.py    # “v0/v1 平均/最高分”用的加法秘密分享
├─ sharemath.py        # mod Q 的加法共享与运算（两人比较用）
├─ beaver.py           # Beaver 乘法三元组（生成+共享乘法）
├─ histogram.py        # 一热直方图共享/重构/求最大值
├─ aggregator.py       # 编排/中继（只重构聚合，不碰单条）
├─ student_party.py    # 学生侧代理：生成求和/直方图的分享（演示里集中生成）
├─ compare.py          # ⭐ 两人安全比较的 MPC 实现（x>y/==）
├─ protocols.py        # 对外协议：SecureMean / SecureMax / SecureCompare
├─ crypto.py           # ECDH(X25519)+AES-GCM（可降级）
├─ he_threshold.py     # Paillier 同态演示（接口与门限版兼容）
├─ zk.py               # 范围/一致性证明的接口（可替换为真正 ZK）
└─ tokens.py           # 用途绑定令牌（HMAC），治理/配额/时效
```

---

## 功能清单

* **SecureMean（按科目均值）**：输出 `count, mean`；不泄露单条分数
* **SecureMax（按科目最高分）**：输出 `max, frequency`；不泄露单条分数
* **SecureCompare（两人比较）**：只返回 `>, <, ==`；不泄露 `x, y` 取值
* **审计日志**：哈希链 `tip` 作为可验证锚点
* **（可选）通道加密**：X25519 ECDH 密钥协商 + AES-GCM
* **（可选）用途令牌**：HMAC 令牌限制用途/有效期/调用次数
* **（可选）同态/零知识接口**：Paillier 演示 & ZK 接口占位，便于平滑升级

---

## 关键技术 ↔ 模块映射

| 模块                 | 核心技术                         | 作用                                                            |
| ------------------ | ---------------------------- | ------------------------------------------------------------- |
| `secretsharing.py` | **加法秘密分享**（Additive Sharing） | 将每个分数 `v` 拆成两份 `v=v₁+v₂ (mod Q)` 分发到 S1/S2；用于**均值**、**直方图计数** |
| `sharemath.py`     | **素数域算术共享**（mod Q）           | 定义模数 Q 与加/减/乘/重构；两人比较中的算术电路基础                                 |
| `beaver.py`        | **Beaver 乘法三元组**             | 在共享域进行安全乘法（半诚实安全）；两人比较需要 `x·prefix(y)`                        |
| `histogram.py`     | **一热编码 + 分享**                | 每个分数→one-hot→逐维分享→累加→重构直方图；最大非零索引即最高分                         |
| `compare.py`       | **一热前缀计数 + Beaver 乘法**       | 计算 `GT = Σ_j x[j]·(Σ_{k<j}y[k])`；对调再算得 `LT`，`EQ=1−GT−LT`      |
| `crypto.py`        | **X25519 ECDH + AES-GCM**    | 端到端通道封装；未安装库时自动降级为明文（教学演示）                                    |
| `he_threshold.py`  | **Paillier 加法同态（演示）**        | Enc/Dec/HomAdd 演示；可在同接口替换为**门限 Paillier（t-of-n）**             |
| `zk.py`            | **承诺 + 范围声明（接口占位）**          | 演示版非真 ZK；可替换为 Pedersen+Σ/OR 或 Bulletproofs                    |
| `tokens.py`        | **HMAC-SHA256 令牌**           | 用途绑定令牌（有效期/用途/计数）—治理层                                         |
| `audit.py`         | **哈希链**                      | 追加式日志，便于审计与对账                                                 |

---

## 协议与实现要点

### 1）按科目均值（SecureMean）

* 每个分数 `v` → 两份分享 `v₁, v₂ (mod Q)` → 分发 S1/S2
* S1/S2 各自列内求和 → 得到总和份额 `Σv₁, Σv₂`
* 编排相加重构 `Σv`，再除以人数 `n` 得 `mean`
* **泄露**：仅 `{n, mean}`

### 2）按科目最高分（SecureMax）

* 分数 `x` → 长度 L（默认101）的 **one-hot**
* 逐维加法分享→两服务器累加→编排重构直方图
* **最大值**：最后一个非零索引；同时输出其频次
* **泄露**：仅 `{max, frequency}`（也可仅重构这两项）

### 3）两人私密比较（SecureCompare）

* `X, Y` 各自 one-hot 并加法分享
* 在共享域计算
  [
  GT = \sum_{j} x[j]\cdot \left(\sum_{k<j} y[k]\right)
  ]
  用 **Beaver 三元组**完成乘法
* 对调计算得 `LT`，再 `EQ=1−GT−LT`
* **泄露**：仅 `>, <, ==` 之一

---

## 配置（`config.py`）

* `SUBJECTS`：科目列表（默认 6 科）
* `SCORE_MIN/MAX`：分值范围（决定 one-hot 长度）
* `N_SERVERS`：MPC 服务器数量（当前为 2）
* `BEAVER_POOL`：两人比较用三元组池大小
* `TOKEN_SECRET`/`TOKEN_TTL_SECONDS`：用途令牌配置
* `AUTO_GEN`：无数据时自动生成样本

---

## 运行示例（终端输出）

```
=== Edu-PPSA v1: Privacy-Preserving Student Analytics ===

-- Secure Mean by Subject --
chinese    count=50  mean=83.34
math       count=50  mean=84.06
...

-- Secure Max by Subject --
chinese    max= 98  (count=1)
math       max= 97  (count=2)
...

-- Secure Compare (Two Students, MPC v1) --
Compare(math): stu-012 vs stu-031  ->  winner=stu-031  relation=>

Audit tip: 3b5b1b1f...
```

---

## 隐私与安全

* **半诚实模型**：参与方按协议执行但可能窥视
* **S1/S2 不得合谋**：单台份额无意义；两台合谋可重构单条（需组织与技术上隔离）
* **最小披露**：仅公开聚合结果；可叠加 **k-匿名阈值** 与 **差分隐私**
* **通道安全**：X25519+AES-GCM（未装库降级为明文，仅教学）
* **可验证性**：哈希链日志；ZK 接口预留可接入真正零知识证明

---


