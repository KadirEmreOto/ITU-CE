#include <iostream>
#include <cstdlib>
#include <stdio.h>

using namespace std;

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

	node pop(){
		length--;
		node output = *head;
		head = head->prev;
		return output;
	}
};

char lab[15][25] = {
	"#o###################",
	"#     #      #    # #",
	"# ### # ###### ## # #",
	"# # #        # #  # #",
	"# # ### ######## ## #",
	"#     # # ##   #    #",
	"### # # # ## # # ####",
	"# # # # #    #      #",
	"# # # # # ######### #",
	"# # # # # #       # #",
	"# # #   # # #####   #",
	"# # ##### # #   #####",
	"#         #   #     #",
	"################### #"
};


void display(){
	system("clear");
	for (int i = 0; i < 14; i++){
		for (int j = 0; j < 24; j++){
			if (lab[i][j] == '#')
				printf("⬛️ ");
			else if (lab[i][j] == 'o')
				printf("😁 ");
			else
				printf("  ");
		}
		printf("\n");
	}
	system("sleep 0.05");
}

int main(int argc, char const *argv[]) {
	Stack s;
	s.create();

	node enterance;
	enterance.x = 0;
	enterance.y = 1;
	enterance.camefrom = UP;

	node quit;
	quit.x = 13;
	quit.y = 22;

	node cursor = enterance;
	bool backtrace = false;
	bool goback = false;

	while (cursor.x != quit.x && cursor.y != quit.y){
		if (backtrace)  lab[cursor.x][cursor.y] = ' ';
		if (!backtrace) lab[cursor.x][cursor.y] = 'o';
		display();

		if (!goback){
			cursor.down = 0; cursor.left = 0; cursor.right = 0; cursor.up = 0;

			if (cursor.x < 14 && lab[cursor.x+1][cursor.y] != '#') cursor.down = 1;
			if (cursor.y < 24 && lab[cursor.x][cursor.y+1] != '#') cursor.right = 1;
			if (cursor.x > 0 && lab[cursor.x-1][cursor.y] != '#') cursor.up = 1;
			if (cursor.y > 0 && lab[cursor.x][cursor.y-1] != '#') cursor.left = 1;

		} else goback = false;

		node past = cursor;
		bool move = true;

		if (cursor.right && cursor.camefrom != RIGHT &&
			((backtrace && lab[cursor.x][cursor.y+1] == 'o') || (!backtrace && lab[cursor.x][cursor.y+1] == ' '))){
			cursor.y++;
			cursor.camefrom = LEFT;
			past.right = 0;
		}

		else if (cursor.up && cursor.camefrom != UP &&
			((backtrace && lab[cursor.x-1][cursor.y] == 'o') || (!backtrace && lab[cursor.x-1][cursor.y] == ' '))){
			cursor.x--;
			cursor.camefrom = DOWN;
			past.up = 0;
		}

		else if (cursor.left && cursor.camefrom != LEFT &&
			((backtrace && lab[cursor.x][cursor.y-1] == 'o') || (!backtrace && lab[cursor.x][cursor.y-1] == ' '))){
			cursor.y--;
			cursor.camefrom = RIGHT;
			past.left = 0;
		}

		else if (cursor.down && cursor.camefrom != DOWN &&
			((backtrace && lab[cursor.x+1][cursor.y] == 'o') || (!backtrace && lab[cursor.x+1][cursor.y] == ' '))){
			cursor.x++;
			cursor.camefrom = UP;
			past.down = 0;
		}

		else move = false;

		if (cursor.x != quit.x || cursor.y != quit.y){
			if (cursor.right + cursor.left + cursor.up + cursor.down > 2){
				if (!backtrace) {
					node ttt = past;
					s.push(&ttt);
				}
				else{
					printf("1\n");
					cursor = s.pop();
					printf("2\n");
					goback = true;
					backtrace = false;
				}
			}

			if (cursor.x == quit.x && cursor.y == quit.y)
				break;

			if (!move){
				cursor.down = 0; cursor.left = 0; cursor.right = 0; cursor.up = 0;

				if (cursor.camefrom == LEFT) {
					cursor.left = 1;
				}
				if (cursor.camefrom == RIGHT) {
					cursor.right = 1;
				}
				if (cursor.camefrom == DOWN) {
					cursor.down = 1;
				}
				if (cursor.camefrom == UP) {
					cursor.down = 1;
				}
				cursor.camefrom = 0;
				goback = true;
				backtrace = true;

				int o = 0;
				o += (lab[cursor.x+1][cursor.y] == 'o');
				o += (lab[cursor.x-1][cursor.y] == 'o');
				o += (lab[cursor.x][cursor.y+1] == 'o');
				o += (lab[cursor.x][cursor.y-1] == 'o');
				if (o > 1) printf("Cycle detected\n");
			}
		}
	}

	lab[quit.x][quit.y-3] = 'o';
	display();

	cursor = enterance;
	while (cursor.x != quit.x && cursor.y != quit.y){
		printf("(%d %d)\n", cursor.x, cursor.y);

		cursor.down = 0; cursor.left = 0; cursor.right = 0; cursor.up = 0;

		if (cursor.x < 14 && lab[cursor.x+1][cursor.y] == 'o') cursor.down = 1;
		if (cursor.y < 24 && lab[cursor.x][cursor.y+1] == 'o') cursor.right = 1;
		if (cursor.x > 0 && lab[cursor.x-1][cursor.y] == 'o') cursor.up = 1;
		if (cursor.y > 0 && lab[cursor.x][cursor.y-1] == 'o') cursor.left = 1;

		if (cursor.up && cursor.camefrom != UP){
			cursor.x--;
			cursor.camefrom = DOWN;
		}

		else if (cursor.left && cursor.camefrom != LEFT){
			cursor.y--;
			cursor.camefrom = RIGHT;
		}

		else if (cursor.down && cursor.camefrom != DOWN){
			cursor.x++;
			cursor.camefrom = UP;
		}

		else if (cursor.right && cursor.camefrom != RIGHT){
			cursor.y++;
			cursor.camefrom = LEFT;
		}
	}

	return 0;
}
