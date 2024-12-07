#
# see: https://adventofcode.com/2024/day/5
# 
 
from time import time

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
# Day 05
#########################################################################################
DAY = "05"


def checkUpdates(rules,pages):
    for i in range(len(pages)):
        affectedRules = [r for r in rules if pages[i] in r]
        pagesNotOK = [r for r in pages[i+1:] if [r,pages[i]] in affectedRules] + [l for l in pages[:i] if [pages[i],l] in affectedRules]
        if len(pagesNotOK) > 0:
            return False
    return True


def orderList(inList,actRules):
    while True:
        change = False

        for i in range(len(inList)-1):
            if [inList[i], inList[i+1]] in actRules: 
                # no change necessary - go on with the next element of the list
                next
            elif  [inList[i+1], inList[i]] in actRules: 
                # change necessary - after change of the elements start at the beginning of the list
                inList[i], inList[i+1] = inList[i+1], inList[i]
                change = True
                break
            
        if not change:
            # no more changes for the list
            return inList


def doAllParts(isTest = True):
    data = loadInput(isTest)

    resultP1, resultP2 = 0, 0

    if isTest:
        for i in range(len(data)):
            print(data[i])
        print()

    pageOrderingRules, updates = [], []

    for i in range(len(data)):
        if "|" in data[i]:
            pageOrderingRules.append([int(num) for num in data[i].split("|")])
        if "," in data[i]:
            updates.append([int(num) for num in data[i].split(",")])

    for pages in updates:
        if checkUpdates(pageOrderingRules, pages):   
            # Part 1
            resultP1 += pages[int((len(pages)-1)/2)]
        else:   
            # Part 2
            affectedRules = [r for r in pageOrderingRules if r[0] in pages and r[1] in pages]
            pages = orderList(pages, affectedRules)            
            
            resultP2 += pages[int((len(pages)-1)/2)]
        

    if not isTest:
        writeSolutionFile(1, resultP1)
        writeSolutionFile(2, resultP2)

    return resultP1, resultP2

    
#########################################################################################

result_test = doAllParts()
result = doAllParts(False)

print("--- PART 1 ---")
print(f"Solution Example: {result_test[0]}")
print(f"Solution Part 1:  {result[0]}")


print("\n==============\n")
print("--- PART 2 ---")
print(f"Solution Example: {result_test[1]}")
print(f"Solution Part 2:  {result[1]}")

#########################################################################################
print()
printTimeTaken()
