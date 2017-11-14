/* @Author
 * Student Name: Kadir Emre Oto
 * Student ID: 150140032
 * Date: 23.11.2016 */

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <time.h>

#define SNAME_LENGTH 50
#define LNAME_LENGTH 50
#define STYLE_LENGTH 30
#define SINGER_LENGTH 50

using namespace std;
struct playlist;

struct song{
	char* name = new char[SNAME_LENGTH];
	char* singer = new char[SINGER_LENGTH];
	char* style = new char[STYLE_LENGTH];
	int year;
};

struct song_node{
	song* data;
	song_node* next;
	song_node* prev;
	playlist* parent;

	void display(){
		printf("%s - %s - %s\n", data->name, data->singer, data->style);
	}

	void play(){
		printf("playing . . .\n");
		printf("%s - %s - %s\n", data->name, data->singer, data->style);
		printf("NEXT(N) - PREVIOUS(P) - EXIT(E)\n");
	}
};

struct playlist{
	int songnumber;
	char* name = new char[LNAME_LENGTH];
	song_node* head;
	playlist* next;
	playlist* prev;

	void create(char* pname){
		head = NULL;
		next = NULL;
		prev = NULL;
		strcpy(name, pname);													// set playlist name
	}

	void display(){																// display all songs in the playlist
		int index = 1;
		song_node* cursor = head;												// cursor is traverse and starts as head
		printf("\t%s\n", name);
		while (cursor->next != head){											// except last node
			printf("%d. ", index++);											// print and increse index
			cursor->display();													// print the node
			cursor = cursor->next;												// take forward the node
		}
		printf("%d. ", index++);												// print and increase index
		cursor->display();														// print LAST node
	}

	void play(int start){														// play the playlist
		char choice = 'c';														// 'c' is not necessary

		song_node* cursor = head;
		for (int i=0; i < start; i++, cursor = cursor->next);					// move cursor <start> times forward

		while (choice != 'e' && choice != 'E'){									// if choice is 'e' or 'E' terminate the loop
			cursor->play();

			choice = getchar(); getchar();										// get char from user (second one takes '\n')
			if (choice == 'n' || choice == 'N') cursor = cursor->next;			// go forward
			if (choice == 'p' || choice == 'P') cursor = cursor->prev;			// go back
		}
	}

	void shuffle(){																// shuffle function for random playlist
		// We do not have to change nodes to shuffle a linked list,
		// if we swap the data of random nodes, the list will be shuffled.

		int i, r;
		song_node *cursor, *toChange;											// cursor & toChange will be swapped

		for (cursor = head; cursor->next != head; cursor=cursor->next){			// for every node in the playlist
			r = rand() % songnumber;											// random number
			for (i = 0, toChange=cursor; i < r; i++, toChange=toChange->next);	// find the rth node
			swap(cursor->data, toChange->data);									// swap data
		}
	}

	void addSongNode(song_node* toAdd){											// add a song_node object to playlist
		song_node* cursor = head;
		songnumber++;															// increase songnumber

		if (head == NULL){														// if playlist is empty, the node should be head
			head = toAdd;
			head->next = head;
			head->prev = head;
			return;
		}

		else if (!strcmp(name, "SORTED")){										// if the playlist is sorted playlist
			if (strcmp(head->data->singer, toAdd->data->singer) > 0 ||			// if the node should be head
			   (strcmp(head->data->singer, toAdd->data->singer) == 0 &&
			   (strcmp(head->data->name, toAdd->data->name) > 0))){

				toAdd->next = head;												// configure position
				toAdd->prev = head->prev;

				head->prev->next = toAdd;
				head->prev = toAdd;

				head = toAdd;

				return;															// process done, return
			}

			while (cursor->next != head){										// find the correct place
				if (strcmp(cursor->next->data->singer, toAdd->data->singer) > 0)
					break;

				if (strcmp(cursor->next->data->singer, toAdd->data->singer) == 0 &&
					strcmp(cursor->next->data->name, toAdd->data->name) > 0)
					break;

				cursor = cursor->next;
			}
		}

		else if (!strcmp(name, "CHRONOLOGICAL")){								// if the playlist is Chronological playlist
			if (head->data->year > toAdd->data->year){							// if the node should be added as first node (head)
				toAdd->next = head;												// confiure position
				toAdd->prev = head->prev;

				head->prev->next = toAdd;
				head->prev = toAdd;

				head = toAdd;
				return;															// process done, return the function
			}

			while (cursor->next != head && cursor->next->data->year < toAdd->data->year)
				cursor = cursor->next;
		}

		else 																	// if playlist is not Chronological or Sorted
			cursor = head->prev;												// add song node to end


		toAdd->next = cursor->next;												// configure the links
		cursor->next->prev = toAdd;

		cursor->next = toAdd;
		toAdd->prev = cursor;
	}
} *Chronological, *Sorted, *Random;

