/* @Author
 * Student Name: Kadir Emre Oto
 * Student ID: 150140032
 * Date: 18.10.2016
*/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;
typedef struct patient patient;													// for clear and fast typing
const int MAXN = 1000;															// for size of used arrays

struct patient{
	char name[30], doctorName[30];
	char branchName[30], diagnosis[30];
	int patientNumber, polyclinicNumber;
}ar[MAXN];																		// instance patient array for size 1000

int current=0;																	// current holds how much elements are in array of patient, ar

void display(){
	system("clear");

	cout << "Please select the operation to perform and enter the opeartion code" << endl;
	cout << "(P) Print all of the patient records, " << endl;
	cout << "(B) Search the data by the branch name, " << endl;
	cout << "(C) Search the data by the polyclinic number, " << endl;
	cout << "(I) Insert a new patient record, " << endl;
	cout << "(R) Remove the patient record, " << endl;
	cout << "(E) Exit the program, " << endl;
	cout << "Your selection is: ";
}

void list(){
	for (int i=0; i < current; ++i){
		cout << i+1 << "." << "\t";
		if (!strcmp((ar+i)->name, "")) {cout << endl; continue;}				// if the record is null, just pass the new line
		cout << (ar+i)->name << "\t";
		cout << (ar+i)->doctorName << "\t";
		cout << (ar+i)->diagnosis << "\t";
		cout << (ar+i)->patientNumber << "\t";
		cout << (ar+i)->polyclinicNumber << "\t";
		cout << (ar+i)->branchName << endl;
	}
}

int compare(patient *a, patient *b){											// for fast typing
	return strcmp(a->branchName, b->branchName);								// compare two struct according to branchName
}

bool isPolyclinicNumberValid(int pn){											// check if polyclinicNumber is used before
	int pos;
	for (pos=0; pos < current && (ar+pos)->polyclinicNumber != pn; pos+=10);	// search the polyclinicNumber on records array
	return ((ar+pos)->polyclinicNumber != pn);
}

bool isPatientNumberUnique(int pn){
	int pos;
	for(pos=0; pos < current && (ar+pos)->patientNumber != pn; pos++);			// search the patient number on records array
	return ((ar+pos)->patientNumber != pn);
}

void deletePatient(patient *p){													// delete (clear) all variables in record struct
	strcpy(p->name, "");
	strcpy(p->doctorName, "");
	strcpy(p->diagnosis, "");
	strcpy(p->branchName, "");
	p->polyclinicNumber = 0;
	p->patientNumber = 0;
}

