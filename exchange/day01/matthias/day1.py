from collections import Counter

fname1 = "data_01-example.txt"
fname2 = "data_01-matthias_baake.txt"

fname = fname2

# Datei einlesen, in left_t ein Tupel mit den Zahlen aus der linken und
# in right_t ein Tupel mit den Zahlen aus der rechten Spalte ablegen
with open(fname) as file:
    left_t, right_t = zip(
        *map(
            lambda line: map(int, line.split()),
            file,
        )
    )

#
# Teilaufgabe 1
#
result = sum(
    map(
        lambda val_left, val_right: abs(val_left - val_right),
        sorted(left_t),
        sorted(right_t),
    )
)

print(f"Ergebnis von Teilaufgabe 1 für Datei {fname}: {result}")
# fname1: 11
# fname2: 1580061

#
# Teilaufgabe 2
#
right_cnt = Counter(right_t)

similarity = sum(
    map(
        lambda val: val * right_cnt[val],
        left_t,
    )
)

print(f"Ergebnis von Teilaufgabe 2 für Datei {fname}: {similarity}")
# inp1: 31
# inp2: 23046913
