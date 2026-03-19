from eduppsa.utils import ensure_demo_students, read_students
from eduppsa.protocols import (
    run_secure_mean_by_subject_secure,
    run_secure_max_by_subject_secure,
    run_secure_compare_two_v1_secure,
)
from eduppsa.audit import HashChainLog
from eduppsa.tokens import issue_token
import config, os, random

def main():
    os.makedirs("data", exist_ok=True)
    if not os.path.exists("data/students.csv") and config.AUTO_GEN:
        ensure_demo_students("data/students.csv", n=50, seed=config.RNG_SEED)

    log = HashChainLog()

    # 为本次会话签发三类用途令牌（mean / max / compare）
    mean_token = issue_token({"action": "mean", "scope": "all_subjects"})
    max_token  = issue_token({"action": "max",  "scope": "all_subjects"})
    # compare 的 scope 里放入要比较的科目
    cmp_token  = issue_token({"action": "compare", "scope": "math"})

    print("=== Edu-PPSA v1.1-secure ===")

    means = run_secure_mean_by_subject_secure(
        "data/students.csv", subjects=config.SUBJECTS, token=mean_token, log=log
    )
    print("\n-- Secure Mean by Subject (MPC + ECDH/AES + Token) --")
    for s, (cnt, mean) in means.items():
        print(f"{s:<10} count={cnt:<3d}  mean={mean:.2f}")

    maxima = run_secure_max_by_subject_secure(
        "data/students.csv", subjects=config.SUBJECTS, token=max_token, log=log
    )
    print("\n-- Secure Max by Subject (MPC + ECDH/AES + Token) --")
    for s, (best, freq) in maxima.items():
        print(f"{s:<10} max={best:>3d}  (count={freq})")

    rows = read_students("data/students.csv")
    ids = [r["id"] for r in rows]
    if len(ids) >= 2:
        id1, id2 = random.sample(ids, 2)
        res = run_secure_compare_two_v1_secure(
            "data/students.csv", subj="math", id1=id1, id2=id2, token=cmp_token, log=log
        )
        print("\n-- Secure Compare (Two Students, MPC + ECDH/AES + Token) --")
        if res is None:
            print("比较失败：找不到指定学生/科目")
        else:
            winner, relation = res
            print(f"Compare(math): {id1} vs {id2}  ->  winner={winner}  relation={relation}")
    else:
        print("\n样本太少，无法比较。")

    print("\nAudit tip:", log.tip())

if __name__ == "__main__":
    main()
