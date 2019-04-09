#include "fileoperations.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

void Dosya::ekle(Tel_Kayit *ykptr){
	fseek(teldefteri, 0, SEEK_END);
	fwrite(ykptr, sizeof(Tel_Kayit), 1, teldefteri);
}

void Dosya::olustur(){
	dosyaadi="teldefteri.txt";
	teldefteri = fopen( dosyaadi, "r+" );

	if(!teldefteri){
		if(!(teldefteri = fopen( dosyaadi, "w+" ))){
			cerr << "File can not be opened" << endl;
			exit(1);
		}
	}
}

void Dosya::kapat(){
       fclose(teldefteri);
}

int Dosya::ara(char aranacak[]){
	Tel_Kayit k;

	int sayac=0;
	bool tumu=false;
	int bulunan=0;
	if(strcmp(aranacak,"*")==0) tumu=true;
	fseek(teldefteri, 0, SEEK_SET);

	while(!feof(teldefteri)){
		sayac++;
		fread( &k, sizeof (Tel_Kayit), 1, teldefteri);
		if(feof(teldefteri)) break;
       	if(!tumu && strncmp(k.ad, aranacak, strlen(aranacak))!=0)
			continue;
		cout << sayac << "." << k.ad << " " << k.telno << endl;
		bulunan++;
	}
	return bulunan;
}

void Dosya::guncelle(int kayitno, Tel_Kayit *ykptr){
	if(fseek(teldefteri, sizeof(Tel_Kayit)*(kayitno-1), SEEK_SET)==0)
		fwrite(ykptr, sizeof(Tel_Kayit), 1, teldefteri);
}

void Dosya::sil2(int kayitno){
	Tel_Kayit boskayit={"",""};
	if(fseek(teldefteri, sizeof(Tel_Kayit)*(kayitno-1), SEEK_SET)==0)
		fwrite(&boskayit, sizeof(Tel_Kayit), 1, teldefteri);
}

void Dosya::sil(int kayitno){
	Tel_Kayit k;
	int sayac=0;

	FILE *temp = fopen( "temp.txt", "w+" );
	fseek(teldefteri, 0, SEEK_SET);
	while(!feof(teldefteri)){
		sayac++;
		fread( &k, sizeof (Tel_Kayit), 1, teldefteri);
		if(feof(teldefteri)) break;
       	if (sayac == kayitno) continue;
		fwrite(&k, sizeof(Tel_Kayit), 1, temp);
	}

	teldefteri = fopen( dosyaadi, "w+" );
	fseek(temp, 0, SEEK_SET);
	while(!feof(temp)){
		fread( &k, sizeof (Tel_Kayit), 1, temp);
		if(feof(temp)) break;
		fwrite(&k, sizeof(Tel_Kayit), 1, teldefteri);
	}

	fclose(temp);
}
