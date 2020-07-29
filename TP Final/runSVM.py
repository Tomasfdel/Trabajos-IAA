import os
import random
import sys
from PyML import *


dataFolder = "heladas"
foldAmount = 10


def runLinearSVM(trainVandL, testVandL, C):
	trainValues, trainLabels = trainVandL
	testValues, testLabels = testVandL
	trainDataset = VectorDataSet(trainValues, L = trainLabels)
	testDataset = VectorDataSet(testValues, L = testLabels)
	
	s = SVM(C = C)
	s.train(trainDataset)
	results = s.test(testDataset) 
	return 1 - results.getSuccessRate()


def runPolySVM(trainVandL, testVandL, degree, C):
	trainValues, trainLabels = trainVandL
	testValues, testLabels = testVandL
	trainDataset = VectorDataSet(trainValues, L = trainLabels)
	testDataset = VectorDataSet(testValues, L = testLabels)
	trainDataset.attachKernel('poly', degree = degree)
	
	s = SVM(C = C)
	s.train(trainDataset)
	results = s.test(testDataset) 
	return 1 - results.getSuccessRate()


def readFolds(foldAmount, foldFolder):
	results = []
	for foldIndex in range(foldAmount):
		foldValues = []
		foldLabels = []
		fp = open(foldFolder + "/" + str(foldIndex), 'r')
		for line in fp.readlines():
			lineValues = line.strip().split(",")
			foldValues += [list(map(float, lineValues[:-1]))]
			foldLabels += [lineValues[-1]]
		results += [(foldValues, foldLabels)]
	return results


def specificFoldsData(foldsData, foldIndexes):
	values = []
	labels = []
	for index in foldIndexes:
		indexValues, indexLabels = foldsData[index]
		values += indexValues
		labels += indexLabels
	return values, labels
	

def linearMain():
	foldFolder = dataFolder + "/" + str(foldAmount) + " Folds"
	cList = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 23, 26, 29, 32, 35, 39, 43, 47, 50, 60, 70, 80, 90, 100]

	foldsData = readFolds(foldAmount, foldFolder)

	resultsFolder = dataFolder + "/Execution Results/Linear SVM"
	os.mkdir(resultsFolder)
	resultsFile = open(resultsFolder + "/" + "Results", 'w')
	configsFile = open(resultsFolder + "/" + "Configurations", 'w')

	for testFold in range(foldAmount):
		errorSums = [0 for _ in cList]
		for validFold in range(foldAmount):
			if validFold != testFold:
				trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {validFold, testFold})
				validData = specificFoldsData(foldsData, {validFold})
				
				for cIndex, C in enumerate(cList):
					errorSums[cIndex] += runLinearSVM(trainData, validData, C)
				
		minimumIndex = errorSums.index(min(errorSums))
		
		trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {testFold})
		testData = specificFoldsData(foldsData, {testFold})
		finalResult = runLinearSVM(trainData, testData, cList[minimumIndex])
		
		resultsFile.write(str(finalResult) + "\n")
		configsFile.write("C: " + str(cList[minimumIndex]) + "\n")
		
	resultsFile.close()
	configsFile.close()


def linearSimpleMain():
	foldFolder = dataFolder + "/" + str(foldAmount) + " Folds"
	cList = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 23, 26, 29, 32, 35, 39, 43, 47, 50, 60, 70, 80, 90, 100]

	foldsData = readFolds(foldAmount, foldFolder)

	resultsFolder = dataFolder + "/Execution Results/Linear SVM (Simple)"
	os.mkdir(resultsFolder)
	resultsFile = open(resultsFolder + "/" + "Results", 'w')
	configsFile = open(resultsFolder + "/" + "Configurations", 'w')
	
	validFold = random.randint(0, foldAmount - 1)

	errors = []
	trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {validFold})
	validData = specificFoldsData(foldsData, {validFold})		
	for C in cList:
		errors += [runLinearSVM(trainData, validData, C)]
	minimumIndex = errors.index(min(errors))
	
	for testFold in range(foldAmount):		
		trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {testFold})
		testData = specificFoldsData(foldsData, {testFold})
		finalResult = runLinearSVM(trainData, testData, cList[minimumIndex])
		resultsFile.write(str(finalResult) + "\n")
		
	configsFile.write("Test Fold: " + str(validFold) + "\n")	
	configsFile.write("C: " + str(cList[minimumIndex]) + "\n")
	
	resultsFile.close()
	configsFile.close()


