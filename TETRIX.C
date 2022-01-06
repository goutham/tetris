/*
 * PROGRAM:  SUPER TETRIX v1.3
 * LANGUAGE: C
 * AUTHOR:   Goutham P. Bhat
 */

/* header */
#include "myhead.h"

/* global variables */
char			name[20];
unsigned long int 	score = 0, lines = 0;
float 			interval = 9.0;
WINDOW 			Score, Lines, nextB;

/* reservoir of shape definitions */
long int Brick[4][4] =
	{
		{ 0X00C0A000, 0XB3778000, 0X3C5BC000, 0X04020000 },
		{ 0X41A82000, 0XEFEF2000, 0X7F18E000, 0X50000000 },
		{ 0X00800000, 0X88254000, 0X02144000, 0X00000000 },
		{ 0X00000000, 0X80000000, 0X00000000, 0X00000000 }
	};

/* phases of each kind of brick */
int phases[] = { 2, 1, 2, 2, 4, 4, 4 };

/* offset for comparer in drawBrick() */
int offsets[] = { 0, 2, 3, 5, 7, 11, 15 };

/* colors for each kind of brick */
int colors[] = { 2, 3, 6, 6, 4, 5, 5 };

/*-*-*/
void main(void)
{
	char fname[20], key; 	/* filename, key press */
	int game, ch, i;
	MENU Main;              /* Main is a MENU type to display main menu */

	_setcursortype(0); 	/* set cursor type to _NOCURSOR */

	for(;;)			/* begin invincible for loop */
	{
		clearscreen(1, 25); 	/* clear whole screen */
		writes(1, 1, 4, 11, "                               SUPER TETRIX v1.3                                "); /* write game name */
		/* menu items */
		writes(3, 34, 1, 14, " Play game  ");
		writes(4, 34, 1, 14, "            ");
		writes(5, 34, 1, 14, " High score ");
		writes(6, 34, 1, 14, "            ");
		writes(7, 34, 1, 14, " Help screen");
		writes(8, 34, 1, 14, "            ");
		writes(9, 34, 1, 14, " Quit game  ");

		/* Main's attributes */
		Main.row = 3;
		Main.col = 34;
		Main.steps = 2;
		Main.length = 12;
		Main.nitems = 4;
		Main.bcol_hlt = 4;
		Main.fcol_hlt = 11;
		Main.bcol_nhlt = 1;
		Main.fcol_nhlt = 14;

		/* get User's choice... */
		ch = menuNavigator(&Main);

		/* ...and do accordingly */
		switch(ch)
		{
			case 1:
				getName(); 	 /* get User's name and break */
				break;

			case 2:
				printHighScore(); /* display high score and continue */
				continue;

			case 3:
				Help(); 	  /* show help screen and continue */
				continue;

			case 4:
				clearscreen(2, 25);	/* clear from line 2 to line 25 */
				writes(3, 32, 6, 11, " Goutham P. Bhat "); /* NAME of AUTHOR */
				getch();
				clearscreen(1, 25); 	/* clear whole screen */
				_setcursortype(2); 	/* set curor type to _NORMALCUROSR */
				exit(0); 		/* exit program */
		}

		for(;;) 	/* the gameplay loop */
		{
			game = gamePlay(); 	/* start game */

			if(game == 1)	 	/* if pit is full */
			{
				WINDOW win;
				MENU ask;

				checkHighScore(); /* check and see if this is high score */

				/* win window attributes */
				win.topLeftX = 29;
				win.topLeftY = 10;
				win.botRightX = 52;
				win.botRightY = 16;
				win.bcol = 0;
				win.fcol = 14;
				win.header = "";
				Window(&win);

				gotoxy(31, 11);
				printf("GAME OVER");
				gotoxy(31, 12);
				printf("Continue?");
				gotoxy(39, 13);
				printf(" Yes ");
				gotoxy(39, 14);
				printf(" No ");

				/* ask menu attributes */
				ask.row = 13;
				ask.col = 39;
				ask.bcol_hlt = 4;
				ask.fcol_hlt = 10;
				ask.bcol_nhlt = 0;
				ask.fcol_nhlt = 14;
				ask.nitems = 2;
				ask.length = 5;
				ask.steps = 1;

				ch = menuNavigator(&ask); 	/* get User's choice */

				switch(ch)
				{
					case 1: 	/* if User wants to continue */
						restoreScreen(&Score);
						restoreScreen(&Lines);
						restoreScreen(&nextB);
						score = 0;
						lines = 0;
						interval = 9.0;
						clearscreen(1, 25);
						continue;

					case 2: 	/* if doesn't want to continue */
						restoreScreen(&Score);
						restoreScreen(&Lines);
						restoreScreen(&nextB);
						clearscreen(1, 25);
						score = 0;
						lines = 0;
						interval = 9.0;
						break;
				}
				break;
			}

			else if(game == 2) 	/* if User voluntarily wants to quit */
			{
				checkHighScore();
				restoreScreen(&Score);
				restoreScreen(&Lines);
				restoreScreen(&nextB);
				clearscreen(1, 25);
				score = 0;
				lines = 0;
				interval = 9.0;
				break;
			}
		}
	}
}