struct list{
	int playlist_count;
	playlist* head;

	void create(){
		playlist_count = 0;
		head = NULL;
	}

	void display(){																// display all playlists
		system("clear");
		int index = 1;
		playlist* cursor = head;

		while (cursor->next != head){											// except last node, every node will be enter
			printf("%d. %s\n", index++, cursor->name);							// print and increase index and print name of playlist
			cursor = cursor->next;												// take forward the playlist
		}
		printf("%d. %s\n", index++, cursor->name);								// for last node
	}

	void addPlaylist(playlist* p){												// add a new play list
		playlist_count++;

		if (head == NULL){														// if list is empty
			head = p;															// playlist should be head
			head->next = head;
			head->prev = head;
		}

		else{
			head->prev->next = p;												// else add it end
			p->prev = head->prev;
			head->prev = p;
			p->next = head;
		}
	}

	void addSong(playlist* toAdded){
		if (!toAdded) display();												// if toAdd is NULL, display playlists to ask to user

		if (toAdded && (strcmp(toAdded->name, "SORTED") == 0 ||					// it is not allowed to add song to those playlists
					    strcmp(toAdded->name, "RANDOM") == 0 ||
				        strcmp(toAdded->name, "CHRONOLOGICAL")==0)){

			printf("You cannot add any music to that playlist"); getchar();
			return;
		}

		char *pname = new char[LNAME_LENGTH];

		while (!toAdded){
			printf("\nEnter the *NAME* of playlist in which you want to add song: ");
			scanf("%[^\n]s", pname); getchar();
			if (strcmp(pname, "SORTED") == 0 ||									// again it is not allowed to add song to those playlists
				strcmp(pname, "RANDOM") == 0 ||
				strcmp(pname, "CHRONOLOGICAL") == 0){

				printf("You cannot add any music to that playlist\n");
				continue;
			}

			toAdded = findPlaylist(pname);
			if (!toAdded) printf("Playlist not found\n");
		}

		Sorted->display();														// display the sorted list

		int index = 1;
		song_node* cursor;

		while (index != 0){
			printf("Enter *INDEX* of song (0 for exit): ");
			scanf("%d", &index); getchar();
			if (!index) break;

			cursor = Sorted->head;
			song_node* node = new song_node;									// create new node

			for (int i=0; i < index-1; i++, cursor = cursor->next);				// find the node

			node->data = cursor->data;											// write the data into new node from selected node
			toAdded->addSongNode(node);											// add it to playlist
		}
		toAdded->display();
		delete [] pname;
	}

