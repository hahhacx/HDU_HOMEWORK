import csv, os, random
from typing import Dict, List
import config

def ensure_demo_students(path: str, n: int = 50, seed: int = 42):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    random.seed(seed)
    with open(path, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["id"] + config.SUBJECTS)
        w.writeheader()
        for i in range(n):
            row = {"id": f"stu-{i:03d}"}
            for s in config.SUBJECTS:
                row[s] = random.randint(config.SCORE_MIN, config.SCORE_MAX)
            w.writerow(row)

def read_students(path: str) -> List[Dict]:
    rows = []
    with open(path, newline="", encoding="utf-8") as f:
        rd = csv.DictReader(f)
        for r in rd:
            rows.append(r)
    return rows

def to_int(x: float) -> int:
    return int(round(x * config.SCALE))

def to_float(x: int) -> float:
    return x / config.SCALE
