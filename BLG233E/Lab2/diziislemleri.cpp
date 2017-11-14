#include <iostream>
#include <stdlib.h>
//#include <cstdlib.h>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include "diziislemleri.h"

using namespace std;

void Dizi::ekle(Tel_Kayit *ykptr){
	int i=0;

	while (i < kayitsayisi && (strcmp(ykptr->ad, (k+i)->ad) > 0))
		i++;

	if (i != kayitsayisi){
		for (int j=kayitsayisi-1; i <= j; j--){
			strcpy((k+j+1)->ad, (k+j)->ad);
			strcpy((k+j+1)->telno, (k+j)->telno);
		}
	}

	strcpy((k+i)->ad, ykptr->ad);
	strcpy((k+i)->telno, ykptr->telno);
	kayitsayisi++;
}

void Dizi::kaydet(){
	fclose(teldefteri);
	teldefteri = fopen(dosyaadi, "w+");
	fwrite(k, sizeof(Tel_Kayit), kayitsayisi, teldefteri);
}

void Dizi::olustur(){
	kayitsayisi=0;

	dosyaadi = "teldefteri.txt";
	teldefteri = fopen( dosyaadi, "r+" );

	if(!teldefteri){
		if(!(teldefteri = fopen( dosyaadi, "w+" ))){
			cerr << "File can not be opened" << endl;
			exit(1);
		}
	}

	Tel_Kayit temp;
	while(!feof(teldefteri)){
		fread( &temp, sizeof (Tel_Kayit), 1, teldefteri);
		if(feof(teldefteri)) break;
		ekle(&temp);
	}
}

int Dizi::ara(char aranacak[]){
	int sayac=0;
	bool tumu=false;
	int bulunan,flag=0;

	if(strcmp(aranacak,"*")==0)
		tumu=true;

	for(int i=0;i<kayitsayisi;i++){
		if (tumu || strncmp((k+i)->ad, aranacak, strlen(aranacak))==0){
			cout << i+1 << ". " << (k+i)->ad << endl;
			bulunan=i;
			flag=1;
		}
	}
	return flag;
}

void Dizi::guncelle(int kayitno, Tel_Kayit *ykptr){
	sil(kayitno);
	ekle(ykptr);
}

void Dizi::sil(int kayitno){
	for (int i=kayitno; i < kayitsayisi; i++){
		strcpy((k+i-1)->ad,  (k+i)->ad);
		strcpy((k+i-1)->telno,  (k+i)->telno);
	}

	kayitsayisi--;
}

void Dizi::listele(){
	for(int i=0;i<kayitsayisi;i++)
		cout << i+1 << ". " << (k+i)->ad << "\t" << (k+i)->telno << endl;

	if (kayitsayisi == 0)
		cout << "Phone Book is empty!" << endl;
}
