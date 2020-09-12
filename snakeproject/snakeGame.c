

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 25 //The number of rows
#define COLS 75 //The number of collumns
#define MAX_COUNT 5 //Counter for when to add a new snake node
#define SPEED_REDUCTION 0.03 //Amout of speed reduction

typedef struct snakeNode
{
	int x;
	int y;
	struct snakeNode* next;
}SNAKE_NODE;

void init(void);

void gotoxy(int x, int y);

void sleep(float secs);

/*
This function draws the edges of the board
*/
void drawBoard();

/*
This function gets a SNAKE_NODE input
the function prints '@' at its x and y parameters
*/
void printHead(SNAKE_NODE* head);

/*
This function gets a SNAKE_NODE input
the function prints ' ' at its x and y parameters
*/
void printBlank(SNAKE_NODE* head);

/*
This function gets intigers x and y and a SNAKE_NODE inputs
the funcion creates a new SNAKE_NODE with the given parameters and returns it
*/
SNAKE_NODE* createNewNode(int x, int y, SNAKE_NODE* next);

/*
This function gets SNAKE_NODEs head and node inputs
the function add the node to the snake and returns it
*/
SNAKE_NODE* addNodeToTail(SNAKE_NODE* head, SNAKE_NODE* node);

/*
Thsi function gets a SNAKE_NODE, char ch, int count, and bool* contGame inputs,
this function adds a new node according to 'ch'
and if needed removes the last node
*/
SNAKE_NODE* snakeChange(SNAKE_NODE* head, char ch, int count, bool* contGame);

/*
This function gets a SNAKE_NODE input
the function return true if the snake represented with the node collided with it self or hit the edges,
false otherwise
*/
bool isLost(SNAKE_NODE* head);

/*
This function gets a SNAKE_NODE input
the function returns true if the snake collided with it self,
false otherwise
*/
bool snakeCollide(SNAKE_NODE* head);

/*
This function gets SNAKE_NODE input
the function remove the last node of the chain 
*/
void removeLastNode(SNAKE_NODE* head);

/*
This function gets SNAKE_NODE input
the function free all the dinamiclly created nodes and prints GAME OVER
*/
void gameOver(SNAKE_NODE* head);

/*
This function gets SNAKE_NODE input
the function free all the chain of nodes that was dinamiclly created
*/
void freeAll(SNAKE_NODE* head);

/*
This function gets double* speed as an input
the function removes 0.03 from speed if it is above 0.1
*/
void speedChange(double* speed);

int main()
{
	init();
	//initialize board and snake
	drawBoard();
	SNAKE_NODE *head = createNewNode(12, 37, NULL);
	//initialize parameters
	int count = 1;
	double speed = 1.00;
	bool continueGame = true;
	char ch = 'd';
	//game
	while(continueGame)
	{
		if (_kbhit())
			ch = _getch();
		printHead(head);
		sleep(speed);
		head = snakeChange(head, ch, count, &continueGame);
		count++;
		if (count == MAX_COUNT)
		{
			speedChange(&speed);
			count = 0;
		}
	} //while
	gameOver(head);
}

void gameOver(SNAKE_NODE* head)
{
	printBlank(head);
	freeAll(head);
	gotoxy(ROWS / 2, COLS / 2 - 5);
	printf("GAME OVER!");
	gotoxy(ROWS + 1, COLS + 1);
}

void freeAll(SNAKE_NODE* head)
{
	SNAKE_NODE* next;

	while (head != NULL)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

void speedChange(double* speed)
{
	if (*speed > 0.1)
		*speed -= SPEED_REDUCTION;
}
void init(void) {
	system("cls");
}

void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

void sleep(float secs) {
	clock_t clocks_start = clock();
	while (clock() - clocks_start < secs*CLOCKS_PER_SEC);
}

void drawBoard()
{
	int i, j;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			if (i == 0 || i == ROWS - 1)
				printf("#");
			else
			{
				if (j == 0 || j == COLS - 1)
				{
					printf("#");
				}
				else
					printf(" ");
			}
			if (j == COLS - 1)
				printf("\n");
		}
	}
}

void printHead(SNAKE_NODE *head)
{
	gotoxy(head->x, head->y);
	printf("@");
}

SNAKE_NODE* addNodeToTail(SNAKE_NODE* head, SNAKE_NODE* node)
{
	node->next = head;
	return node;
}

void printBlank(SNAKE_NODE *head)
{
	SNAKE_NODE* node = head->next;
	while (node != NULL)
	{
		gotoxy(node->x, node->y);
		printf(" ");
		node = node->next;
	}
}

SNAKE_NODE* createNewNode(int x, int y, SNAKE_NODE* next)
{
	SNAKE_NODE* node = (SNAKE_NODE*)malloc(sizeof(SNAKE_NODE));

	node->x = x;
	node->y = y;
	node->next = next;

	return node;
}

SNAKE_NODE* snakeChange(SNAKE_NODE* head, char ch, int count, bool* contGame)
{
	switch (ch)
	{
	case 'w':
		head = addNodeToTail(head, createNewNode((head->x) - 1, head->y, head));
		break;
	case 's':
		head = addNodeToTail(head, createNewNode((head->x) + 1, head->y, head));
		break;
	case 'a':
		head = addNodeToTail(head, createNewNode(head->x, (head->y) - 1, head));
		break;
	case 'd':
		head = addNodeToTail(head, createNewNode(head->x, (head->y) + 1, head));
		break;
	}

	*contGame = !isLost(head);

	if (count % MAX_COUNT != 0)
	{
		removeLastNode(head);
	}

	return head;
}

void removeLastNode(SNAKE_NODE* head)
{
	SNAKE_NODE* current = head->next;
	SNAKE_NODE* previous = head;

	while (current->next != NULL)
	{
		previous = current;
		current = current->next;
	}

	gotoxy(current->x, current->y);
	printf(" ");
	free(current);
	previous->next = NULL;
}

bool isLost(SNAKE_NODE* head)
{
	if (head->y == 1 || head->y == COLS || head->x == 1 || head->x == ROWS)
		return true;
	if (snakeCollide(head))
		return true;
	return false;
}

bool snakeCollide(SNAKE_NODE* head)
{
	SNAKE_NODE* current = head;

	while (current->next != NULL)
	{
		if ((head->x == current->next->x) && (head->y == current->next->y))
		{
			return true;
		}

		current = current->next;
	}
	return false;
}