/* checks if the current score is high score */
void checkHighScore(void)
{
	char fname[20];
	unsigned long sc;
	FILE *fp;

	fp = fopen("HSCORE.STX", "r");
	if(fp == NULL)
	{
		fp = fopen("HSCORE.STX", "w");
		fprintf(fp, "%s\n%lu\n%lu", "NONAME", 0L, 0L);
		fclose(fp);
		fp = fopen("HSCORE.STX", "r");
	}

	fgets(fname, 25, fp);
	fscanf(fp, "%lu", &sc);
	if(score >= sc && score != 0)
	{
		char key;
		WINDOW Win;
		Win.topLeftX = 29;
		Win.topLeftY = 10;
		Win.botRightX = 52;
		Win.botRightY = 16;
		Win.bcol = 0;
		Win.fcol = 14;
		Win.header = "";
		Window(&Win);
		writes(11, 31, 0, 11, "You've got a High");
		writes(12, 31, 0, 11, "Score");
		writes(13, 31, 0, 11, "Press SPACE BAR . . .");
		for(;;)
		{
			key = getch();
			if(key == 32)
				break;
		}
		restoreScreen(&Win);
		fclose(fp);
		fp = fopen("HSCORE.STX", "w+");
		fprintf(fp, "%s\n%lu\n%lu", name, score, lines);
		fclose(fp);
		printHighScore();
	}
}

/* checks if a line is formed and if yes, deletes the line */
void checkLine(void)
{
	char far *vdu = (char far *)0xB8000000;
	int i, j, flag;

	for(i = 1; i < 23; i ++)
	{
		flag = 0;
		for(j = 25; j < 55; j ++)
		{
			if(*(vdu + i * 160 + j * 2 + 1) == BKCOLOR << 4)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0)
		{
			for(j = 25; j < 55; j ++)
			{
				*(vdu + i * 160 + j * 2 + 1) = BKCOLOR << 4;
				delay(20);
			}
			score += 100;
			lines ++;
			if(lines != 0 && lines % 10 == 0 && interval > 0)
				interval -= 0.9;
			if(interval < 0)
				interval = 0;
			setScreen(i);
		}
	}
}

/* clears screen from start row to end row */
void clearscreen(int start, int end)
{
	char far *vdu = (char far *)0xB8000000;
	int i, j;
	gotoxy(1, start);
	-- start;
	-- end;
	start *= 160;
	end *= 160;

	for(i = start; i <= end; i += 160)
	{
		for(j = 0; j < 160; j += 2)
		{
			*(vdu + i + j) = 0;
			*(vdu + i + j + 1) = 7;
		}
	}
}

/* draws a brick specified by 'kindabrick' in phase specified by 'phase' with 'color' at x, y */
int drawBrick(int kindabrick, int phase, int color, int x, int y)
{
	int offs, i, j;
	long int comparer;
	char far *vdu = (char far *)0xB8000000;

	offs = offsets[kindabrick - 1] + phase;
	comparer = 1L << (12 + 19 - offs); 	/* value that specifies shape of brick */

	if(color != BKCOLOR) 			/* if color != BKCOLOR */
		/* checks if the brick can occupy current position properly */
		for(i = 0; i < 4; i ++)
			for(j = 0; j < 8; j += 2)
				if(((Brick[i][j / 2] & comparer) == comparer) && *(vdu + i * 160 + j * 2 + (x - 1) * 2 + (y - 1) * 160 + 1) != BKCOLOR << 4 || y >= 25)
					return 1;
	/* draws brick on screen */
	for(i = 0; i < 4; i ++)
		for(j = 0; j < 8; j += 2)
			if((Brick[i][j / 2] & comparer) == comparer)
				*(vdu + i * 160 + j * 2 + (x - 1) * 2 + (y - 1) * 160 + 1) = color << 4,
				*(vdu + i * 160 + j * 2 + (x - 1) * 2 + (y - 1) * 160 + 3) = color << 4;
	return 0;
}

