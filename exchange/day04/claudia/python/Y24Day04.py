#
# see: https://adventofcode.com/2024/day/4
# 
 
from time import time
#import re

__startTime = time()

def loadInput(isTest = True, part=""):
    global __startTime

    if isTest:
        filename = f"exchange/day{DAY}/claudia/input-example{part}.txt"
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
# Day 04
#########################################################################################
DAY = "04"


def doPart1(isTest = True):
    data = loadInput(isTest,"P1")
    dimY=len(data)
    dimX=len(data[0])
    result = 0

    if isTest:
        for i in range(dimY):
            print(data[i])
        print()

    directions = [(x,y) for x in range(-1,2) for y in range(-1,2) if (x,y) != (0,0)]
    
    for i in range(dimY):
        tmpStr = []
        for j in range(len(data[i])):
            tmpStr.append(data[i][j])
        data[i] = tmpStr

    for i in range(dimY):
        for j in range(len(data[i])):
            word=data[i][j]
            if word == "X":
                for d in directions:
                    if 0 <= i+3*d[0] < dimX and 0 <= j+3*d[1] < dimY:
                        word=data[i][j]+data[i+d[0]][j+d[1]]+data[i+2*d[0]][j+2*d[1]]+data[i+3*d[0]][j+3*d[1]]
                        if word == "XMAS":
                            result += 1

    
    if not isTest:
        writeSolutionFile(1, result)

    return result

             
def doPart2(isTest = True):
    data = loadInput(isTest,"P2")
    dimY = len(data)
    dimX = len(data[0])
    result = 0
        
    if isTest:
        for i in range(dimY):
            print(data[i])
        print()

    d = [1,-1] 
    # d=1 for (1,1) and (-1,-1)
    # d=-1 for (1,-1) and (-1,1)
    for i in range(dimX):
        for j in range(dimY):
            if 0 < i < dimX-1 and 0 < j < dimY-1 and data[i][j] == "A":
                if data[i+d[0]][j+d[1]]+data[i-d[0]][j-d[1]] in ["MS", "SM"] and data[i+d[0]][j-d[1]]+data[i-d[0]][j+d[1]] in ["MS", "SM"]:
                    result+=1
  
        
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