	void removeSong(playlist* toRemoved){
		if (!toRemoved) display();												// if toRemoved is NULL, ask to user

		if (toRemoved && (strcmp(toRemoved->name, "SORTED") == 0 ||				// it is not allowed to remove song from standart playlists
						  strcmp(toRemoved->name, "RANDOM") == 0 ||
						  strcmp(toRemoved->name, "CHRONOLOGICAL")==0)){

			printf("You cannot remove any music from that playlist"); getchar();
			return;
		}

		char *pname = new char[LNAME_LENGTH];
		while (!toRemoved){
			printf("\nEnter the *NAME* of playlist which you want to remove song: ");
			scanf("%[^\n]s", pname); getchar();
			if (strcmp(pname, "SORTED") == 0 ||
				strcmp(pname, "RANDOM") == 0 ||
				strcmp(pname, "CHRONOLOGICAL") == 0){
					printf("You cannot remoce any music from that playlist"); getchar();
					continue;
				}
			toRemoved = findPlaylist(pname);
		}

		toRemoved->display();

		printf("\nEnter the song to be removed from that playlist (INDEX): ");
		int index; scanf("%d", &index); getchar();

		song_node* cursor = toRemoved->head;
		for (int i=0; i < index-1; i++, cursor = cursor->next);

		cursor->prev->next = cursor->next;										// remove playlist
		cursor->next->prev = cursor->prev;

		if (index == 1) toRemoved->head = cursor->next;							// If the song to be removed is head

		delete cursor;															// deallocate
		delete [] pname;
	}

	void createList(){
		system("clear");
		printf("\tCREATE A NEW PLAYLIST\n");									// print the menu
		printf("Choose an operation\n");
		printf("1: Songs of a specific style\n");
		printf("2: Songs of a specific singer\n");
		printf("3: A combination of existing playlists\n");
		printf("4: A combination of existing songs\n");
		printf("0: Exit \n\n");

		int choice, count=0;
		char* key = new char[LNAME_LENGTH];
		char* pname = new char[LNAME_LENGTH];
		playlist* toAdd = new playlist;											// create new playlist
		toAdd->create("TEMPNAME");

		do {
			printf("Enter a choice (1, 2, 3, 4, 0): ");
			scanf("%d", &choice); getchar();
		}while (0 > choice || choice > 4);

		if (!choice) return;

		if (choice == 1){														// Songs of style
			while (!count){
				printf("Enter the style ('e' for exit): ");
				scanf("%[^\n]s", key); getchar();
				if (strcmp(key, "e") == 0 || strcmp(key, "E") == 0) break;

				song_node* cursor = Sorted->head;
				while (cursor->next != Sorted->head){							// except last element
					if (strcmp(cursor->data->style, key) == 0){					// if there is a match
						song_node* node = new song_node;						// create new node
						node->data = cursor->data;

						toAdd->addSongNode(node);								// and add it to list
						count++;
					}

					cursor = cursor->next;
				}
				if (strcmp(cursor->data->style, key) == 0){						// for last element
					song_node* node = new song_node;
					node->data = cursor->data;

					toAdd->addSongNode(node);
					count++;
				}

				if (!count) printf("No song found\n");
			}
		}

		else if (choice == 2){													// Songs of Singer
			while (!count){
				printf("Enter the singer ('e' for exit): ");
				scanf("%[^\n]s", key); getchar();

				if (strcmp(key, "e") == 0 || strcmp(key, "E") == 0) break;

				song_node* cursor = Sorted->head;
				while (cursor->next != Sorted->head){							// except last element
					if (strcmp(cursor->data->singer, key) == 0){				// if there is a match
						song_node* node = new song_node;						// create new node
						node->data = cursor->data;								// adjust the data

						toAdd->addSongNode(node);								// add
						count++;
					}

					cursor = cursor->next;
				}
				if (strcmp(cursor->data->singer, key) == 0){					// for last element
					song_node* node = new song_node;
					node->data = cursor->data;

					toAdd->addSongNode(node);
					count++;
				}

				if (!count) printf("No song found\n");
			}
		}

		else if (choice == 3){													// combination of playlists
			display();
			playlist* plist;
			song_node* cursor;

			while (1){
				printf("\nEnter the *NAME* of playlists ('e' for exit): ");
				scanf("%[^\n]s", key); getchar();

				if (strcmp(key, "e") == 0 || strcmp(key, "E") == 0) break;

				plist = findPlaylist(key);
				if (!plist) continue;

				cursor = plist->head;
				while (cursor->next != plist->head){							// except last element
					song_node* node = new song_node;							// create new node
					node->data = cursor->data;

					toAdd->addSongNode(node);
					cursor = cursor->next;
				}
				song_node* node = new song_node;
				node->data = cursor->data;
				toAdd->addSongNode(node);
				count++;
			}
		}

		else {																	// combination of songs
			Sorted->display();
			song_node* cursor;
			int index;

			while (1){
				printf("\nEnter the *INDEX* of song (0 for exit): ");
				scanf("%d", &index); getchar();

				if (!index) break;												// if index is 0, terminate the loop

				cursor = Sorted->head;
				for (int i=0; i < index-1; i++, cursor=cursor->next);			// find the true node

				song_node* node = new song_node;								// create new node
				node->data = cursor->data;
				toAdd->addSongNode(node);										// add
				count++;
			}
		}

		if (!count)
			delete toAdd;														// if there is no addition, deallocate
		else{
			addPlaylist(toAdd);													// add playlist to list

			printf("Enter playlist name: ");									// ask the name of playlist and set
			scanf("%[^\n]s\n", pname); getchar();
			strcpy(toAdd->name, pname);
		}

		delete [] key;															// deallocate
		delete [] pname;
	}

