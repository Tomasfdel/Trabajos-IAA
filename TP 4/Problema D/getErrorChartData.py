def main():
	problems = ["Horizontal", "Diagonal"]
	folders = ["2", "4", "8", "16", "32"]
	kValues = {"Horizontal": ["1", "5", "10", "20", "40"], "Diagonal": ["1", "10", "100", "300", "1000"]}

	for problem in problems:
		trainFile = open(problem + "/TrainingErrors", 'w')
		validFile = open(problem + "/ValidationErrors", 'w')
		testFile = open(problem + "/TestErrors", 'w')
			
		for index, folder in enumerate(folders):
			kTrainFile = open(problem + "/" + folder + "/TrainingErrors", 'r')
			kValidFile = open(problem + "/" + folder + "/ValidationErrors", 'r')
			kTestFile = open(problem + "/" + folder + "/TestErrors", 'r')

			kValue = kValues[problem][index]

			for line in kTrainFile.readlines():
				lineValues = line.strip().split()
				if kValue == lineValues[0].strip():
					trainFile.write(folder + " " + lineValues[1].strip() + "\n")
				
			for line in kValidFile.readlines():
				lineValues = line.strip().split()
				if kValue == lineValues[0].strip():
					validFile.write(folder + " " + lineValues[1].strip() + "\n")
				
			for line in kTestFile.readlines():
				lineValues = line.strip().split()
				if kValue == lineValues[0].strip():
					testFile.write(folder + " " + lineValues[1].strip() + "\n")
				
			kTrainFile.close()
			kValidFile.close()
			kTestFile.close()
				
				
		trainFile.close()
		validFile.close()
		testFile.close()				
	
		
if __name__ == "__main__":
	main()
