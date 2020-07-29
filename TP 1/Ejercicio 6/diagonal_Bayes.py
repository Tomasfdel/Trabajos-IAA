def main():
	inFile = open("diagonal.data", "r")	
	
	total = 0
	correctos = 0
	
	for linea in inFile.readlines():
		total += 1

		valores=linea.strip().split(',')
		suma = 0
		for valor in valores[:-1]:
			suma += float(valor.strip())
		
		clase = int(valores[-1].strip()[0])
	
		if (suma <= 0 and clase == 0) or (suma > 0  and clase == 1):
			correctos += 1
	
	print("%.3f" % ((1 - correctos/total) * 100))
	inFile.close()
	
	
if __name__ == "__main__":
	main()
