fname1 = "data_02-example.txt"
fname2 = "data_02-matthias_baake.txt"

fname = fname2

with open(fname) as file:
    inp_l = list(
        map(
            lambda line: list(map(int, line.split())),
            file,
        )
    )

MIN_DIFF = 1
MAX_DIFF = 3


def check_pair(num, num_next):
    diff = num_next - num
    return (
        MIN_DIFF <= diff <= MAX_DIFF,  # Zahlen aufsteigend, Differenz zulässig
        -MAX_DIFF <= diff <= -MIN_DIFF,  # Zahlen absteigend, Differenz zulässig
    )


def check_safe(num_l):
    ascending_ok_t, descending_ok_t = zip(
        *map(
            check_pair,
            num_l[:-1],
            num_l[1:],
        )
    )
    return all(ascending_ok_t) or all(descending_ok_t)


# Teilaufgabe 1
num_safe = sum(map(check_safe, inp_l))
print(f"Ergebnis Teilaufgabe 1: {num_safe}")
# fname1: 2
# fname2: 220


# Teilaufgabe 2
def check_safe_tolerance(num_l):
    return check_safe(num_l) or any(
        map(
            check_safe,
            map(
                lambda remove_pos: num_l[:remove_pos] + num_l[remove_pos + 1 :],
                range(len(num_l)),
            ),
        )
    )


num_safe2 = sum(map(check_safe_tolerance, inp_l))
print(f"Ergebnis Teilaufgabe 2: {num_safe2}")
# fname1: 4
# fname2: 296
