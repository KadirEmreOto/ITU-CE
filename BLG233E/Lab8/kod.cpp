#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#define maxsize 10

using namespace std;
bool found = false;

struct node{
    bool right, left, down, up;
    bool visited;
	bool used;
    char val;

    node(){
        right = left = down = up = true;
        visited = false;
		used = false;
        val = ' ';
    }
} maze[maxsize][maxsize];

struct Pair{
    int i, j;
} *start, *finish;

struct Stack{
    Pair* array[maxsize * maxsize];
    int head;

    void create(){ head = 0; }
    bool is_empty(){ return (head == 0); }
    void push(Pair* n){ array[head++] = n; }
    Pair* pop(){ return (array[--head]); }
    void shuffle(){
        int r;
        for (int i=0; i < head-1; i++){
            r = rand() % head;
            swap(array[i], array[r]);
        }
    }
} s, all, ans;

int sum(int* ar, int size){
    int res = 0;
    for (int i=0; i < size; i++)
        res += ar[i];
    return res;
}

void create(int i, int j){
    if (i < 0 || i >= maxsize) return;
    if (j < 0 || j >= maxsize) return;

    maze[i][j].visited = true;

    int used[4], r;
    for (int i=0; i < 4; i++) used[i]=0;
    bool found = false;

    while (sum(used, 4) < 4){
        r = rand() % 4;
        used[r] = 1;

        if (r == 0 && i+1 < maxsize && !maze[i+1][j].visited){
            maze[i][j].down = false;
            maze[i+1][j].up = false;

            Pair* p = new Pair;
            p->i = i;
            p->j = j;
            s.push(p);

            create(i+1, j);
            found = true;
            break;
        }
        else if (r == 1 && i-1 > -1 && !maze[i-1][j].visited){
            maze[i][j].up = false;
            maze[i-1][j].down = false;

            Pair* p = new Pair;
            p->i = i;
            p->j = j;
            s.push(p);

            create(i-1, j);
            found = true;
            break;
        }
        else if (r == 2 && j+1 < maxsize && !maze[i][j+1].visited){
            maze[i][j].right = false;
            maze[i][j+1].left = false;

            Pair* p = new Pair;
            p->i = i;
            p->j = j;
            s.push(p);

            create(i, j+1);
            found = true;
            break;
        }
        else if (r == 3 && j-1 > -1 && !maze[i][j-1].visited){
            maze[i][j].left = false;
            maze[i][j-1].right = false;

            Pair* p = new Pair;
            p->i = i;
            p->j = j;
            s.push(p);

            create(i, j-1);
            found = true;
            break;
        }
    }

    if (!found && !s.is_empty()){
        Pair* p = s.pop();
        create(p->i, p->j);
    }

    else if (!found){
        while (!all.is_empty()) {
            Pair* p = all.pop();
            if (!maze[p->i][p->j].visited){
                create(p->i, p->j);
                break;
            }
        }
    }
}

void display(){
    system("clear");
    for (int i=0; i < maxsize; i++) {
        for (int j=0; j < maxsize; j++)
            printf(" %c", maze[i][j].up?'-':' ');
        printf("\n|");
        for (int j=0; j < maxsize; j++)
            printf("%c%c", maze[i][j].val, maze[i][j].right?'|':' ');
        printf("\n");
    }

    for (int i=0; i < maxsize; i++) {
        if (maze[maxsize-1][i].down)
            printf(" -");
    }
    printf("\n");
	system("sleep 0.01");
}

void DFS(int x, int y , int i, int j){
	if (maze[x][y].used) return;

	if (x == i && y == j) found = true;
	if (found) return;

	maze[x][y].used = true;
	maze[x][y].val = 'o';
	display();

	if (!maze[x][y].down) DFS(x + 1, y, i, j);
	if (!maze[x][y].up) DFS(x - 1, y, i, j);

	if (!maze[x][y].right) DFS(x, y + 1, i, j);
	if (!maze[x][y].left) DFS(x, y - 1, i, j);

	if (!found){
		maze[x][y].val = ' ';
		maze[x][y].used = false;
	}
}

int main(int argc, char const *argv[]) {
    srand((unsigned int)time(NULL));
    for (int i=0; i < maxsize; i++){
        for (int j=0; j < maxsize; j++) {
            Pair* p = new Pair;
            p->i = i;
            p->j = j;
            all.push(p);
        }
    }
    all.shuffle();

    create(0, 0);
    display();

	Pair* start = new Pair;
	Pair* finish = new Pair;

	printf("Başlangıç (x, y)> : ");
	scanf("%d", &(start->i));
	scanf("%d", &(start->j));

	printf("Bitiş (x, y)>: ");
	scanf("%d", &(finish->i));
	scanf("%d", &(finish->j));

	DFS(start->i, start->j, finish->i, finish->j);
	if (found){
		maze[finish->i][finish->j].val = 'o';
		display();
	}
	else
		printf("NO VALID SOLUTION\n");

    return 0;
}
