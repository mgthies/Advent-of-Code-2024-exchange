import re
from functools import reduce

pat_mul = re.compile(
    r"""
    (?:mul\(([0-9]+),([0-9]+)\)) | # group(1), group(2)
    (do\(\)) | # group(3)
    (don't\(\)) # group(4)
    """,
    flags=re.VERBOSE,
)

fname1 = "data_03-example-2.txt"
fname2 = "data_03-matthias_baake.txt"

fname = fname2

with open(fname) as file:
    text = file.read()


def handle_match(state, match):
    active, total = state

    if match.group(3):  # do()
        return True, total

    if match.group(4):  # don't()
        return False, total

    # mul(...,...)
    if active:
        return active, total + int(match.group(1)) * int(match.group(2))
    else:
        return active, total


_, result = reduce(
    handle_match,
    pat_mul.finditer(text),
    (True, 0),
)

print(f"Ergebnis für Teilaufgabe 2 bei {fname}: {result}")
# fname1: 48
# fname2: 83158140
