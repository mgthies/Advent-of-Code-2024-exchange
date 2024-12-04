#!/usr/bin/env python3

with open('day4-s.txt') as f:
    input = {(x, y): char for y, line in enumerate(f) for x, char in enumerate(line.strip())}


def find_word(matrix, word):
    directions = [
        (-1, 0), (1, 0), (0, -1), (0, 1),
        (-1, -1), (-1, 1), (1, -1), (1, 1)
    ]
    for x, y in matrix:
        for dx, dy in directions:
            if all(matrix.get((x + dx * i, y + dy * i), '') == char for i, char in enumerate(word)):
                yield x, y

def find_cross(matrix):
    for x, y in matrix:
        if matrix.get((x, y), '') != 'A':
            continue
        pair1 = matrix.get((x - 1, y - 1), '') + matrix.get((x + 1, y + 1), '')
        pair2 = matrix.get((x - 1, y + 1), '') + matrix.get((x + 1, y - 1), '')
        if {pair1, pair2} <= {'MS', 'SM'}:
            yield x, y
            
print('Part 1:', len(list(find_word(input, 'XMAS'))))
print('Part 2:', len(list(find_cross(input))))