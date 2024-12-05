#!/usr/bin/env python3
import re

with open('day3.txt') as f:
    instructions = re.findall(r'(do|don\'t)\(\)|mul\((\d{1,3}),(\d{1,3})\)', f.read())

product1 = product2 = 0
multiply = True
for do_dont, a, b in instructions:
    if do_dont:
        multiply = do_dont == 'do'
    else:
        product = int(a) * int(b)
        product1 += product
        product2 += product if multiply else 0

print('Part 1:', product1)
print('Part 2:', product2)