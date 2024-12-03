#
# see: https://adventofcode.com/2024/day/2
# 
 
from time import time

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
# Day 02
#########################################################################################
DAY = "02"

def checkList(inputList, isTest=True, part=1):
    dimInput = len(inputList)
    incList = sorted(inputList)
    decList = sorted(inputList, reverse=True)
        
    isDec = [(inputList[j] == decList[j]) for j in range(dimInput)]
    isInc = [(inputList[j] == incList[j]) for j in range(dimInput)]
    
    if all(isDec) or all(isInc):
        steps = [abs(inputList[i-1]-inputList[i]) for i in range(1,dimInput)]
        
        if all(isDec):
            strSlope = "decreasing"
        else:
            strSlope = "increasing"

        if max(steps) <= 3 and min(steps) > 0:
            if isTest and part == 1:
                diffSteps=sorted(list(set(steps)))
                strSteps=""
                for i in range(len(diffSteps)):
                    if len(diffSteps) - i > 2 :
                        strSteps += f"{diffSteps[i]}, "
                    elif len(diffSteps) - i == 2:
                        strSteps += f"{diffSteps[i]} or "
                    else:
                        strSteps += f"{diffSteps[i]} "
                print(f"{inputList}: Safe because the levels are all {strSlope} by {strSteps}")
            return True
        else:
            if isTest and part == 1:
               idx = steps.index(max(steps))
               print(f"{inputList}: Unsafe because {inputList[idx]} {inputList[idx+1]} is an increase of {max(steps)}.")
            return False
        

def doPart1(isTest = True):
    data = loadInput(isTest)
    dimData=len(data)
    result = 0

    if isTest:
        for i in range(dimData):
            print(data[i])
        print()
    
    for i in range(dimData):
        if checkList(data[i], isTest, 1):
            result += 1
  
    if not isTest:
        writeSolutionFile(1, result)

    return result

             
def doPart2(isTest = True):
    data = loadInput(isTest)
    
    dimData = len(data)
    result = 0
    strLevel = {1: "first", 2: "second", 3: "third", 4: "fourth", 5: "fifth"}

    for i in range(dimData):
        if checkList(data[i], isTest, 2):
            if isTest:
                print(f"{data[i]}: Safe without removing any level.")
            result +=1
        else:
            level = -1
            for j in range(len(data[i])): 
                tmpList = data[i].copy()
                tmpList.pop(j)
                if checkList(tmpList, isTest, 2):
                    result +=1
                    level = j
                    break
            if isTest:
                if level == -1:
                    print(f"{data[i]}: Unsafe regardless of which level is removed.")
                else:
                    print(f"{data[i]}: Safe by removing {strLevel[j+1]} level, {data[i][j]}.")
        
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
