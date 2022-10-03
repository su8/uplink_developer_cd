char* strdup(char *strptr)
{
	char *charptr;

	charptr=(char *)malloc(sizeof(char)*(strlen(strptr)+1));
	if(charptr) 
		strcpy(charptr,strptr);
	return(charptr);
}