def polyMain():	
	foldFolder = dataFolder + "/" + str(foldAmount) + " Folds"
	cList = [1, 3, 5, 7, 9, 11, 15, 19, 25, 35, 50]
	degreeList = [2, 3, 4, 6, 8]

	foldsData = readFolds(foldAmount, foldFolder)

	resultsFolder = dataFolder + "/Execution Results/Poly SVM"
	os.mkdir(resultsFolder)
	resultsFile = open(resultsFolder + "/" + "Results", 'w')
	configsFile = open(resultsFolder + "/" + "Configurations", 'w')

	for testFold in range(foldAmount):
		errorSums = [[0 for _ in cList] for _ in degreeList]
		for validFold in range(foldAmount):
			if validFold != testFold:
				trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {validFold, testFold})
				validData = specificFoldsData(foldsData, {validFold})
				
				for degIndex, degree in enumerate(degreeList):
					for cIndex, C in enumerate(cList):	
						errorSums[degIndex][cIndex] += runPolySVM(trainData, validData, degree, C)
				
		minDegIndex = 0
		minCIndex = 0
		for degIndex in range(len(degreeList)):
			for cIndex in range(len(cList)):
				if errorSums[degIndex][cIndex] < errorSums[minDegIndex][minCIndex]:
					minDegIndex = degIndex
					minCIndex = cIndex
		
		trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {testFold})
		testData = specificFoldsData(foldsData, {testFold})
		finalResult = runPolySVM(trainData, testData, degreeList[minDegIndex], cList[minCIndex])
		
		resultsFile.write(str(finalResult) + "\n")
		configsFile.write("Degree: " + str(degreeList[minDegIndex]) + "  -  C: " + str(cList[minCIndex]) + "\n")
		
	resultsFile.close()
	configsFile.close()
	
	
def polySimpleMain():
	foldFolder = dataFolder + "/" + str(foldAmount) + " Folds"
	cList = [1, 3, 5, 7, 9, 11, 15, 19, 25, 35, 50]
	degreeList = [2, 3, 4, 6, 8]

	foldsData = readFolds(foldAmount, foldFolder)

	resultsFolder = dataFolder + "/Execution Results/Poly SVM (Simple)"
	os.mkdir(resultsFolder)
	resultsFile = open(resultsFolder + "/" + "Results", 'w')
	configsFile = open(resultsFolder + "/" + "Configurations", 'w')
	
	validFold = random.randint(0, foldAmount - 1)
	
	errors = []
	trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {validFold})
	validData = specificFoldsData(foldsData, {validFold})
	for degree in degreeList:
		errors += [[]]
		for C in cList:
			errors[-1] += [runLinearSVM(trainData, validData, C)]
	
	minDegIndex = 0
	minCIndex = 0
	for degIndex in range(len(degreeList)):
		for cIndex in range(len(cList)):
			if errors[degIndex][cIndex] < errors[minDegIndex][minCIndex]:
				minDegIndex = degIndex
				minCIndex = cIndex
	
	for testFold in range(foldAmount):		
		trainData = specificFoldsData(foldsData, {foldIdx for foldIdx in range(foldAmount)} - {testFold})
		testData = specificFoldsData(foldsData, {testFold})
		finalResult = runPolySVM(trainData, testData, degreeList[minDegIndex], cList[minCIndex])
		resultsFile.write(str(finalResult) + "\n")
		
	configsFile.write("Test Fold: " + str(validFold) + "\n")	
	configsFile.write("Degree: " + str(degreeList[minDegIndex]) + "  -  C: " + str(cList[minCIndex]) + "\n")
	
	resultsFile.close()
	configsFile.close()


if __name__ == "__main__":
	possiblePrograms = {"linear": linearMain, "linear_simple": linearSimpleMain, "poly": polyMain, "poly_simple": polySimpleMain}
	program = sys.argv[1]
	
	if program not in possiblePrograms.keys():
		print("ERROR: Argumento invalido. Opciones: linear, linear_simple, poly, poly_simple")
	else:
		possiblePrograms[program]()
