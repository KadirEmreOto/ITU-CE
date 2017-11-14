#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;
const int MAXN=50;

struct record{
	char *name,*surname,*num;
}*ar,*tmp;

int N=2,cur;

void print_menu(bool warning){
	system("clear");

	if(warning)
		cout << "Invalid Choice! Please enter a valid character" << endl;

	cout << "A: Add" << endl;
	cout << "S: Search" << endl;
	cout << "L: List All" << endl;
	cout << "R: Remove" << endl;
	cout << "U: Update" << endl;
	cout << "Q: Quit" << endl << endl;
	cout << "Your choice: ";
}

void slide_left(int ind){
	for(int i=ind ; i ; --i)
		if(!strcmp(ar[i].name,ar[i-1].name) && strcmp(ar[i].surname,ar[i-1].surname) < 0)
			swap(ar[i],ar[i-1]);
		else if( strcmp(ar[i].name,ar[i-1].name) < 0 )
			swap(ar[i],ar[i-1]);
		else
			break;
}

void slide_right( int ind ){

	for(int i=ind ; i<cur-1 ; ++i)
		if( !strcmp(ar[i].name,ar[i+1].name) && strcmp(ar[i].surname,ar[i+1].surname) > 0 )
			swap(ar[i],ar[i-1]);
		else if( strcmp(ar[i].name,ar[i+1].name) > 0 )
			swap(ar[i],ar[i+1]);
		else
			break;
}

void add(){

	char temp[MAXN];

	cout << "Please enter name: ";

	cin.ignore(1000,'\n');
	cin.getline(temp,MAXN);

	ar[cur].name = new char[strlen(temp)];
	strcpy(ar[cur].name,temp);

	cout << "Please enter surname: ";

	cin.getline(temp, MAXN);

	ar[cur].surname = new char[strlen(temp)];
	strcpy(ar[cur].surname,temp);

	cout << "Please enter number: ";

	cin.getline(temp,MAXN);

	ar[cur].num = new char[strlen(temp)];
	strcpy(ar[cur].num,temp);

	slide_left(cur);
	cur++;

	cout << "Person has been added" << endl;
	getchar();
}

void search( int ch ){
	bool check=false;
	char temp[MAXN];

	if(ch == 0)
		cout << "Please enter the name of the person you want to look for (for all people just press enter)" << endl;
	else if(ch == 1)
		cout << "Please enter the name of the person you want to remove (for all people just press enter)" << endl;
	else
		cout << "Please enter the name of the person you want to update (for all people just press enter)" << endl;

	cin.ignore(1000,'\n');
	cin.getline(temp, MAXN);

	for (int i=0 ; i<cur ; i++)
		if(!strncmp(ar[i].name,temp, strlen(temp))){
			cout << i+1 << ". " << (*(ar+i)).name << ' ' << (*(ar+i)).surname << ' ' << (*(ar+i)).num << endl;
			check=true;
		}

	if(!check)
		cout << "Not found" << endl;

	getchar();
}

void list(){
	for(int i=0 ; i<cur ; i++)
		cout << i+1 << ". " << (*(ar+i)).name << ' ' << (*(ar+i)).surname << ' ' << (*(ar+i)).num << endl;

	if (!cur) cout << "Phone Book is empty!" << endl;

	getchar();
	getchar();
}

void remove(){

	int ind;

	search(1);

	cout << "Please enter index number of the person you want to remove: ";
	cin >> ind;
	ind--;

	for( int i=ind ; i<cur-1 ; i++ )
		swap(ar[i], ar[i+1]);

	delete ar[cur].name;
	delete ar[cur].surname;
	delete ar[cur].num;

	cur--;

	cout << "Person has been removed" << endl;
	getchar();
}

void update(){

	int ind;
	char temp[MAXN];

	search(2);

	cout << "Please enter index number of the person you want to update: ";
	cin >> ind;
	ind--;

	cout << "Please new enter name: ";

	cin.ignore(1000,'\n');
	cin.getline(temp,MAXN);

	ar[ind].name = new char[strlen(temp)];
	strcpy(ar[ind].name,temp);

	cout << "Please new enter surname: ";

	cin.getline(temp, MAXN);

	ar[ind].surname=new char[strlen(temp)];
	strcpy(ar[ind].surname,temp);

	cout << "Please new enter number: ";

	cin.getline(temp, MAXN);

	ar[ind].num = new char[strlen(temp)];
	strcpy(ar[ind].num, temp);

	slide_left(ind);
	slide_right(ind);

	cout << "Person has been updated" << endl;
	getchar();
}

void quit(){

	for( int i=0 ; i<N ; i++ ){
		delete((*(ar+i)).name);
		delete((*(ar+i)).surname);
		delete((*(ar+i)).num);
	}

	delete(ar);
	delete(tmp);
}

int main(){
	char ch;
	bool warning = 0;
	bool terminate = true;

	ar = new record[N];

	while(terminate){
		if(cur == N-1){

			N <<= 1;
			tmp = ar;
			ar = new record[N];

			for( int i=0 ; i<cur ; i++ )
				ar[i]=tmp[i];
		}

		else if (cur == N / 2 - 2){
			N >>= 1;
			tmp = ar;
			ar = new record[N];

			for( int i=0 ; i<cur ; i++ )
				ar[i]=tmp[i];
		}

		print_menu(warning);
		warning = 0;
		cin >> ch;

		if(ch=='A' || ch=='a' ) add();
   		else if(ch=='S' || ch=='s') search(0);
		else if(ch=='L' || ch=='l')	list();
		else if(ch=='R' || ch=='r') remove();
		else if(ch=='U' || ch=='u') update();
		else if(ch=='Q' || ch=='q'){
			quit();
			terminate = false;
		}
		else warning=1;
	}

	return 0;
}
