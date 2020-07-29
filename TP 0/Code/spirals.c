#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846

// Writes the amount of coordinates of the generated points in the .names file.
void writeNamesFile(){
	FILE *fp = fopen("spirals.names", "w");
	fprintf(fp, "0, 1.\n");
	
	fprintf(fp, "coord0: continuous.\n");
	fprintf(fp, "coord1: continuous.\n");
	
	fclose(fp);
}

// Checks if a given point is between the curves of the given spiral.
int betweenCurves(int class, double xCoord, double yCoord){
	// Calculate the polar coordinates for the point.
	double distance = sqrt(pow(xCoord, 2) + pow(yCoord, 2));
	double angle = atan2(yCoord, xCoord);
	
	// If the given point is outside the circle, then it will not
	// be inside any of the spiral areas.
	if(distance > 1) return 0;
	
	// Then, calculate the distance to the center of a point in one of
	// the spiral curves that is in the same angle as the given point.
	double baseCurve = angle / (4 * M_PI);
	
	// Check if the given point is in the area for the spiral 
	// that belongs to class 0. If it is, then return True
	// if it belongs to that class, False otherwise.
	if((distance >= baseCurve && distance <= baseCurve + 0.25) ||
	   (distance >= baseCurve + 0.5 && distance <= baseCurve + 0.75) ||
	    distance >= baseCurve + 1)
		return !class;
	
	// If it does not belong to that spiral, return the negation of
	// the previous possible result.
	return class;
}

// Generates a random point that belongs to the given spiral.
void randomSpiralPoint(int class, double *xCoord, double *yCoord){
	int found = 0;
	
	while(!found){
		// Generates a pair of (x,y) values, each of them
		// in the [-1, 1] interval.
		double scale = (double) rand() / RAND_MAX;
		*xCoord = scale * 2 - 1;
    
		scale = (double) rand() / RAND_MAX;
		*yCoord = scale * 2 - 1;
		
		// If the current point belongs to the given spiral, exit the loop.
		found = betweenCurves(class, *xCoord, *yCoord);
	}
	
}

// Writes all points of a given class to the .data file.
void generatePoints(FILE *fp, int class, int amount){
	int i;
	double xCoord, yCoord;
	for(i = 0; i < amount; i++){
		// Generates a random point that belongs to the corresponding
		// spiral and writes its values to the file.
		randomSpiralPoint(class, &xCoord, &yCoord);
		fprintf(fp, "%f, %f, %d.\n", xCoord, yCoord, class);
	}
}

// Writes the generated points in the .data file.
void writeDataFile(int pointAmount){
	FILE *fp = fopen("spirals.data", "w");
		
	//If the given amount of points is odd, assign an extra one to class 0.
	generatePoints(fp, 1, pointAmount/2);
	generatePoints(fp, 0, (pointAmount + 1)/2);
	
	fclose(fp);
}

int main(int argc, char *argv[]){
	// If a seed is provided, use it. If not, set the clock time as seed.
	if(argc == 2)
		srand(time(NULL));
	else if (argc == 3){
		int seed = atoi(argv[2]);
		srand(seed);
	}
	else{
		printf("Argument error. Usage: executable n [seed] \n");
		return 1;
	}
	
	int pointAmount = atoi(argv[1]);
	
	writeNamesFile();
	writeDataFile(pointAmount);
	
	return 0;
}
