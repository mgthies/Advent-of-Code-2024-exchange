#!/usr/bin/env python3

with open('day2.txt') as f:
    reports = [list(map(int, line.split())) for line in f]


def is_safe(report):
    for i in range(len(report) - 1):
        if abs(report[i] - report[i + 1]) > 3 or report[i] == report[i + 1]:
            return False
    return sorted(report) == report or sorted(report, reverse=True) == report

safe = 0
tolerated = 0
for r in reports:
    if is_safe(r):
        safe += 1
    elif any(is_safe(r[:i] + r[i+1:]) for i in range(len(r))):
        tolerated += 1

print('Part 1:', safe)
print('Part 2:', safe+tolerated)