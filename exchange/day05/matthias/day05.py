from functools import cmp_to_key

fname1 = "data_05-example.txt"
fname2 = "data_05-matthias_baake.txt"

fname = fname2
dirname = "2024"


def read_file(dirname, fname):
    rule_s = set()
    update_l = []
    in_rule = True

    with open(dirname + "/" + fname) as file:
        for line in file:
            if not line.strip():
                in_rule = False
                continue
            if in_rule:
                rule_s.add(tuple(map(int, line.split("|"))))
            else:
                update_l.append(tuple(map(int, line.split(","))))
    return rule_s, update_l


def check_update(update_t, rule_s):
    for pos, num1 in enumerate(update_t):
        for num2 in update_t[pos + 1 :]:
            if (num2, num1) in rule_s:
                return False
    return True


rule_s, update_l = read_file(dirname, fname)

# Teilaufgabe 1
sum_middle = 0
update_incorrect_l = []
for update in update_l:
    if check_update(update, rule_s):
        middle = update[int(len(update) / 2)]
        sum_middle += middle
    else:
        update_incorrect_l.append(update)

print(f"Ergebnis Teilaufgabe 1 für {fname}: {sum_middle}")
# fname1: 143
# fname2: 4578

# Teilaufgabe 2

# Lösung: Sortierung anhand der Regeln vornehmen, dazu Vergleichsfunktion
# verwenden (wie in python 2)

# Annahme: Jede für die Vergleiche bei der Sortierung benötigte Angabe (x|y)
# ist in der Eingabe vorhanden. Dies ist der Fall, sonst würde es zu einem
# Abbruch kommen. Andernfalls müsste man die transitive Hülle über die x|y-
# Angaben bilden.

# Dictionary mit Key = zwei Werte, Value = -1, 0 oder 1, je nach
# vorgeschriebener Reihenfolge
rule_d = {}
for num1, num2 in rule_s:
    rule_d[(num1, num1)] = 0
    rule_d[(num2, num2)] = 0
    rule_d[(num1, num2)] = -1
    rule_d[(num2, num1)] = 1


# Sortierung mit Verwendung der Vergleichsfunktion cmp_rule über die
# Hilfsfunktion cmp_to_key
def sort_update(update_t):
    return sorted(update_t, key=cmp_to_key(lambda a, b: rule_d[(a, b)]))


sum_new_middle = 0
for update in update_incorrect_l:
    new_update = sort_update(update)
    sum_new_middle += new_update[int(len(new_update) / 2)]

print(f"Ergebnis Teilaufgabe 2 für {fname}: {sum_new_middle}")
# fname1: 123
# fname2: 6179
