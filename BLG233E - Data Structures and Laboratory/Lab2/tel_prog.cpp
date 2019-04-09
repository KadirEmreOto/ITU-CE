#include <iostream>
#include <stdlib.h>
#include <iomanip>
//#include <conio.h>
#include <ctype.h>
#include "diziislemleri.h"

using namespace std;
typedef Dizi Veriyapisi;
Veriyapisi defter;

void menu_yazdir();
bool islem_yap(char);
void kayit_ara();
void kayit_ekle();
void kayit_sil();
void kayit_guncelle();
void kayit_listele();

int main(){
	defter.olustur();
	bool bitir = false;
	char secim;
	while (!bitir){
		menu_yazdir();
		cin >> secim;
		bitir = islem_yap(secim);
	}
	return EXIT_SUCCESS;
}
void menu_yazdir(){
	system("clear");
	cout << endl << endl;
	cout << "Phone Book Application" << endl;
	cout << "Choose an operation" << endl;
	cout << "A: Search Record" << endl;
	cout << "E: Add Record" << endl;
	cout << "G: Update Record" << endl;
	cout << "S: Delete Record" << endl;
	cout << "L: List Records" << endl;
	cout << "C: Exit" << endl;
	cout << endl;
	cout << "Enter an option {A, E, G, S, C, L} : ";
}

bool islem_yap(char secim){
	bool sonlandir=false;
	switch (secim) {
		case 'A': case 'a':
			kayit_ara();
			break;
		case 'E': case 'e':
			kayit_ekle();
			break;
		case 'G': case 'g':
			kayit_guncelle();
			break;
		case 'S': case 's':
			kayit_sil();
			break;
		case 'C': case 'c':
			cout << "Are you sure that you want to terminate the program?(E/H):";
			cin >> secim;
			if(secim=='E' || secim=='e')
				sonlandir=true;
			defter.kaydet();
			break;
		case 'L': case 'l':
			kayit_listele();
			break;
		default:
			cout << "Error: You have made an invalid choice" << endl;
			break;
	}
	return sonlandir;
}

void kayit_ara(){
	char ad[AD_UZUNLUK];
	cout << "Please enter the name of the person you want to search (press '*' for listing all):" << endl;
	cin.ignore(1000, '\n');
	cin.getline(ad,AD_UZUNLUK);
	if(defter.ara(ad)==0)
		cout << "Record can not be found" << endl;
	getchar();
};

void kayit_ekle(){
	Tel_Kayit yenikayit;
	cout << "Please enter the information of the person you want to save " << endl;
	cout << "Name : ";
	cin.ignore(1000, '\n');
	cin.getline(yenikayit.ad,AD_UZUNLUK);
	cout << "Phone number :";
	cin >> setw(TELNO_UZUNLUK) >> yenikayit.telno;
	defter.ekle(&yenikayit);
	cout << "Record has been added" << endl;
	getchar();
};

void kayit_sil(){
	char ad[AD_UZUNLUK];
	int secim;
	cout << "Please enter the name of the person you want to delete (press '*' for listing all):" << endl;
	cin.ignore(1000, '\n');
	cin.getline(ad,AD_UZUNLUK);

	int kisisayisi=defter.ara(ad);
	if(kisisayisi==0)
		cout << "Record can not be found" << endl;
	else{
		if (kisisayisi==1){
			cout << "Record has been found." << endl;
			cout << "Please enter the index of the record if you want to delete this contact (Press -1 to exit without deletion): " ;
		}
		else
			cout << "Please enter the index of the record that you want to delete (Press -1 to exit without deletion): " ;
		cin >> secim;
		if(secim==-1) return;
		defter.sil(secim);
		cout << "Record has been deleted" <<endl;
	}
	getchar();
};

void kayit_guncelle(){
	char ad[AD_UZUNLUK];
	int secim;
	cout << "Please enter the name of the person you want to update (press '*' for listing all):" << endl;
	cin.ignore(1000, '\n');
	cin.getline(ad,AD_UZUNLUK);
	int kisisayisi=defter.ara(ad);
	if(kisisayisi==0)
		cout << "Record can not be found" << endl;
	else{
		if (kisisayisi==1){
			cout << "Record has been found." << endl;
			cout << "Please enter the index of the record if you want to update this contact (Press -1 to exit without updating) " ;
		}
		else
			cout << "Please enter the index of the record that you want to update (Press -1 to exit without updating): " ;
		cin >> secim;
		if(secim==-1) return;
		Tel_Kayit yenikayit;
		cout << "Please enter the up-to-date information" << endl;
		cout << "Name : " ;
		cin.ignore(1000, '\n'); cin.getline(yenikayit.ad,AD_UZUNLUK);
		cout << "Phone number :";
		cin >> setw(TELNO_UZUNLUK) >> yenikayit.telno; defter.guncelle(secim,&yenikayit);
		cout << "Record has been updated successfully" <<endl;
	}
	getchar();
};

void kayit_listele(){
	defter.listele();
	getchar();
	getchar();
}
