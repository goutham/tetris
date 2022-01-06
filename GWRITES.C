void writes(int r, int c, int bcol, int fcol, char *string)
{
	char far *vdu = (char far *)0xB8000000;
	int i, j;
	-- r;
	-- c;
	r *= 160;
	c *= 2;
	j = strlen(string);

	for(i = 0; i < j; i ++)
	{
		*(vdu + r + c + i * 2) = string[i];
		*(vdu + r + c + i * 2 + 1) = (bcol << 4) | fcol;
	}
}