	void deleteList(playlist* plist){
		if (!plist) display();													// if plist is NULL, ask to user

		if (plist && (strcmp(plist->name, "SORTED") == 0 ||						// for standart playlists
					  strcmp(plist->name, "RANDOM") == 0 ||
				      strcmp(plist->name, "CHRONOLOGICAL")==0)){

			printf("You cannot delete the list"); getchar();
			return;
		}

		char* pname = new char[LNAME_LENGTH];

		while (!plist && strcmp(pname, "e") != 0 && strcmp(pname, "E")){
			printf("\nEnter the *NAME* of playlists ('e' for exit): ");
			scanf("%[^\n]s", pname); getchar();

			if (strcmp(pname, "SORTED") == 0 || strcmp(pname, "CHRONOLOGICAL")==0 ||
				strcmp(pname, "RANDOM") == 0){

				printf("You cannot delete the %s list\n", pname);
				continue;
			}

			plist = findPlaylist(pname);
			if (!plist && (strcmp(pname, "e") != 0 && strcmp(pname, "E")))
				printf("No playlist found");
		}

		delete [] pname;
		if (!plist) return;

		plist->next->prev = plist->prev;										// configure links for process of deletion
		plist->prev->next = plist->next;

		if (plist == head) head = plist->next;									// if it is head
		playlist_count--;
		delete plist;															// deallocation
	}

	playlist* findPlaylist(char* pname){										// search playlist according to name
		playlist* cursor = head;
		while (strcmp(cursor->name, pname) != 0 && cursor->next != head)
			cursor = cursor->next;

		if (strcmp(cursor->name, pname) == 0) return cursor;
		return NULL;
	}

	void printList(){
		char choice = '.'; int index = 1;
		playlist* cursor = head;;

		while (choice != 'E' && choice != 'e'){
			display();
			index %= playlist_count;
			if (!index) index = playlist_count;

			printf("\n%d. %s\n", index, cursor->name);
			printf("NEXT(N) - PREVIOUS(P) - SELECT(S) - EXIT(E)\n");

			choice = getchar(); getchar();

			if (choice == 'n' || choice == 'N'){
				index++; cursor = cursor->next;
			}
			else if (choice == 'p' || choice == 'P'){
				index--; cursor = cursor->prev;
			}

			else if (choice == 's' || choice == 'S'){
				printf("DELETE(D) - ADD SONG(A) - REMOVE SONG(R) - PLAY(P)\n" );// navigate
				choice = getchar(); getchar();									// secong getchar for '\n' character
				if (choice == 'D' || choice == 'd') { deleteList(cursor); cursor = head;}
				else if (choice == 'A' || choice == 'a') addSong(cursor);
				else if (choice == 'R' || choice == 'r') removeSong(cursor);
				else if (choice == 'P' || choice == 'p') cursor->play(0);

				choice = 's';													// before else if block, it was 's'
			}
		}
	}

