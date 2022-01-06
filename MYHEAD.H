 /* header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <alloc.h>

/* symbolic constants */
#define BKCOLOR 0
#define FRCOLOR 1

struct menu
{
	int row, col;
	int bcol_hlt, fcol_hlt;
	int bcol_nhlt, fcol_nhlt;
	int nitems;
	int length;
	int steps;
};
typedef struct menu MENU;

struct window
{
	int topLeftX, topLeftY;
	int botRightX, botRightY;
	int bcol, fcol;
	char *screen, *header;
};
typedef struct window WINDOW;

/* global prototypes */
void 	checkHighScore(void);
void 	checkLine(void);
void 	clearscreen(int, int);
int 	drawBrick(int, int, int, int, int);
void 	drawStage(void);
int 	gamePlay(void);
void 	getName(void);
void 	Help(void);
void 	printHighScore(void);
int 	Random(void);
void	setScreen(int);
void 	writes(int, int, int, int, char *);
void    storeScreen(char **, int, int, int, int);
void 	restoreScreen(WINDOW *);
void 	Window(WINDOW *pwin);
void 	highlight(MENU *, int, int);