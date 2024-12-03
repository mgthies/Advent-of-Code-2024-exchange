#
# see: https://adventofcode.com/2024/day/3
# 
 
from time import time
import re

__startTime = time()

def loadInput(isTest = True):
    global __startTime

    if isTest:
        filename = f"exchange/day{DAY}/claudia/input-example.txt"
    else:
        filename = f"exchange/day{DAY}/claudia/input.txt"

    with open(filename) as f:
        content = [l[:-1] if l[-1] == "\n" else l for l in f.readlines()]

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
# Day 03
#########################################################################################
DAY = "03"

def getAllTasks(lstStrings, inputData):
    strRe= "|".join(lstStrings)
    return re.findall(rf"{strRe}",inputData)

def getToDoTasks(inputString, do):
    toDoParts = []
    for elem in inputString:
        if do:
            if elem == "don't()":
                do = False
            elif elem != "do()":
                toDoParts.append(elem)
        else:
            if elem == "do()":
                do = True
    return (toDoParts, do)

def doTask(task):
    factors = list(map(int,getAllTasks(["[0-9]*,[0-9]*"], task)[0].split(",")))
    return factors[0] * factors[1]


def doPart1(isTest = True):
    data = loadInput(isTest)
    dimData=len(data)
    result = 0

    if isTest:
        # Different examples for test mentioned for part1 and part 2 
        # The the input-file contains both examples are => line 1 for part 1 and line 2 for part 2
        # => use only one line of input-file
        data.pop()
        dimData = len(data)
        print(data)
        print()
    
    for i in range(dimData):
        lstToDo=getAllTasks(["mul\([0-9]*,[0-9]*\)"], data[i])
        
        for j in range(len(lstToDo)):
            result += doTask(lstToDo[j])

    if not isTest:
        writeSolutionFile(1, result)

    return result

             
def doPart2(isTest = True):
    data = loadInput(isTest)
    dimData = len(data)
    result = 0
    do = True
        
    if isTest:
        data.pop(0)
        dimData = len(data)
        print(data)
        print()

    for i in range(dimData):
        lstToDo=getAllTasks(["don't\(\)", "do\(\)", "mul\([0-9]*,[0-9]*\)"], data[i])
        lstToDo, do = getToDoTasks(lstToDo, do)
        
        for j in range(len(lstToDo)):
            f1,f2=lstToDo[j].split(",")
            result += (int(f1.split("(")[1]) * int(f2.split(")")[0]))
        
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
