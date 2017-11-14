
// Author Name : Kadir Emre Oto
// Student ID  : 150140032
// E-mail	   : otok@itu.edu.tr

#include <stdio.h>											// for standart input/output functions
#include <string.h>											// for string functions
#include <stdlib.h>											// for dynamic allocation functions

int threshold;												// declared at global scope to use in compare function
typedef struct person person;								// for clear coding

struct person{												// to store person data
	int personId;
	double height;
	double weight;
	double BMI;
};

double absolute(double number){
	if (number < 0) return -number;							// if number is negative, return -number
	return number;
}

int compare(const void *l, const void *r){					// to sort by dinstance between threshold and BMI value
	int a = absolute(threshold - ((*(person*)l).BMI));		// distance between 1st person's BMI and threshold
	int b = absolute(threshold - ((*(person*)r).BMI));		// distance between 2sd person's BMI and threshold
	if ( a < b ) return  1;
	if ( a > b ) return -1;
	return 0;
}

double BMI(double weight,double height) {					// to calculate BMI value of person
	return weight/(height*height);
}

int main(int argc, char const *argv[]) {
	int i;													// for iterations
	int N = atoi(argv[1]);									// read N from command line and convert it to integer
	int M = atoi(argv[2]);									// read M from command line and convert it to integer
	threshold = atoi(argv[3]);								// read threshold from command line and convert it to integer

	FILE* input;											// file stream for input
	FILE* output;											// file stream for output

	person *array = malloc(sizeof(person)* N);				// create array of person
	char* buffer = (char *)malloc(sizeof(char) * 256);		// to get first line of input file

	input = fopen("height_weight.txt", "r");				// open input file with reading mode
	fscanf(input,"%[^\n]s", buffer);						// get the first line (titles)

	for (i = 0; i < N; i++) {								// read N following lines and store in person struct
		fscanf(input,"%d %lf %lf",&(array[i].personId),&(array[i].height),&(array[i].weight));
		array[i].BMI = BMI(array[i].weight,array[i].height /100);
	}

	fclose(input);											// close the input file
	qsort(array, N, sizeof(person), compare);				// sort first N person by distance between threshold and BMI value

	output = fopen("output.txt", "w");						// open the output file with writing mode
	fprintf(output,"Person_id\tHeight(cm)\tWeight(kg)\t");	// write the titles to output file
	fprintf(output, "BMI\t|BMI-Threshold|\n");

	for (i = 0; i < M; i++){								// write sorted person information to output file
		fprintf(output, "%d\t%.0lf\t", array[i].personId, array[i].height);
		fprintf(output, "%.0lf\t%.2lf\t%.2lf\n", array[i].weight, array[i].BMI, absolute(array[i].BMI - threshold));
	}

	fclose(output);											// close the output file
	free(array);											// free allocated memories for array
	free(buffer);											// free allocated memories for buffer

	return 0;												// terminate the program
}
