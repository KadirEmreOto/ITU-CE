#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define fi first
#define se second
#define MAXN 500
#define sz(a) ((int)a.size())

using namespace std;

typedef pair<int,int> ii;

int N,M,top,dfs_top;
int vis[MAXN][MAXN];
int dir[4][2]={{1,0},{0,1},{-1,0},{0,-1}};

ii st[MAXN*MAXN];
ii dfs[MAXN*MAXN];

char maze[][MAXN]={"#o###########################################",
				   "#                     #######################",
				   "# ################### #######################",
				   "# ################### ###                 ###",
				   "# ################### ### ############### ###",
				   "# ################### ##  ############### ###",
				   "#             ####### ## ################ ###",
				   "# ########### ####### ##             #### ###",
				   "# ########### #######     ########## #### ###",
				   "# ###########  ########## ##########   ## ###",
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

void write( bool finish ){

	if(finish)
		cout << endl << endl << endl << "A PATH HAS BEEN FOUND" << endl << endl << endl;
	else
		system("clear");

	for( int i=0 ; i<N ; i++,puts("") )
		for( int j=0 ; j<M ; j++ )
			printf("%c",maze[i][j]);

	system("sleep 0.05");
}

int main(){

	int stx,sty;

	M=strlen(maze[0]);
	N=sizeof(maze)/MAXN;

	for( int i=0 ; i<N ; i++ )
		for( int j=0 ; j<M ; j++ )
			if( maze[i][j]=='#' )
				vis[i][j]=2;
			else if( maze[i][j]=='o' )
				stx=i,sty=j;

	bool fl = false;

	dfs[++dfs_top] = ii(stx,sty);

	while( dfs_top ){

		int x=dfs[dfs_top].fi;
		int y=dfs[dfs_top--].se;

		vis[x][y]=1;
		st[++top]=ii(x,y);

		write(0);

		if( maze[x][y]=='E' ){
			fl = true;
			break;
		}

		maze[x][y]='o';

		for( int i=0 ; i<4 ; i++ ){

			int newx=x+dir[i][0];
			int newy=y+dir[i][1];

			if( 0<=newx && 0<=newy && newx<N && newy<M && !vis[newx][newy]  )
				dfs[++dfs_top] = ii(newx,newy);
		}

	}

	if( !fl ){
		system("clear");
		puts("\n\n\nTHERE IS NO PATH\n\n");
		return 0;
	}

	system("clear");
	for( int i=1 ; i<=top ; i++ )
		cout << st[i].fi << ' ' << st[i].se << endl;

	while(top){
		maze[st[top].fi][st[top].se]='o';
		top--;
	}

	write(1);
	return 0;
}
