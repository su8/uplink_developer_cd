#include <ctype.h>
int strcasecmp(char *str1,char *str2)
{
	unsigned char 	*p1 = (unsigned char *) str1;
	unsigned char 	*p2 = (unsigned char *) str2;
	unsigned char	c1, c2;
	
	do
	{
		c1 = *p1++;
		c2 = *p2++;
	
		if (!c1 && !c2)
			return(0);
	}
	while ( tolower(c1) == tolower(c2) );
	
	return(c1 - c2);
}
