#include <iostream>
#include <cstdlib>

#define RIGHT   1
#define LEFT    2
#define UP      3
#define DOWN    4

struct node{
	int x;
	int y;
	int right;
	int left;
	int up;
	int down;
	int camefrom;

	node* next;
	node* prev;
};

struct Stack{
	node* head;
	int length;

	void create(){
		head = NULL;
		length = 0;
	}

	bool is_empty(){
		return length == 0;
	}

	void push(node* p){
		length++;
		if (head == NULL){
			head = p;
			p->next = NULL;
			p->prev = NULL;
		}

		else{
			head->next = p;
			p->prev = head;
			head = p;
		}
	}

	node* pop(){
		length--;
		node* output = head;
		head = head->prev;
		return output;
	}
};

char lab2[8][8] = {
	{'x','x','x','x','x','x','x','x'},
	{' ',' ','x','x','x',' ',' ',' '},
	{'x',' ','x','x','x',' ','x','x'},
	{'x',' ',' ',' ',' ',' ',' ','x'},
	{'x',' ','x',' ','x','x',' ','x'},
	{'x',' ','x','x',' ','x',' ','x'},
	{'x',' ',' ','x',' ',' ',' ','x'},
	{'x','x','x','x','x','x','x','x'}};

char lab[][46] = { "#o###########################################",
				   "#                     #######################",
				   "# ################### #######################",
				   "# ################### ###                 ###",
				   "# ################### ### ############### ###",
				   "# ################### ##  ############### ###",
				   "#             ####### ## ################ ###",
				   "# ########### ####### ##             #### ###",
				   "# ########### #######     ########## #### ###",
				   "# ###########  ########## ##########   ######",
				   "## #######     ########### ######## ##  E ###",
				   "##                 ########         #########",
				   "##### ######### #############################",
				   "##### #########            ##################",
				   "##### ########### ###########################",
				   "#####     ####### ###########################",
				   "##### ########### ###########################",
				   "#####                                    ####",
				   "##### #######################################",
				   "##### #######################################",};


void printlab(char l[][46]){
	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 45; j++)
			cout << l[i][j];
		cout << endl;
	}
	cout << endl << endl;
}

int main(int argc, char const *argv[]) {
	Stack s;
	s.create();

	node enterance;
	enterance.x = 0;
	enterance.y = 1;
	enterance.camefrom = UP;

	node quit;
	quit.x = 11;
	quit.y = 41;

	node cursor = enterance;
	bool backtrace = false;
	bool goback = false;

	while (cursor.x != quit.x || cursor.y != quit.y){

		if (backtrace)  lab[cursor.x][cursor.y] = ' ';
		if (!backtrace) lab[cursor.x][cursor.y] = 'o';

		if (!goback){
			if (cursor.x < 19 && lab[x+1][y] != 'x') cursor.down = 1;
			if (cursor.y < 45 && lab[x][y+1] != 'x') cursor.right = 1;
			if (cursor.x > 0 && lab[x-1][y] != 'x') cursor.up = 1;
			if (cursor.y > 0 && lab[x][y-1] != 'x') cursor.left = 1;
		}
		else goback = false;

		node past = cursor;
		bool move = true;

		if (cursor.right && cursor.camefrom != RIGHT){
			cursor.y++;
			cursor.camefrom = LEFT;
			past.right = 0;
		}

		else if (cursor.left && cursor.camefrom != LEFT){
			cursor.y--;
			cursor.camefrom = RIGHT;
			past.right = 0;
		}

		else if (cursor.down && cursor.camefrom != DOWN){
			cursor.x++;
			cursor.camefrom = UP;
			past.down = 0;
		}

		else if (cursor.up && cursor.camefrom != UP){
			cursor.x--;
			cursor.camefrom = DOWN;
			past.up = 0;
		}

		else move = false;

		if (cursor.x != quit.x || cursor.y != quit.y){
			if (cursor.right + cursor.left + cursor.up + cursor.down > 2){
				if (!backtrace) s.push(&past);
				else{
					cursor = *s.pop();
					goback = true;
				}
			}

			if (!move){
				if (cursor.camefrom == LEFT) cursor.left = 1;
				if (cursor.camefrom == RIGHT) cursor.right = 1;
				if (cursor.camefrom == DOWN) cursor.down = 1;
				if (cursor.camefrom == UP) cursor.up = 1;

				backtrace = true;
			}
		}
	}

	return 0;
}
