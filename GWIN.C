#include "myhead.h"

void Window(WINDOW *pwin)
{
	char far *vdu = (char far *)0xB8000000;
	int tx, ty, bx, by, bc, fc, i, j;

	pwin->screen = NULL;

	bc = pwin->bcol;
	fc = pwin->fcol;

	tx = pwin->topLeftX;
	bx = pwin->botRightX;
	ty = pwin->topLeftY;
	by = pwin->botRightY;

	-- tx;
	-- bx;
	-- ty;
	-- by;

	tx *= 2;
	bx *= 2;
	ty *= 160;
	by *= 160;

	storeScreen(&pwin->screen, tx, ty, bx, by);

	for(i = ty; i <= by; i += 160)
		for(j = tx; j <= bx; j += 2)
		{
			*(vdu + i + j) = ' ';
			*(vdu + i + j + 1) = fc | (bc << 4);
		}

	*(vdu + tx + ty) = 201;
	*(vdu + bx + by) = 188;
	*(vdu + tx + by) = 200;
	*(vdu + ty + bx) = 187;

	for(i = ty + tx + 2; i < ty + bx; i += 2)
		*(vdu + i) = 205;

	for(i = by + tx + 2; i < by + bx; i += 2)
		*(vdu + i) = 205;

	for(i = ty + 160; i < by; i += 160)
		*(vdu + i + tx) = *(vdu + i + bx) = 186;

	for(j = 0; pwin->header[j] != '\0'; j ++);

	for(i = 0; pwin->header[i] != '\0'; i ++)
		*(vdu + ((pwin->topLeftX + pwin->botRightX - 2) / 2 - j / 2) * 2 + (pwin->topLeftY - 1) * 160 + 2 * i) = pwin->header[i];
}

void storeScreen(char **pbuf, int tx, int ty, int bx, int by)
{
	char far *vdu = (char far *)0xB8000000;
	int chs, k = 0, i, j;

	chs = (by / 160 - ty / 160 + 1) * (bx / 2 - tx / 2 + 1) * 2;
	*pbuf = (char *)malloc(sizeof(char) * chs);

	if(*pbuf == NULL)
	{
		clrscr();
		printf("\n Fatal error");
		printf("\n Press any key to exit . . .");
		getch();
		exit(1);
	}

	for(i = ty; i <= by; i += 160)
		for(j = tx; j <= bx; j += 2)
		{
			(*pbuf)[k ++] = *(vdu + i + j);
			(*pbuf)[k ++] = *(vdu + i + j + 1);
		}
}

void restoreScreen(WINDOW *pwin)
{
	char far *vdu = (char far *)0xB8000000;
	int tx, ty, bx, by, bc, fc, i, j, k = 0;

	tx = pwin->topLeftX;
	bx = pwin->botRightX;
	ty = pwin->topLeftY;
	by = pwin->botRightY;

	-- tx;
	-- bx;
	-- ty;
	-- by;

	tx *= 2;
	bx *= 2;
	ty *= 160;
	by *= 160;

	for(i = ty; i <= by; i += 160)
		for(j = tx; j <= bx; j += 2)
		{
			*(vdu + i + j) = pwin->screen[k ++];
			*(vdu + i + j + 1) = pwin->screen[k ++];
		}
	free(pwin->screen);
	pwin->screen = NULL;
}