/* draws the tetrix pit */
void drawStage(void)
{
	char far *vdu = (char far *)0xB8000000;
	int i, j;

	for(i = 24; i < 56; i ++)
		*(vdu + i * 2 + 1) = FRCOLOR << 4;

	for(i = 24; i < 56; i ++)
		*(vdu + i * 2 + 23 * 160 + 1) = FRCOLOR << 4;

	for(i = 1; i < 23; i ++)
			*(vdu + i * 160 + 24 * 2 + 1) = *(vdu + i * 160 + 55 * 2 + 1) = FRCOLOR << 4;

	for(i = 1; i < 23; i ++)
		for(j = 25; j < 55; j ++)
			*(vdu + i * 160 + j * 2 + 1) = BKCOLOR << 4;
}

/* tetrix game play begins with this function */
int gamePlay(void)
{
	char key;
	char far *vdu = (char far *)0xB8000000;
	int x, y, r, phase, dec, ph, color, xx, yy, rr = 0, i, j, offs;
	long int comparer;
	clock_t start, end; 		/* clock variables */

	_setcursortype(0); 		/* _NOCURSOR */
	clearscreen(1, 25); 		/* clear whole screen */
	randomize(); 			/* randomize random */
	drawStage(); 			/* draw tetrix pit */

	/* Score window attributes */
	Score.topLeftX = 60;
	Score.topLeftY = 2;
	Score.botRightX = 76;
	Score.botRightY = 5;
	Score.bcol = 1;
	Score.fcol = 11;
	Score.header = " SCORE ";
	Window(&Score);

	/* Lines window attributes */
	Lines.topLeftX = 60;
	Lines.topLeftY = 7;
	Lines.botRightX = 76;
	Lines.botRightY = 10;
	Lines.bcol = 1;
	Lines.fcol = 11;
	Lines.header = " LINES ";
	Window(&Lines);

	/* next brick window attributes */
	nextB.topLeftX = 4;
	nextB.topLeftY = 2;
	nextB.botRightX = 20;
	nextB.botRightY = 10;
	nextB.bcol = 0;
	nextB.fcol = 11;
	nextB.header = " NEXT ";
	Window(&nextB);

	for(;;) 			/* game in motion */
	{
		if(!rr)
			r = Random(); 	/* get a random brick */
		else
			r = rr;
		x = 38;
		y = 2;
		phase = 0;

		rr = Random();
		xx = 9;
		yy = 5;

		offs = offsets[rr - 1];
		comparer = 1L << (12 + 19 - offs);

		for(i = 0; i < 4; i ++)
			for(j = 0; j < 8; j += 2)
				if((Brick[i][j / 2] & comparer) == comparer)
					*(vdu + i * 160 + j * 2 + (xx - 1) * 2 + (yy - 1) * 160 + 1) = colors[rr - 1] << 4,
					*(vdu + i * 160 + j * 2 + (xx - 1) * 2 + (yy - 1) * 160 + 3) = colors[rr - 1] << 4;
				else
					*(vdu + i * 160 + j * 2 + (xx - 1) * 2 + (yy - 1) * 160 + 1) = 0,
					*(vdu + i * 160 + j * 2 + (xx - 1) * 2 + (yy - 1) * 160 + 3) = 0;

		dec = drawBrick(r, 0, colors[r - 1], x, y); /* draw brick on screen */

		if(dec) /* if brick is restrained from being created, pit is full */
			return 1;

		start = clock(); 	/* clock starts */
		for(;;)
		{
			gotoxy(62, 3);
			printf("%13d", score);
			gotoxy(62, 8);
			printf("%13d", lines);

			end = clock(); 	/* clock ends */

			if((end - start) > interval) /* if its time to move */
			{
				drawBrick(r, phase, BKCOLOR, x, y);	/* BLACK brick */
				start = clock(); 			/* clock starts */
				y ++; 					/* move vertically down */
				dec = drawBrick(r, phase, colors[r - 1], x, y); /* draw it */

				if(dec) 	/* if brick cannot be moved */
				{
					drawBrick(r, phase, colors[r - 1], x, y - 1); /* draw brick in previous position */
					checkLine(); 	/* check if line is formed */
					break;
				}
			}

			if(kbhit()) 		/* if button is pressed */
			{
				key = getch();
				if(key == 27) 	/* if ESC */
				{
					WINDOW win;
					MENU ask;
					/* display menu */
					win.topLeftX = 29;
					win.topLeftY = 10;
					win.botRightX = 52;
					win.botRightY = 15;
					win.bcol = 2;
					win.fcol = 14;
					win.header = "";
					Window(&win);
					gotoxy(31, 11);
					printf("Quit?");
					gotoxy(39, 12);
					printf(" Yes ");
					gotoxy(39, 13);
					printf(" No ");

					ask.row = 12;
					ask.col = 39;
					ask.bcol_hlt = 6;
					ask.fcol_hlt = 10;
					ask.bcol_nhlt = 2;
					ask.fcol_nhlt = 14;
					ask.nitems = 2;
					ask.length = 5;
					ask.steps = 1;
					ph = menuNavigator(&ask); /* get User's choice */

					if(ph == 1)
					{
						restoreScreen(&win);
						return 2;
					}
					else
					{
						restoreScreen(&win);
						continue;
					}
				}

				else if(key == 9)	/* if TAB, drop brick at once */
				{
					dec = 0;
					while(!dec)
					{
						drawBrick(r, phase, BKCOLOR, x, y);
						y ++;
						score ++;
						dec = drawBrick(r, phase, colors[r - 1], x, y);
					}
					drawBrick(r, phase, colors[r - 1], x, y - 1);
					checkLine();
					break;
				}

				else if(key == 0) 	/* if 'other' keys */
				{
					key = getch();
					if(key == 77) 	/* RIGHT */
					{
						drawBrick(r, phase, BKCOLOR, x, y);
						dec = drawBrick(r, phase, colors[r - 1], x + 2, y);
						if(dec)
						{
							drawBrick(r, phase, colors[r - 1], x, y);
							continue;
						}
						x += 2;
					}

					else if(key == 75) 	/* LEFT */
					{
						drawBrick(r, phase, BKCOLOR, x, y);
						dec = drawBrick(r, phase, colors[r - 1], x - 2, y);
						if(dec)
						{
							drawBrick(r, phase, colors[r - 1], x, y);
							continue;
						}
						x -= 2;
					}

					else if(key == 72) 	/* UP to change phase */
					{
						ph = phase;
						drawBrick(r, phase, BKCOLOR, x, y);
						if(phase < phases[r - 1] - 1)
							phase ++;
						else phase = 0;
						dec = drawBrick(r, phase, colors[r - 1], x, y);
						if(dec)
						{
							drawBrick(r, ph, colors[r - 1], x, y);
							phase = ph;
							continue;
						}
					}

					else if(key == 80) 	/* DOWN to sink rapidly */
					{
						drawBrick(r, phase, BKCOLOR, x, y);
						y ++;
						score ++;
						dec = drawBrick(r, phase, colors[r - 1], x, y);
						if(dec)
						{
							drawBrick(r, phase, colors[r - 1], x, y - 1);
							checkLine();
							break;
						}
					}
				}
			}
		}
	}
}

