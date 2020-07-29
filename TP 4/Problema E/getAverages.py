def numberStrip(line):
	return float(line.split(":")[1].rstrip().rstrip("%"))


def main():
	folder = "Results"
	values = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "12", "14", "16", "18", "20", "23", "26", "29", "32", "35", "39", "43", "47", "50"]
	executionNumber = 20

	trainFile = open(folder + "/TrainingErrors", 'w')
	validFile = open(folder + "/ValidationErrors", 'w')
	testFile = open(folder + "/TestErrors", 'w')

	for value in values:
		fp = open(folder + "/" + value + "/ExecutionLogs", 'r')
		print("Valor: " + value)
		train = []
		valid = []
		test = []

		for number, line in enumerate(fp.readlines()):
			if number % 19 == 14:
				train += [numberStrip(line)]
			if number % 19 == 15:
				valid += [numberStrip(line)]
			if number % 19 == 16:
				test += [numberStrip(line)]
		
		print(train)
		print(valid)
		print(test)
		print("\n\n")
		trainFile.write(value + " " + str(sum(train) / executionNumber) + "\n")
		validFile.write(value + " " + str(sum(valid) / executionNumber) + "\n")
		testFile.write(value + " " + str(sum(test) / executionNumber) + "\n")
		
		fp.close()
		
	trainFile.close()
	validFile.close()
	testFile.close()
	
		
if __name__ == "__main__":
	main()
