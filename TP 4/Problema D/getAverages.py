def numberStrip(line):
	return float(line.split(":")[1].rstrip().rstrip("%"))


def main():
	problems = ["Horizontal", "Diagonal"]
	folders = ["2", "4", "8", "16", "32"]
	values = ["0.0001", "0.0005", "0.001", "0.005", "0.01", "0.05", "0.1", "0.5", "1", "5", "10", "20", "40", "50", "100", "500", "1000", "10000"]
	executionNumber = 20

	for problem in problems:
		print("Problem: " + problem)
		for folder in folders:
			trainFile = open(problem + "/" + folder + "/TrainingErrors", 'w')
			validFile = open(problem + "/" + folder + "/ValidationErrors", 'w')
			testFile = open(problem + "/" + folder + "/TestErrors", 'w')
			print("-- Folder: " + folder)

			for value in values:
				fp = open(problem + "/" + folder + "/" + value, 'r')
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
