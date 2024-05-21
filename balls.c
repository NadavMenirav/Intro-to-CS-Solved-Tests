#include <stdio.h>
#include <string.h>

typedef enum { red, orange, yellow, green, blue } Ball;

typedef struct Player {
	char* name;
	int ID;
	Ball* myBalls;
	int numBalls;
} Player;

typedef struct PItem {
	Player* myP;
	PItem* next;
	PItem* prev;
} PItem;

typedef struct Game {
	PItem* players;
	PItem* endPlay;
	Ball* balls;
	int numBalls;
} Game;

Game* initGame();
Player* newPlayer(const char* name, int id, Ball ball[], int numballs);
int addPlayer(Game* game, Player* player);
int addBallsToBag(Game* game, Player* player);
int moveBalls(Game* game, Player* player, int x);
int removePlayer(Game* game, Player* player);
PItem* isPlayerInGame(Game* game, Player* player);
int playerCountInGame(Game* game);
int startPlay(Game* game);


int main() {
	return 0;
}

Game* initGame() {
	//The function allocs a game and puts default values in it
	Game* game = (Game*)malloc(sizeof(game));
	game->players = NULL;
	game->endPlay = NULL;
	game->balls = NULL;
	game->numBalls = 0;
	return game;
}

Player* newPlayer(const char* name, int id, Ball ball[], int numballs) {
	//The function creates a new player
	Player* player = (Player*)malloc(sizeof(Player));
	player->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
	strcpy(player->name, name);
	
	player->ID = id;
	player->numBalls = numballs;

	player->myBalls = (Ball*)malloc(numballs * sizeof(Ball));

	for (int i = 0; i < numballs; i++) 
		player->myBalls[i] = ball[i];
	
	return player; 
}

int addPlayer(Game* game, Player* player) {
	//The function gets a player and adds it to the end of the player list
	PItem* curFirst = game->players;
	PItem* curLast = NULL;
	PItem* p = game->players;

	if (curFirst != NULL) { //Checking if there is another player with the same ID 
		do {
			if (!strcmp(p->myP->name, player->name))
				return -1;
			p = p->next;
		} while (p != curFirst); 
	}


	PItem* playerItem = (PItem*)malloc(sizeof(PItem));
	playerItem->myP = (Player*)malloc(sizeof(Player));
	playerItem->myP = player;

	playerItem->next = (PItem*)malloc(sizeof(PItem)); //To-Do: Does it need malloc??
	playerItem->prev = (PItem*)malloc(sizeof(PItem));

	if (curFirst == NULL) {
		playerItem->next = playerItem;
		playerItem->prev = playerItem;
		game->players = playerItem;	
		return addBallsToBag(game, player);
	}
	
	playerItem->next = curFirst;	

	curLast = curFirst->prev;
	playerItem->prev = curLast;

	curLast->next = playerItem;
	curFirst->prev = playerItem;

	return addBallsToBag(game, player);
}

int addBallsToBag(Game* game, Player* player) {
	int curNumBalls = game->numBalls;
	int newNumBalls = curNumBalls + player->numBalls;

	game->balls = (Ball*)realloc(game->balls, newNumBalls * sizeof(Ball));

	for (int i = 0; i < player->numBalls; i++) {
		game->balls[i + curNumBalls] = player->myBalls[i];
	}
	game->numBalls = newNumBalls;

	free(player->myBalls);
	player->myBalls = NULL;
	player->numBalls = 0;	
	return newNumBalls;
}

int moveBalls(Game* game, Player* player, int x) {
	//The function is a play of the game, the player picks how many balls to take from the game bag to his bag
	int newNumBallsGame, newNumBallsPlayer;
	Ball* newBallsInGame = NULL;
	if (!game || !player || x < 0)
		return -1;

	if (x == 0)
		return game->numBalls;

	if (game->numBalls < x)
		return -1;

	newNumBallsPlayer = player->myBalls + x;
	newNumBallsGame = game->balls - x;

	player->myBalls = (Ball*)realloc(player->myBalls, newNumBallsPlayer * sizeof(Ball));
	for (int i = 0; i < x; i++) {
		player->myBalls[i + player->numBalls] = game->balls[game->numBalls - i];
	}

	if (newNumBallsGame > 0) {
		newBallsInGame = (Ball*)malloc(newNumBallsGame * sizeof(Ball));
		for (int i = 0; i < newNumBallsGame; i++) {
			newBallsInGame[i] = game->balls[i];
		}
	}

	free(game->balls);
	game->balls = newBallsInGame;
	
	player->numBalls = newNumBallsPlayer;
	game->numBalls = newNumBallsGame;
	return newNumBallsGame;
}

int removePlayer(Game* game, Player* player) {
	//The function takes a player out of the game
	PItem* playerItem = isPlayerInGame(game, player);

	if (!playerItem)
		return -1;

	if (playerCountInGame == 1) {
		free(game->players);
		game->players = NULL;
		return 0;
	}


	playerItem->prev->next = playerItem->next;
	playerItem->next->prev = playerItem->prev;

	if (playerItem == game->players)
		game->players = game->players->next;

	playerItem->next = game->endPlay;
	playerItem->prev = NULL;
	if (game->endPlay)
		game->endPlay->prev = playerItem;

	game->endPlay = playerItem;
	return playerCountInGame(game);
}

PItem* isPlayerInGame(Game* game, Player* player) {
	//The function checks if a player is in the game. If he is it returns his playerItem, if he isn't it returns NULL
	PItem* returnP = NULL;
	if (game->players == NULL)
		return NULL;

	returnP = game->players;
	do {
		if (returnP->myP->ID == player->ID)
			return returnP;
		returnP = returnP->next;
	} while (returnP != game->players);

	return NULL;
}

int playerCountInGame(Game* game) {
	if (!game->players)
		return 0;
		
	PItem* p = game->players;
	int count = 0;
	do {
		count++;
		p = p->next;
	} while (p != game->players);
	return count;
}

int startPlay(Game* game) {
	int ballCountChoice = 0, i = 0;
	PItem* playerItemTurn = game->players;
	while (game->players && game->numBalls) {
		for (int j = 0; j < i; j++) //Getting the current player
			playerItemTurn = playerItemTurn->next;

		printf("How many balls do you want?\n");
		scanf("%d", &ballCountChoice);
		if (ballCountChoice > game->numBalls) {
			removePlayer(game, playerItemTurn->myP);
			continue;
		}

		moveBalls(game, playerItemTurn->myP, ballCountChoice);
		i++;
	}
	return game->numBalls;
}