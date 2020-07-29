def numberStrip(line):
	return float(line.split(":")[1].rstrip().rstrip("%"))


def main():
	folders = ["c_0", "c_2"]
	values = ["1", "5", "9", "13", "17", "21", "35", "49", "75", "99"]
	executionNumber = 20

	for folder in folders:
		print("--Folder: " + folder)
		
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
				if number % 20 == 15:
					train += [numberStrip(line)]
				if number % 20 == 16:
					valid += [numberStrip(line)]
				if number % 20 == 17:
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
