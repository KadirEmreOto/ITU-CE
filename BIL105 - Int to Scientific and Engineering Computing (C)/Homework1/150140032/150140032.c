
// Author Name : Kadir Emre Oto
// Student ID  : 150140032
// E-mail	   : otok@itu.edu.tr

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	int N, P, W, S;

	printf("$ Enter number of players per team (N)\n$ ");
	scanf("%d", &N);

	printf("$ Enter probability for a successful pass (P)\n$ ");
	scanf("%d", &P);

	printf("$ Enter number of passes to win a single round (W)\n$ ");
	scanf("%d", &W);

	printf("$ Enter target score (S)\n$ ");
	scanf("%d", &S);

	int score1=0 , score2=0, team=0;				// team scores and selected team
	int rnumber=0, pcount=0;						// round number, succesfully pass count
	int cplayer, nplayer;							// current player, next player
	int chance;										// chance for a successful pass
	int isFirst;									// is the current player first on the round? 1 is yes, 0 is no.

	srand(time(NULL)); 								// to get different random numbers everytime

	while (score1 < S & score2 < S) {
		isFirst = 1; 								// set the current player first before round starts
		rnumber += 1;								// increse round number

		pcount = 0; 								// reset pass count for new round
		team = rand() % 2;							// select team randomly.
													// if it is 0, it means team1 is selected, otherwise team2 is selected
		printf("Round-%d:\n", rnumber); 
		printf("\tTeam%d is selected", team+1); 

		cplayer = rand() % N; 						// select current player randomly
		nplayer = cplayer;   						// set next player as current player

		if (pcount == W-1) {						// if W is 1 
			printf("\n\tPlayer%d", cplayer + 1);
		}

		while (pcount != W-1){						// continue until the pass count reaches W
			if (isFirst == 1)						// check if the current player is first
				printf("\n\tPlayer%d", cplayer + 1);

			while (cplayer == nplayer){				// if current player is selected as next player
				nplayer = rand() % N;				// -> change 
			}

			chance = rand() % 101;					// generate a random number between 0 and 100 (inclusive)

			if (chance < P) {						// if the current player can pass the ball successfully with a probability of P%
				isFirst = 0;						// set isFirst 0 
				pcount += 1;						// increase succesfully pass count
				cplayer = nplayer;					// and now the next player is the current player

				printf(" -> Player%d", cplayer + 1);
			} else{									// else if opponent team catches the ball
				pcount = 0;							// reset pass count because opponent team catches the ball
				team += 1;							// change the current team
				team %= 2;

				isFirst = 1;						// set the player first
				cplayer = rand() % N;				// select new current player for oppenent team

				printf("\n\tTeam%d captured the ball!", team + 1);
			}
		}

		if (team == 0){								
			score1 += 1;							// increase the score of team 1
			printf("\n\tSuccess! New Score of Team1 is %d\n\n", score1);
		}else{
			score2 += 1;							// increase the score of team 2
			printf("\n\tSuccess! New Score of Team2 is %d\n\n", score2);
		}
	}

	printf("GAME OVER: Team%d reached the target score (%d) and won the game.\n", team+1, S);
	return 0;
}