bool add(patient *p){
	if (p->patientNumber > 9999 || p->patientNumber < 1000){					// if patientNumber is out of range
		cerr << endl << "Error: The patient number must have exacly 4 digits!" << endl;
		getchar(); return false;
	}

	if (!isPatientNumberUnique(p->patientNumber)){
		cerr << endl << "Error: The patient number is in use" << endl;
		getchar(); return false;
	}

	int pos;
	for (pos = 0; pos < current & compare(p, (ar+pos)) > 0; pos+=10);			// Find the lower bound according to branchName

	if (!compare(p, (ar+pos))){													// If the branch is added before.
		int s = (ar+pos)->polyclinicNumber, b=-1;								// s: small polyclinicNumber
		if (!compare(p, (ar+pos+10))) b = (ar+pos+10)->polyclinicNumber;		// b: big polyclinicNumber

		if (s != p->polyclinicNumber && b != -1 && b != p->polyclinicNumber){	// if branch have 2 polyclinic and user wants to add a new polyclinic
			cerr << endl << "Error: A branch can have only 2 polyclinics!"; 	// error message appears
			getchar(); return false;
		}

		if (s == p->polyclinicNumber){											// if the record should be added to smaller polyclinic
			for (int j=0; j < 10; j++){
				if (!strcmp((ar+pos+j)->name, "")){								// find first null (blank) position in polyclinic
					swap(*p, *(ar+pos+j));										// copy record
					return true;
				}
			}
			cerr << endl << "Error: Polyclinic is full. (10/10)" << endl; 		// error message appears
			getchar(); return false;
		}

		else if (b == p->polyclinicNumber){										// if the record should be added to bigger polyclinic
			for (int j=10; j < 20; j++){
				if (!strcmp((ar+pos+j)->name, "")){								// find first null (blank) position in polyclinic
					swap(*p, *(ar+pos+j));										// copy record
					return true;
				}
			}
			cerr << endl << "Error: Polyclinic is full. (10/10)" << endl; 		// Error Message
			getchar(); return false;
		}

		else{																	// if the branch exists and have only one polyclinic :> new polyclinic should be add if it is valid
			int l=10;															// (pos + l) is the position that new record should be added
			if (p->polyclinicNumber < s) l = 0;									// if new record is smaller than current polyclinic

			if (!isPolyclinicNumberValid(p->polyclinicNumber)){
				cerr << endl << "Error: The polyclinic is in different branch" << endl;
				getchar(); return false;
			}

			if (pos+l != current)												// if new branch does not add the end of array
				for (int j=current-1; pos+l <= j; j--)							// shift all records that are bigger 10 times forward
					swap(*(ar+j+10), *(ar+j));

			swap(*p, *(ar+pos+l));												// add the record to where is supposed
			for (int j=l+1; j<l+10; j++) deletePatient(ar+pos+j);				// expand the current size of array for 10

			current += 10;														// expand the current size of array for 10
		}
	}

	else{																		// If branch will be added for the first time
		if (!isPolyclinicNumberValid(p->polyclinicNumber)){						// check the polyclinicNumber
			cerr << endl << "Error: The polyclinic is in different branch" << endl;
			getchar(); return false;
		}

		if (pos != current)														// if new branch does not add the end of array
			for (int j=current-1; pos <= j; j--)								// shift all records that are bigger 10 times forward
				swap(*(ar+j+10), *(ar+j));

		swap(*p, *(ar+pos));													// add the record to where is supposed
		for (int j=1; j<10; j++) deletePatient(ar+pos+j);						// clear data remainder of the shifted records

		current += 10;															// expand the current size of array for 10
	} return true;
}

void remove(){
	int temp;																	// to hold patient number
	cout << "Enter patient number: "; cin >> temp; getchar();

	int pos;
	for (pos=0; pos < current && (ar+pos)->patientNumber != temp; pos++);		// determine the record that is supposed to delete

	if ((ar+pos)->patientNumber != temp){										// If the patient number is not on the list
		cerr << endl << "Patient does not found!" << endl;						// write error message
		getchar(); return;														// and terminate the function
	}

	int j;
	for (j=pos+1; j % 10; j++) swap(*(ar+j), *(ar+j-1));						// move the record wishing delete to end of polyclinic

	deletePatient(ar+j-1);														// delete the last record of polyclinic

	if (strcmp((ar+pos-pos%10)->name, "") == 0){								// If the polyclinic deleted completely
		for (j=pos-pos%10+10; j < current; j++) swap(*(ar+j), *(ar+j-10));		// Clear all null records
		for (j=0; j < 10; j++) deletePatient(ar+current-1-j);

		current -= 10;															// decrease the size of array
	}

	cout << endl << "Record has been deleted!" << endl; getchar();
}

void search(int by){
	if (by == 1){ 																// by branchName
		char temp[30]; getchar();
		cout << "Enter branch name: "; cin.getline(temp, 30);

		int pos, l=10;
		for (pos = 0; pos < current && strcmp(temp, (ar+pos)->branchName) > 0; pos+=10);
		if (!strcmp(temp, (ar+pos+10)->branchName)) l = 20;

		if (strcmp(temp, (ar+pos)->branchName)){
			cerr << endl << "Branch does not found!" << endl;
			getchar(); return;
		}

		for (int j=0, c=1; j < l; j++){
			if (!strcmp((ar+pos+j)->name, "")) continue;
			cout << c++ << "." << "\t";
			cout << (ar+pos+j)->name << "\t";
			cout << (ar+pos+j)->doctorName << "\t";
			cout << (ar+pos+j)->diagnosis << "\t";
			cout << (ar+pos+j)->patientNumber << "\t";
			cout << (ar+pos+j)->polyclinicNumber << "\t";
			cout << (ar+pos+j)->branchName << endl;
		} getchar();
	}

	else if (by == 2){															// by polyclinicNumber
		int temp; getchar();
		cout << "Enter polyclinic number: "; cin >> temp;

		int pos;
		for (pos = 0; pos < current && temp != (ar+pos)->polyclinicNumber; pos+=10);

		if (pos == current){
			cerr << endl << "Polyclinic does not found!" << endl;
			getchar(); getchar(); return;
		}

		for (int j=0; j < 10; j++){
			if (!strcmp((ar+pos+j)->name, "")) break;
			cout << j+1 << "." << "\t";
			cout << (ar+pos+j)->name << "\t";
			cout << (ar+pos+j)->doctorName << "\t";
			cout << (ar+pos+j)->diagnosis << "\t";
			cout << (ar+pos+j)->patientNumber << "\t";
			cout << (ar+pos+j)->polyclinicNumber << "\t";
			cout << (ar+pos+j)->branchName << endl;
		} getchar(); getchar();
	}
}

