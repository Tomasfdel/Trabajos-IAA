import sys
import statistics

def c45lineStrip(algorithmName):
	results = []
	dataFile = open(resultFolder + "/" + algorithmNames[algorithmName], "r")

	resultLineCounter = 0
	for line in dataFile.readlines():
		if(line.strip().endswith("<<")):
			resultLineCounter += 1
			if(resultLineCounter % 2 == 0):
				results += [float(line.split("%")[1].split("(")[1])]

	dataFile.close()
	return results


def naiveBayesLineStrip(algorithmName):
	results = []
	dataFile = open(resultFolder + "/" + algorithmNames[algorithmName], "r")

	for number, line in enumerate(dataFile.readlines()):
		if number % 19 == 16:
			results += [float(line.split(":")[1].rstrip().rstrip("%"))]

	dataFile.close()
	return results


def svmLineStrip(algorithmName):
	results = []
	dataFile = open(resultFolder + "/" + algorithmNames[algorithmName] + "/Results", "r")

	for line in dataFile.readlines():
		results += [100 * float(line.strip())]
	
	dataFile.close()
	return results	


dataFolder = "breast_cancer"
resultFolder = dataFolder + "/" + "Execution Results"
algorithmNames = {"c4.5": "C4.5", "naive_bayes": "Naive Bayes", "linear_svm": "Linear SVM", "linear_svm_simple": "Linear SVM (Simple)", "poly_svm": "Poly SVM", "poly_svm_simple": "Poly SVM (Simple)"}
lineStrip = {"c4.5": c45lineStrip, "naive_bayes": naiveBayesLineStrip, "linear_svm": svmLineStrip, "linear_svm_simple": svmLineStrip, "poly_svm": svmLineStrip, "poly_svm_simple": svmLineStrip}

def main():
	if(len(sys.argv) != 3):
		print("ERROR: Cantidad incorrecta de argumentos.\nModo de uso: programa algoritmo1 alrogitmo2")
		return

	algorithm1, algorithm2 = sys.argv[1], sys.argv[2]

	if(algorithm1 not in algorithmNames or algorithm2 not in algorithmNames):
		print("ERROR: Nombre de algoritmo incorrecto.")
		return

	results1 = lineStrip[algorithm1](algorithm1)
	results2 = lineStrip[algorithm2](algorithm2)

	print(algorithmNames[algorithm1] + ": Promedio " + str(statistics.mean(results1)) + " - Desvio " + str(statistics.stdev(results1)))
	print(algorithmNames[algorithm2] + ": Promedio " + str(statistics.mean(results2)) + " - Desvio " + str(statistics.stdev(results2)))

	average = (sum(results1) - sum(results2)) / len(results1)
	
	deviation = 0
	for index in range(len(results1)):
		deviation += (results1[index] - results2[index] - average) ** 2
	
	deviation = (deviation / (len(results1) - 1)) ** 0.5
	
	tValue = (average / deviation) * (len(results1) ** 0.5)
	
	print("t-value: " + str(tValue)) 

if __name__ == "__main__":
	main()
