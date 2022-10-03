// -*- tab-width:4; c-file-style:"cc-mode" -*-

#include "app/dos2unix.h"
#include <iostream.h>
#include <stdlib.h>

#define BUFLEN 8192

dos2unixbuf::dos2unixbuf(const char *filename, dos2unixbuf::openmode mode)
{
	inner.open(filename, mode);
	buffer = new char [BUFLEN + 1] + 1;
}

dos2unixbuf::~dos2unixbuf()
{
}

void dos2unixbuf::close()
{
	delete [] (buffer - 1);
	inner.close(); 
}

int dos2unixbuf::overflow(int c) 
{
	return c != EOF ? inner.sputc(c) : EOF;
}

int dos2unixbuf::underflow() 
{
	if (gptr() < egptr())
		return * (unsigned char *) gptr();

	char *b = buffer; 

	while (b < buffer + BUFLEN) { 
		int ch = inner.sbumpc();
		
		if (ch == '\r') 
			continue;
		else if (ch == EOF) 
			break;
		else 
			*b++ = ch;
	}

	setg(NULL, buffer, b); 

	if (b == buffer) 
		return EOF;
	else 
		return *(unsigned char *) buffer;
}

int dos2unixbuf::uflow() {
	int ch = underflow();
	if (ch != EOF) {
		stossc();
		return ch;
	}
	else
		return ch;
}
	
int dos2unixbuf::pbackfail(int c) {
	if (gptr() > base()) {
		setg(NULL, gptr() - 1, egptr());
		return c;
	} 
	else
		return EOF;
}

int dos2unixbuf::sync() {
	return inner.sync();
}

void idos2unixstream::close()
{
	dos2unixbuf *buf = (dos2unixbuf *) rdbuf();
	buf->close();
}

idos2unixstream::~idos2unixstream()
{
	delete rdbuf();
}
