#include <iostream>
#include <cstdlib>

bool found = false;
int count = 0;
int stack[1000][2];

char lab[15][25] = {
	"#o###################",
	"#     #           # #",
	"##### # ###### ## # #",
	"# # #          #  # #",
	"# # ############ ## #",
	"#     # # ##   #    #",
	"### # # # ## # # ####",
	"# # # # #    #      #",
	"# # # # # ######### #",
	"# # # # # #       # #",
	"# # #   # # #####   #",
	"# # ##### # #   #####",
	"#         #   #     #",
	"###################E#"
};

void display(){
	system("clear");
	for (int i = 0; i < 14; i++){
		for (int j = 0; j < 24; j++){
			if (lab[i][j] == '#')
				printf("⬛️ ");
			else if (lab[i][j] == 'o')
				printf("😁 ");
			else if (lab[i][j] == 'E')
				printf("✅ ");
			else
				printf("  ");
		}
		printf("\n");
	}
	system("sleep 0.05");
}

void DFS(int x, int y){
	if (lab[x][y] == '#') return;
	if (lab[x][y] == 'o') return;
	if (lab[x][y] == 'E') found = true;
	if (found) return;

	lab[x][y] = 'o';
	display(); printf("Count: %d\n", count);

	if (x + 1 < 14) DFS(x + 1, y);
	if (x - 1 >= 0) DFS(x - 1, y);

	if (y + 1 < 24) DFS(x, y + 1);
	if (y - 1 >= 0) DFS(x, y - 1);

	if (!found){
		lab[x][y] = ' ';
	}
}

int main(int argc, char const *argv[]) {
	lab[0][1] = ' ';
	DFS(0, 1);

	display();
	printf("# PATH #\n");
	for (int i = 0; i < count; i++)
		printf("(%d, %d)\n", stack[i][0], stack[i][1]);

	return 0;
}
