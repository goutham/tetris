#include "myhead.h"

int menuNavigator(MENU *pobject)
{
	char key;
	int item = 1;

	highlight(pobject, 1, item);

	while(1)
	{
		key = getch();

		if(key == 13)
			return item;

		else if(key > 48 && key <= pobject->nitems + 48)
			return key - 48;

		else if(key == 0)
		{
			key = getch();

			if(key == 72 || key == 80)
				highlight(pobject, 0, item);

			if(key == 72)
				item == 1 ? item = pobject->nitems : --item;

			else if(key == 80)
				item == pobject->nitems ? item = 1 : ++ item;

			if(key == 72 || key == 80)
				highlight(pobject, 1, item);
		}
	}
}

void highlight(MENU *pobject, int mode, int pos)
{
	int colours[2], cr, cc, i;
	char far *vdu = (char far *)0xB8000000;

	colours[0] = !mode ? pobject->bcol_nhlt : pobject->bcol_hlt;
	colours[1] = !mode ? pobject->fcol_nhlt : pobject->fcol_hlt;

	colours[0] <<= 4;

	cr = ((pobject->row - 1) + (pos - 1) * pobject->steps) * 160;
	cc = (pobject->col - 1) * 2;

	for(i = 0; i < 2 * pobject->length; i += 2)
		*(vdu + cr + cc + i + 1) = colours[0] | colours[1];
}