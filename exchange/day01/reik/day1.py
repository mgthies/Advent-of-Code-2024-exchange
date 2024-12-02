#!/usr/bin/env python3

with open('day1.txt') as f:
    loc1, loc2 = zip(*(map(int, line.split()) for line in f))

print('Part 1:', sum(abs(x - y) for x, y in zip(sorted(loc1), sorted(loc2))))
print('Part 2:', sum(loc * loc2.count(loc) for loc in loc1))