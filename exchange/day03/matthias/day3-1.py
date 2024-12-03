import re

pat_mul = re.compile(r"mul\(([0-9]+),([0-9]+)\)")

fname1 = "data_03-example-1.txt"
fname2 = "data_03-matthias_baake.txt"

fname = fname2

with open(fname) as file:
    text = file.read()

result = sum(
    map(
        lambda m: int(m.group(1)) * int(m.group(2)),
        pat_mul.finditer(text),
    )
)

print(f"Ergebnis für Teilaufgabe 1 bei {fname}: {result}")
# fname1: 161
# fname2: 173785482
