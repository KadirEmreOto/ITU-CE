#include <stdio.h>
#include <stdlib.h> // RAND_MAX constant is defined here
#include <time.h>   // time()   function is defined here

#define UPPER_RAND_RANGE     20
#define NUM_RANDS_GENERATED  10

int main (void) {
   int i, random_number;
	
   // srand() sets seed of the random number generator.
   // In order to generate different set of random numbers,
   // I use the current time as the seed: time(NULL) so that  
   // when I run the program at different times, 
   // I get different set of random numbers
   srand( time(NULL) );

   printf("\n");

   printf("rand() will generate Random Numbers Between 0 and RAND_MAX=%d\n", RAND_MAX);
   printf("I generate %d Random Numbers by calling rand() and print them out: \n", NUM_RANDS_GENERATED);
   for (i = 1; i <= NUM_RANDS_GENERATED; i++) {
      random_number = rand();
      printf("   random_number(%d) = %d\n", i, random_number);
   } // for

   printf("\n");

   // As you can see, generated numbers can be too large.
   // I need to map the generated numbers to a specified range.
   // C has the modulus (%) operator, I can use it! 
   // When I say (random_number % UPPER_RAND_RANGE), 
   // the generated numbers that range from 0 to RAND_MAX 
   // will be mapped to the numbers that range from 0 to UPPER_RAND_RANGE-1.
   // When I add 1 to (random_number % UPPER_RAND_RANGE), 
   // I can gt random numbers that range from 1 to UPPER_RAND_RANGE
   printf("Generate %d Random Numbers Between 1 and UPPER_RAND_RANGE=%d\n", NUM_RANDS_GENERATED, UPPER_RAND_RANGE );
   printf("by using Modulus (%%) Operator:\n");
   for (i = 1; i <= NUM_RANDS_GENERATED; i++) {
      random_number = (rand() % UPPER_RAND_RANGE) + 1;
      printf("   random_number(%d) = %d\n", i, random_number);
   } // for
   
   printf("\n");

   return 0;
} // main 

