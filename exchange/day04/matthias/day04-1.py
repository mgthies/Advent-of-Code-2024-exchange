from functools import partial
from itertools import starmap

fname1 = "data_04-example-1.txt"
fname2 = "data_04-matthias_baake.txt"

fname = fname2

with open("2024/" + fname) as file:
    matrix_l = list(map(lambda line: line.rstrip(), file.readlines()))


def check_char_at(matrix_l, x, y, dx, dy, pos, char):
    pos_x = x + dx * pos
    pos_y = y + dy * pos
    if not (0 <= pos_x < len(matrix_l[0]) and 0 <= pos_y < len(matrix_l)):
        return False
    return matrix_l[pos_y][pos_x] == char


def find_word_at_xy(word, matrix_l, x, y, dx, dy):
    return all(
        starmap(
            partial(check_char_at, matrix_l, x, y, dx, dy),
            enumerate(word),
        )
    )


def find_word_at_y(word, matrix_l, y, dx, dy):
    return sum(
        map(
            partial(find_word_at_xy, word, matrix_l, y=y, dx=dx, dy=dy),
            range(len(matrix_l[0])),
        )
    )


def find_word_dir(word, matrix_l, dxdy):
    return sum(
        map(
            partial(find_word_at_y, word, matrix_l, dx=dxdy[0], dy=dxdy[1]),
            range(len(matrix_l)),
        )
    )


WORD = "XMAS"
num = sum(
    map(
        partial(find_word_dir, WORD, matrix_l),
        (
            (-1, -1),
            (0, -1),
            (1, -1),
            (-1, 0),
            (1, 0),
            (-1, 1),
            (0, 1),
            (1, 1),
        ),
    )
)

print(f"Ergebnis Teilaufgabe 1 für {fname}: {num}")
# fname1: 18
# fname2: 2434