	void play(){
		int index;
		char choice = '.', *pname = new char[LNAME_LENGTH];
		while (choice != 'e' && choice != 'E'){
			Random->shuffle();													// Before playying, shuffle the random list
			system("clear");													// print menu
			printf("Play>\n");
			printf("Choose an option\n");
			printf("L: Play a playlist\n");
			printf("S: Play a playlist starting from a specific song\n");
			printf("P: Play a single song\n");
			printf("E: Exit\n\n");

			printf("Enter a choice (L, S, P, E): ");

			playlist* toPlay = NULL;
			choice = getchar(); getchar();

			if (choice == 'l' || choice == 'L'){								// NAVIGATE
				while (!toPlay){
					printf("Enter the *NAME* of playlist you want to listen: ");
					scanf("%[^\n]s", pname); getchar();

					toPlay = findPlaylist(pname);
				}
				toPlay->play(0);
			}

			else if (choice == 's' || choice == 'S'){							// NAVIGATE
				display();
				while (!toPlay){
					printf("\nEnter the *NAME* of playlist you want to listen: ");
					scanf("%[^\n]s", pname); getchar();

					toPlay = findPlaylist(pname);
				}

				toPlay->display();
				printf("\nEnter the *INDEX* of song you want to listen: ");
				scanf("%d", &index); getchar();
				toPlay->play(index-1);
			}

			else if (choice == 'p' || choice == 'P'){							// NAVIGATE
				Sorted->display();
				printf("\nEnter the *INDEX* of song you want to listen: ");
				scanf("%d", &index); getchar();

				song_node* cursor = Sorted->head;
				for (int i=0; i < index-1; i++, cursor = cursor->next);
				printf("playing . . .\n");
				cursor->display();
				getchar();
			}
		}
		delete [] pname;														// deallocation
	}

} *baselist;

void writePlaylist(FILE* stream, playlist* plist){								// write a list to file
	song_node* cursor = plist->head;
	do{
		fprintf(stream, "%s\t%s\t%d\t%s\r\n", cursor->data->name, cursor->data->singer,
											  cursor->data->year, cursor->data->style);

		cursor = cursor->next;
	}while(cursor != plist->head);
}

void writeToFile(){
	FILE *database = fopen("songbook.txt", "w");
	writePlaylist(database, Chronological);										// write the Chronological list

	playlist* cursor = baselist->head;
	do{
		if (strcmp(cursor->name, "SORTED") != 0 &&								// if the playlist is not standart
			strcmp(cursor->name, "RANDOM") != 0 &&
			strcmp(cursor->name, "CHRONOLOGICAL") != 0){

			fprintf(database, "*****\r\n");
			fprintf(database, "%s\r\n", cursor->name);
			writePlaylist(database, cursor);

		}

		cursor = cursor->next;
	}while (cursor != baselist->head);
	fclose(database);
}

void exit(){
	writeToFile();																// save the changes

	song_node* cursor;
	playlist* plist = baselist->head;

	for (int i=0; i < baselist->playlist_count - 1; i++){						// deallocate all stuff
		cursor = plist->head;
		for (int j=0; j < plist->songnumber - 1; j++){
			cursor = cursor->next;
			delete cursor->prev;
		}
		plist=plist->next;
		delete cursor;
		delete plist->prev;
	}
	delete plist;
	delete baselist;
}

bool songcmp(song* s1, song* s2){												// compare two song
	if (strcmp(s1->name, s2->name) != 0) return false;							// if they are different
	if (strcmp(s1->singer, s2->singer) != 0) return false;						// return false
	if (strcmp(s1->style, s2->style) != 0) return false;
	if (s1->year != s2->year) return false;

	return true;																// else, return true
}

