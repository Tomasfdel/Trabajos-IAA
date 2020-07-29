import os
import random

def stripClass(line):
    return int(line.strip().split(",")[-1])

def main():
    dataFolder = "breast_cancer"
    dataFile = "breast_cancer.data"
    classAmount = 2
    foldAmount = 10
    
    valuesByClass = [[] for _ in range(classAmount)]
    valuesByFold = [[] for _ in range(foldAmount)]

    sourceFile = open(dataFolder + "/" + dataFile, "r")
    totalLength = 0
    for line in sourceFile.readlines():
        valuesByClass[stripClass(line)] += [line]
        totalLength += 1
    sourceFile.close()
    
    extraValues = []
    for classValues in valuesByClass:
        random.shuffle(classValues)
        classFoldSize = len(classValues) // foldAmount
        for foldIndex in range(foldAmount):
            valuesByFold[foldIndex] += classValues[foldIndex * classFoldSize : (foldIndex + 1) * classFoldSize]
        extraValues += classValues[classFoldSize * foldAmount : ]
        
    extraValuesChunks = [extraValues[index : index + foldAmount] for index in range(0, len(extraValues), foldAmount)]
    foldIndexes = [x for x in range(foldAmount)]
    for chunk in extraValuesChunks:
        random.shuffle(foldIndexes)
        for index, value in enumerate(chunk):
            valuesByFold[foldIndexes[index]] += [value]
        
    foldFolder = dataFolder + "/" + str(foldAmount) + " Folds"
    os.mkdir(foldFolder)
        
    for fold in range(foldAmount):
        foldFile = open(foldFolder + "/" + str(fold), "w")
        for value in valuesByFold[fold]:
            foldFile.write(value)
        foldFile.close()
    
        
if __name__ == "__main__":
    main()