/* gets player name */
void getName(void)
{
	char key;
	int i;
	WINDOW win;
	clearscreen(2, 25);

	win.topLeftX = 2;
	win.topLeftY = 3;
	win.botRightX = 41;
	win.botRightY = 5;
	win.bcol = 2;
	win.fcol = 11;
	win.header = "";
	Window(&win);

	_setcursortype(2);
	gotoxy(4, 4);
	printf("Enter your name: ");
	i = 0;
	for(;;)
	{
		key = getch();
		if(key == 8 && i > 0)
		{
			i --;
			gotoxy(wherex() - 1, wherey());
			printf("%c", 0);
			gotoxy(wherex() - 1, wherey());
			name[i] = '\0';
			continue;
		}
		else if(key == 8) continue;
		else if(i >= 19 && (key == '\n' || key == '\r'))
		{
			name[i] = '\0';
			break;
		}
		else if(i >= 19)
		{
			name[i] = '\0';
			continue;
		}
		else if(key == '\n' || key == '\r')
			break;
		else if((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || key == '.' || key == ' ');
		else continue;
		printf("%c", key);
		name[i ++] = key;
		name[i] = '\0';
	}
	restoreScreen(&win);

	flushall();
	_setcursortype(0);
}

/* help screen */
void Help(void)
{
	char key;
	WINDOW win;

	win.topLeftX = 1;
	win.topLeftY = 2;
	win.botRightX = 80;
	win.botRightY = 25;
	win.bcol = 0;
	win.fcol = 11;
	win.header = " HELP ";
	Window(&win);

	gotoxy(4, 3);
	printf("In this game, you have to arrange the bricks falling into the");
	gotoxy(4, 4);
	printf("pit in such a way that continuous horizontal lines are formed.");
	gotoxy(4, 5);
	printf("When a horizontal line is formed, it is deleted. When the pit");
	gotoxy(4, 6);
	printf("is occupied upto the top, the game is over. Speed of falling");
	gotoxy(4, 7);
	printf("bricks increases after every 10 line deletions.");
	writes(9, 4, 2, 4, "CONTROLS:");
	writes(11, 4, 0, 10, "LEFT arrow    -   Move brick to left");
	writes(12, 4, 0, 10, "RIGHT arrow   -   Move brick to right");
	writes(13, 4, 0, 10, "UP arrow      -   Rotate brick");
	writes(14, 4, 0, 10, "DOWN arrow    -   Sink the brick down");
	writes(15, 4, 0, 10, "TAB key       -   Sink the brick rapidly");
	writes(16, 4, 0, 10, "ESC key       -   'Quit or continue' window");
	writes(18, 4, 1, 11, "Press SPACE BAR to continue . . .");
	for(;;)
	{
		key = getch();
		if(key == 32)
			break;
	}
	restoreScreen(&win);
}

/* prints high score */
void printHighScore(void)
{
	char key;
	unsigned long int sc, li;
	FILE *fp;
	WINDOW win;

	fp = fopen("HSCORE.STX", "r");
	if(fp == NULL)
	{
		fp = fopen("HSCORE.STX", "w");
		fprintf(fp, "%s\n%lu\n%lu", "NONAME", 0L, 0L);
		fclose(fp);
		fp = fopen("HSCORE.STX", "r");
	}
	fgets(name, 25, fp);
	fscanf(fp, "%lu%lu", &sc, &li);

	win.topLeftX = 2;
	win.topLeftY = 3;
	win.botRightX = 30;
	win.botRightY = 13;
	win.bcol = 6;
	win.fcol = 10;
	win.header = " HIGH SCORE ";
	Window(&win);

	writes(4, 5, 6, 10, "Name: ");
	gotoxy(11, 4);
	printf("%s", name);
	writes(6, 5, 6, 10, "Score: ");
	gotoxy(12, 6);
	printf("%lu", sc);
	writes(8, 5, 6, 10, "Lines: ");
	gotoxy(12, 8);
	printf("%lu", li);
	writes(10, 5, 6, 10, "Press SPACE BAR . . .");

	for(;;)
	{
		key = getch();
		if(key == 32)
			break;
	}
	restoreScreen(&win);
}

/* returns a random number between 1 and 7 (inclusive) */
int Random(void)
{
	return(random(7) + 1);
}

/* sets screen after each deletion */
void setScreen(int y)
{
	char *buffer;
	char far *vdu = (char far *)0xB8000000;
	int k = 0, i, j;

	buffer = (char *)malloc(sizeof(char) * 660);

	if(buffer == NULL)
	{
		clearscreen(1, 25);
		printf("\n malloc failed . . . bailing out !!!");
		exit(1);
	}

	for(i = 1; i < y; i ++)
		for(j = 25; j < 55; j ++)
			buffer[k ++] = *(vdu + i * 160 + j * 2 + 1);

	k = 0;

	for(i = 2; i < y + 1; i ++)
		for(j = 25; j < 55; j ++)
			*(vdu + i * 160 + j * 2 + 1) = buffer[k ++];

	free(buffer);
}