void read(){
	FILE *database = fopen("songbook.txt", "r");

	char* pname = new char[LNAME_LENGTH];
	playlist* plist;
	strcpy(pname, "None");

	while (!feof(database)){
		song* data = new song;

		fscanf(database, "%[^\t\r]s", data->name); fgetc(database);

		if (!strcmp(data->name, "*****")){										// meaning, it is user generated list
			fgetc(database);
			fscanf(database, "%[^\t\r\n]s", pname); fgetc(database); fgetc(database);
			plist = new playlist;
			plist->create(pname);
			baselist->addPlaylist(plist);
			continue;
		}

		if (feof(database)) break;												// Sometimes after first scanf, seek pointer reachs end of file
		fscanf(database, "%[^\t\r]s", data->singer); fgetc(database);
		fscanf(database, "%d", &(data->year)); fgetc(database);
		fscanf(database, "%[^\n]s", data->style); fgetc(database);

		if (*(data->style+strlen(data->style)-1) == '\r')						// If the string has "Carriage Return" character
			*(data->style+strlen(data->style)-1) = '\0';						// replace them with NULL character

		if (strcmp(pname, "None") == 0){										// for standart lists
			song_node* node1 = new song_node;
			song_node* node2 = new song_node;
			song_node* node3 = new song_node;

			node1->data = data;
			node2->data = data;
			node3->data = data;

			Random->addSongNode(node1);
			Sorted->addSongNode(node2);
			Chronological->addSongNode(node3);
		}

		else{																	// for user generated lists
			song_node* node = new song_node;
			song_node* cursor = Sorted->head;
			while (cursor->next != Sorted->head && !songcmp(cursor->data, data))// find the correct song
				cursor = cursor->next;

			if (songcmp(cursor->data, data)){
				node->data = cursor->data;
				plist->addSongNode(node);
				delete data;													// deallocate
			}
		}
	}

	fclose(database);															// close the file
	delete pname;
}

void display(){																	// print main menu
	system("clear");
	printf("\nSONG BOX APPLICATION (USING DOUBLY, CIRCULAR MULTI LINKED LIST)\n");
	printf("Choose an operation\n");
	printf("P: Play\n");
	printf("L: Show all playlists\n");
	printf("A: Add song to a playlist\n");
	printf("R: Remove song from a playlist\n");
	printf("C: Create a new playlist\n");
	printf("D: Delete a playlist\n");
	printf("W: Write to file (SAVE)\n");
	printf("E: Exit\n\n");

	printf("Enter a choice (P,L,A,R,C,D,W,E): ");
}

int main(){
	srand(time(NULL));															// for random numbers

	baselist = new list;														// allocate
	baselist->create();

	Random = new playlist;														// allocate
	Sorted = new playlist;														// allocate
	Chronological = new playlist;												// allocate

	Random->create("RANDOM");
	Sorted->create("SORTED");
	Chronological->create("CHRONOLOGICAL");

	baselist->addPlaylist(Random);
	baselist->addPlaylist(Sorted);
	baselist->addPlaylist(Chronological);

	read();

	char choice = '.';
	while (choice != 'E' && choice != 'e'){
		display();
		choice = getchar(); getchar();

		if (choice == 'P' || choice == 'p') baselist->play();					// Navigate
		if (choice == 'L' || choice == 'l') baselist->printList();
		if (choice == 'A' || choice == 'a') baselist->addSong(NULL);
		if (choice == 'R' || choice == 'r') baselist->removeSong(NULL);
		if (choice == 'C' || choice == 'c') baselist->createList();
		if (choice == 'D' || choice == 'd') baselist->deleteList(NULL);
		if (choice == 'W' || choice == 'w') writeToFile();
		if (choice == 'E' || choice == 'e') exit();
	}
	return 0;
}
