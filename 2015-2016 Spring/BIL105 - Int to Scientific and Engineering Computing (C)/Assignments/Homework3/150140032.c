
// Author Name : Kadir Emre Oto
// Student ID  : 150140032
// E-mail	   : otok@itu.edu.tr

#include <stdio.h>								// for standart input/output functions
#include <string.h>								// for string functions
#include <stdlib.h>								// for dynamic allocation functions

int user_menu(void){							// print the user menu
	int choice;

	printf("\n0: Exit the program\n");
	printf("1: Set Current Character Sequence\n");
	printf("2: Get Substring\n");
	printf("3: Remove Substring\n");
	printf("4: Insert\n");
	printf("5: Replace\n\n");
	printf("Enter Your Choice: ");
	scanf("%d", &choice); getchar(); 			// Read choice and get a char (\n) to clean buffer

	return choice;
}

int set_ccs(char **ccs){
	*ccs = malloc(sizeof(char) * 81);								// allocate 81 Character initially
	scanf("%[^\n]s", *ccs);											// read the ccs with spaces
	*ccs = realloc(*ccs, sizeof(char) * (strlen(*ccs) + 1)); 		// reallocate the ccs (+1 for NULL Char)

	if (*ccs == NULL) return -1;									// if setting ccs is unsuccessful, return -1
	return (int)sizeof(char) * (strlen(*ccs) + 1);					// return number of bytes that is allocated
}

char *sub_string(char *ccs, int begin_index, int end_index){
	if (strlen(ccs) <= begin_index || strlen(ccs) <= end_index)		// If begin and end index out of range, return NULL
		return NULL;

	int i;															// define i for "for loop"
	char *sub = malloc(sizeof(char)*(end_index - begin_index + 2));	// define a char pointer and allocate memory just enough

	for (i = 0; i <= end_index - begin_index; i++)					// set the sub
		*(sub+i) = *(ccs+i+begin_index);							// sub[i] = ccs[i+begin_index]
	*(sub+i) = '\0';												// put the null character at the end of sub
	return sub;														// return sub
}

char *remove_string(char **ccs, int begin_index, int end_index){
	int i, diff = end_index - begin_index + 1;						// diff is length of string "remove"
	char *sub = sub_string(*ccs, begin_index, end_index);			// get the string which will be removed with sub_string function

	if (sub == NULL) return sub;									// if indeces are out of range, sub would be NULL. Then return NULL

	for (i = end_index+1; i <= strlen(*ccs); i++)					// otherwise, remove the interval (begin_index - end_index inclusive)
		*(*ccs+i-diff) = *(*ccs+i);									// slide the characters

	*ccs = realloc(*ccs, sizeof(char) * (strlen(*ccs) + 1));		// reallocate the ccs
	return sub;														// return the removed string
}

int insert_string(char **ccs, char *insert, int begin_index){
	int i;															// for "for loop"
	int inslen = strlen(insert);									// length of insert
	int ccslen = strlen(*ccs);										// length of ccs
	int total = inslen + ccslen + 1;								// total length of ccs when insertion string will be add

	*ccs = realloc(*ccs, sizeof(char) * total);						// reallocate the css, because length will increase
	for (i = ccslen; begin_index <= i; i--)							// start with end of ccs and
		*(*ccs+i+inslen) = *(*ccs+i);								// slide the character to right until begin index

	for ( i = 0; i < inslen; i++)									// add the characters in insert into the ccs
		*(*ccs+i+begin_index) = *(insert+i);

	free(insert);													// clean the insert
	if (*ccs == NULL) return -1;									// if insertion is unsuccessful and ccs is NULL, return -1
	return total;													// return new length of ccs
}

int replace_string(char **ccs, char *find, char *replace){
	int i, j, diff, times=0;										// define some variables
	int lenccs = strlen(*ccs);										// define and calculate length of strings
	int lenfind = strlen(find);
	int lenreplace = strlen(replace);

	for (i = 0, j = 0; i < lenccs; i++){							// i: iterator on ccs, j: iterator on find
		diff = times * (lenreplace - lenfind);						// diff is change of length caused by replacements

		if (*(*ccs+i+diff) == *(find+j)) j++;						// mean i'th char of ccs is equal to j'th char of find, so j is increase
		else {i -= j; j = 0;}										// else dismatched the characters, and j is set to 0

		if (j == lenfind){											// if j == lenfind: it means the word was found
			char *temp = malloc(sizeof(char) * lenreplace); 		// create temp because insert_string function frees
			strcpy(temp, replace);									// copy the replace to the temp

			remove_string(ccs, i-j+diff+1, i+diff);					// remove the string found
			insert_string(ccs, temp, i-j+diff+1);					// insert the new string (replace)
			times++;												// times is count of replacements, so increase
			j = 0;													// set the j to 0 for new replacements
		}
	}

	free(find);														// clean the find and the replace
	free(replace);
	return times;													// return count of replacements
}

int main() {
	int choice, space, times;
	char *ccs;
	char *insert, *find, *replace;
	int begin_index, end_index;

	while (1){
		choice = user_menu();
		if (!choice) break;											// if choice is 0, break the loop
		else if (choice != 1 && ccs == NULL){						// if css is not set, print that message
			printf("You have to set Current Character Sequence (CCS) before doing any operations!\n");
			continue;
		}
		if (choice == 1){											// if choice is 1 go to set_ccs
			printf("Enter CCS: ");
			space = set_ccs(&ccs);

			if (space == -1) printf("The allocation is unsuccessful\n");
		}
		else if (choice == 2){										// if choive is 2 go to sub_string
			printf("Enter the BEGIN INDEX and END INDEX numbers: ");
			scanf("%d%d", &begin_index, &end_index);

			char *sub = sub_string(ccs, begin_index, end_index);
			printf("\nCCS: \"%s\"\n", ccs);
			printf("Substring(%d,%d): \"%s\"\n\n", begin_index, end_index, sub);
			free(sub);												// clean up the sub
		}
		else if (choice == 3){
			printf("Enter the BEGIN INDEX and END INDEX numbers: ");
			scanf("%d%d", &begin_index, &end_index);

			char *sub = remove_string(&ccs, begin_index, end_index);
			printf("\nCCS: \"%s\"\n", ccs);
			printf("Removed String(%d,%d): \"%s\"\n\n", begin_index, end_index, sub);
			free(sub);
		}
		else if (choice == 4){
			insert = malloc(sizeof(char) * 81);
			printf("Enter a new string for insertion: ");
			scanf("%[^\n]s", insert);

			printf("Enter the BEGIN INDEX number where the new string begins: ");
			scanf("%d", &begin_index);

			space = insert_string(&ccs, insert, begin_index);
			printf("\nCCS: \"%s\"\n", ccs);

		}
		else if (choice == 5){
			find = malloc(sizeof(char) * 81);
			printf("Find what: ");
			scanf("%[^\n]s", find); getchar();		// read find and clean buffer

			replace = malloc(sizeof(char) * 81);
			printf("Replace with: ");
			scanf("%[^\n]s", replace);

			times = replace_string(&ccs, find, replace);
			printf("There were %d replacement(s).\n", times);
			printf("\nCCS: \"%s\"\n", ccs);
		}
	}

	printf("Goodbye!\n");  			// print goodbye message before terminating
	return 0;
}
