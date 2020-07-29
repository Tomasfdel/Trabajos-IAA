def numberStrip(line):
	return float(line.split(":")[1].rstrip().rstrip("%"))


def main():
	folders = ["horizontal", "diagonal"]
	values = ["2", "4", "8", "16", "32"]

	for folder in folders:
		trainFile = open(folder + "_TrainingErrors", 'w')
		testFile = open(folder + "_TestErrors", 'w')
		print("-- Folder: " + folder)

		for value in values:
			fp = open(folder + "/" + value, 'r')
			print("Valor: " + value)
			train = []
			test = []

			for number, line in enumerate(fp.readlines()):
				if number % 19 == 14:
					# ~ print("TRAIN: " + str(numberStrip(line)))
					train += [numberStrip(line)]
				if number % 19 == 16:
					# ~ print("TEST: " + str(numberStrip(line)))
					test += [numberStrip(line)]
			
			print(train)
			print(test)
			print("\n\n")
			trainFile.write(value + " " + str(sum(train) / 20) + "\n")
			testFile.write(value + " " + str(sum(test) / 20) + "\n")
			
			fp.close()
			
		trainFile.close()
		testFile.close()
	
		
if __name__ == "__main__":
	main()
