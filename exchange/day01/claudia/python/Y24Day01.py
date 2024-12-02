#
# see: https://adventofcode.com/2024/day/1
# 
 
from time import time
from collections import Counter

__startTime = time()

def loadInput(isTest = True):
    global __startTime

    if isTest:
        filename = f"exchange/day{DAY}/claudia/input-example.txt"
    else:
        filename = f"exchange/day{DAY}/claudia/input.txt"

    with open(filename) as f:
        content = [l[:-1] if l[-1] == "\n" else l for l in f.readlines()]

    for i in range(len(content)):
        content[i] = [int(i) for i in content[i].split()]
 
    return content


def writeSolutionFile(part, solution):
    filename = f"exchange/day{DAY}/claudia/solution-for-input.txt"
    parameter = "w" if part==1 else "a"

    with open(filename, parameter) as f:
        f.write(f"Part {part}: {solution}\n")


def printTimeTaken():
    global __startTime
    __endTime = time()
    print("Time: {:.3f}s".format(__endTime-__startTime))

print()

#########################################################################################
# Day 01
#########################################################################################
DAY="01"

def doPart1(isTest = True):
    data = loadInput(isTest)
    cntData = len(data)
    result = 0

    if isTest:
        for i in range(len(data)):
            print(data[i])
        print()

    left = sorted([data[i][0] for i in range(cntData)])
    right = sorted([data[i][1] for i in range(cntData)])
    if isTest:
        print(f"left: {left}")
        print(f"right: {right}\n")
        
    result = sum([abs(left[i]-right[i]) for i in range(cntData)])
    
    if not isTest:
        writeSolutionFile(1, result)

    return result

def doPart2(isTest = True):
    data = loadInput(isTest)
    cntData = len(data)
    result = 0

    left = sorted([data[i][0] for i in range(cntData)])
    right = Counter([data[i][1] for i in range(cntData)])
        
    if isTest:
        print(f"right: {right}\n")

    result = sum([elem * right[elem] for elem in left if elem in right])
        
    if not isTest:
        writeSolutionFile(2, result)

    return result

#########################################################################################

print("--- PART 1 ---")
print(f"Solution Example: {doPart1()}")
print(f"Solution Part 1:  {doPart1(False)}")


print("\n==============\n")
print("--- PART 2 ---")
print(f"Solution Example: {doPart2()}")
print(f"Solution Part 2:  {doPart2(False)}")

#########################################################################################
print()
printTimeTaken()
