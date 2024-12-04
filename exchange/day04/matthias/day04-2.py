from functools import partial
from itertools import starmap

fname1 = "data_04-example-2.txt"
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


def find_2words_at_xy_in_dir(word, matrix_l, data0, data1):
    x0, y0, dx0, dy0 = data0
    x1, y1, dx1, dy1 = data1
    return find_word_at_xy(word, matrix_l, x0, y0, dx0, dy0) and find_word_at_xy(
        word, matrix_l, x1, y1, dx1, dy1
    )


def find_2words_at_xy(word, matrix_l, x, y):
    delta = int(len(word) / 2)
    return sum(
        starmap(
            partial(find_2words_at_xy_in_dir, word, matrix_l),
            (
                ((x - delta, y - delta, 1, 1), (x - delta, y + delta, 1, -1)),
                ((x - delta, y - delta, 1, 1), (x + delta, y - delta, -1, 1)),
                ((x + delta, y + delta, -1, -1), (x - delta, y + delta, 1, -1)),
                ((x + delta, y + delta, -1, -1), (x + delta, y - delta, -1, 1)),
            ),
        )
    )


def find_2words_at_y(word, matrix_l, y):
    return sum(
        map(
            partial(find_2words_at_xy, word, matrix_l, y=y),
            range(len(matrix_l[0])),
        )
    )


def find_2words(word, matrix_l):
    return sum(
        map(
            partial(find_2words_at_y, word, matrix_l),
            range(len(matrix_l)),
        )
    )


WORD = "MAS"
num = find_2words(WORD, matrix_l)

print(f"Ergebnis Teilaufgabe 2 für {fname}: {num}")
# fname1: 9
# fname2: 1835
