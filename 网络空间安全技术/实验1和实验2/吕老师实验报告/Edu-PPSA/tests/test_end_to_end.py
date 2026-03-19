import os
from eduppsa.utils import ensure_demo_students, read_students
from eduppsa.protocols import run_secure_mean_by_subject, run_secure_max_by_subject
import config

def test_end_to_end():
    os.makedirs("data", exist_ok=True)
    ensure_demo_students("data/students.csv", n=20, seed=1)
    means = run_secure_mean_by_subject("data/students.csv", config.SUBJECTS)
    maxima = run_secure_max_by_subject("data/students.csv", config.SUBJECTS)

    rows = read_students("data/students.csv")
    for subj in config.SUBJECTS:
        vals = [int(r[subj]) for r in rows]
        cnt = len(vals)
        mean = sum(vals)/cnt
        mmax = max(vals)
        # 容差=0（整数），平均值允许很小的浮点误差
        assert abs(means[subj][0] - cnt) == 0
        assert abs(means[subj][1] - mean) < 1e-9
        assert maxima[subj][0] == mmax
