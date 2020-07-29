#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846

// Writes the amount of coordinates of the generated points in the .names file.
void writeNamesFile(int dimension){
	FILE *fp = fopen("horizontal.names", "w");
	fprintf(fp, "0, 1.\n");
	
	int i;
	for(i = 0; i < dimension; i++)
		fprintf(fp, "coord%d: continuous.\n", i);
	
	fclose(fp);
}

// Evaluates a Gaussian function with center in 0 and the given standard
// deviation, in the given X value.
double evalGauss(double xValue, double stdDev){
	double coefficient = stdDev * sqrt(2 * M_PI);
	double exponent = -1 * pow(xValue, 2) / (2 * pow(stdDev, 2));
	return exp(exponent) / coefficient;
}

// Generates a random coordinate following the Gaussian distribution for
// the given center and standard deviation.
double randomGaussPoint(int center, double stdDev){
	double xValue;
	int found = 0;
	
	// This loop looks to find the X coordinate of a random point
	// in a Gaussian distribution with center in 0 and the given
	// standard deviation.
	while(!found){
		// First, get a random X coordinate 
		// in the [-5 * stdDev, 5 * stdDev] interval.
		double scale = (double) rand() / RAND_MAX;
		xValue = -5 * stdDev + scale * 10 * stdDev;
    
		// Then, get a random Y coordinate 
		// between the possible values.
		scale = (double) rand() / RAND_MAX;
		double yValue = scale / (stdDev * sqrt(2 * M_PI));
		
		// If the given point is below the Gaussian function,
		// then exit the loop.
		if(yValue <= evalGauss(xValue, stdDev))
			found++;
	}
	
	// Since the coordinate was generated considering the center to be 0,
	// then its value is added to the center of the given distribution.
	return xValue + center;
}

// Writes all points of a given class to the .data file.
void generatePoints(FILE *fp, int class, int center, int dimension, int amount, double stdDev){
	int i;
	for(i = 0; i < amount; i++){
		int j;
		// Generates each coordinate randomly, given the center and standard deviation.
		// The first coordinate is centered in the given value, the rest in 0.
		for(j = 0; j < dimension; j++)			
			if(j == 0)
				fprintf(fp, "%f, ", randomGaussPoint(center, stdDev));
			else
				fprintf(fp, "%f, ", randomGaussPoint(0, stdDev));
		
		fprintf(fp, "%d.\n", class);
	}
}

// Writes the generated points in the .data file.
void writeDataFile(int dimension, int pointAmount, double stdDev){
	FILE *fp = fopen("horizontal.data", "w");
		
	//If the given amount of points is odd, assign an extra one to class 0.
	generatePoints(fp, 1, 1, dimension, pointAmount/2, stdDev);
	generatePoints(fp, 0, -1, dimension, (pointAmount + 1)/2, stdDev);
	
	fclose(fp);
}

int main(int argc, char* argv[]){
	// If a seed is provided, use it. If not, set the clock time as seed.
	if(argc == 4)
		srand(time(NULL));
	else if(argc == 5){
		int seed = atoi(argv[4]);
		srand(seed);
	}
	else{
		printf("Argument error. Usage: executable d n C [seed] \n");
		return 1;
	}
	
	int dimension = atoi(argv[1]);
	int pointAmount = atoi(argv[2]);
	double C = atof(argv[3]);
	
	writeNamesFile(dimension);
	writeDataFile(dimension, pointAmount, C);	

	return 0;
}
