import config
def valid_subject(subj: str) -> bool:
    return subj in config.SUBJECTS