void read(){
	FILE *database = fopen("database.txt", "r");

	patient temp;
	while (!feof(database)){
		fscanf(database, "%[^\t]s", temp.name); fgetc(database);
		if (feof(database)) break;												// Sometimes after first scanf, seek pointer reachs end of file
		fscanf(database, "%[^\t]s", temp.doctorName); fgetc(database);
		fscanf(database, "%[^\t]s", temp.diagnosis); fgetc(database);
		fscanf(database, "%d", &(temp.patientNumber));
		fscanf(database, "%d", &(temp.polyclinicNumber)); fgetc(database);
		fscanf(database, "%[^\n\t]s", temp.branchName); fgetc(database);

		if (*(temp.branchName+strlen(temp.branchName)-1) == 13)					// If the string has "Carriage Return" character
			*(temp.branchName+strlen(temp.branchName)-1) = 0;					// replace them with NULL character

		add(&temp);
	}
	fclose(database);
}

void save(){
	FILE *database = fopen("database.txt", "w");								// open file

	for (int i=0; i < current; i++){
		if (!strcmp((ar+i)->name, "")) continue;								// if the record is NULL (empty), just ignore

		fprintf(database, "%s\t", (ar+i)->name);
		fprintf(database, "%s\t", (ar+i)->doctorName);
		fprintf(database, "%s\t", (ar+i)->diagnosis);
		fprintf(database, "%d\t", (ar+i)->patientNumber);
		fprintf(database, "%d\t", (ar+i)->polyclinicNumber);
		fprintf(database, "%s\n", (ar+i)->branchName);
	}
	fclose(database);															// close file
}

int main(int argc, char const *argv[]) {
	char choice;
	bool terminate = false;														// to end while loop below
	read();																		// read records from database.txt

	while (!terminate){
		display();																// display menu
		choice = getchar();														// user choice

		if (choice == 'P' || choice == 'p') {
			list();																// list all records
			getchar(); getchar();
		}

		else if (choice == 'B' || choice == 'b') search(1);
		else if (choice == 'C' || choice == 'c') search(2);
		else if (choice == 'R' || choice == 'r') remove();
		else if (choice == 'E' || choice == 'e') terminate = true;
		else if (choice == 'I' || choice == 'i'){
			getchar();
			patient p;															// Instance temporary patient

			cout << "Enter patient name: "; cin.getline(p.name, 50);
			cout << "Enter doctor name: "; cin.getline(p.doctorName, 50);
			cout << "Enter diagnosis: "; cin.getline(p.diagnosis, 50);
			cout << "Enter patient number (with 4 digits): "; cin >> p.patientNumber;
			cout << "Enter polyclinic number: "; cin >> p.polyclinicNumber; getchar();
			cout << "Enter branch name: "; cin.getline(p.branchName, 50);

			if (add(&p)){														// If addition is succesfull
				cout << endl << "Patient has been added." << endl;
				getchar();
			}
		}
		else {
			cout << endl << "Invalid Choice! Press enter for continue.." << endl;
			getchar(); getchar();
		}
	}

	save();																		// If the while loop ends, it means user wants to end program, save all changes.
	return 0;
}
