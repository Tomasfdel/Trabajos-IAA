def main():
	inFiles = [("faces",".data"), ("faces",".test")]
	
	for folder, extension in inFiles:
		inFile = open(folder + "/" + "faces" + extension, 'r')
		outFile = open(folder + "/" + "faces_norm" + extension, 'w')
		
		for line in inFile.readlines():
			values = line.strip().split(',')
			for pixel in values[:-1]:
				outFile.write("{:.10f}".format(float(pixel)/255) + ",")
			
			#The last number is the points' class, not a pixel	
			outFile.write(values[-1] + "\n")
		
		inFile.close()
		outFile.close()	
	
	
	
if __name__ == "__main__":
	main()
