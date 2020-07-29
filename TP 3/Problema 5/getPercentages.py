def numberStrip(line):
	return float(line.split(":")[1].rstrip().rstrip("%"))


def main():
	problem = "spirals"

	trainFile = open(problem + "_TrainingErrors", 'w')
	validFile = open(problem + "_ValidationErrors", 'w')
	testFile = open(problem + "_TestErrors", 'w')

	fp = open("Results/ExecutionLogs", 'r')
	
	for number, line in enumerate(fp.readlines()):
		if number % 20 == 0:
			i = line.strip().strip('-').strip()
		if number % 20 == 15:
			# ~ print("TRAIN: " + str(numberStrip(line)))
			trainFile.write(i + " " + str(numberStrip(line)) + "\n")
		if number % 20 == 16:
			# ~ print("TEST: " + str(numberStrip(line)))
			validFile.write(i + " " + str(numberStrip(line)) + "\n")
		if number % 20 == 17:
			# ~ print("TEST: " + str(numberStrip(line)))
			testFile.write(i + " " + str(numberStrip(line)) + "\n")
			
	fp.close()
		
	trainFile.close()
	validFile.close()
	testFile.close()
	
		
if __name__ == "__main__":
	main()
