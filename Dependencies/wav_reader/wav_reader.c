#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav_reader.h"

int read_chars(FILE *in, char *s, int n)
{
	int t,ch;
	for (t=0; t<n; t++)
	{
		ch=getc(in);
		if (ch==EOF) return -1;
		s[t]=ch;
	}
	s[t]=0;
	return 0;
}

unsigned int read_long(FILE *in)
{
	unsigned int t;
	
	t=getc(in);
	t=(getc(in)<<8)+t;
	t=(getc(in)<<16)+t;
	t=(getc(in)<<24)+t;
	
	return t;
}

unsigned short int read_word(FILE *in)
{
	unsigned short int t;
	
	t=getc(in);
	t=(getc(in)<<8)+t;
	
	return t;
}

int parse_header(FILE *in)
{
	int length;
	char riff_type[5];
	
	length=read_long(in);
	read_chars(in,riff_type,4);
	
	return 0;
}

int skip_chunk(FILE *in)
{
	int length;
	
	length=read_long(in);
	fseek(in, length, SEEK_CUR);
	
	return 0;
}

int parse_fmt(FILE *in, struct fmt_chunk_t *fmt_chunk)
{
	int length;
	int nextPosition;
	
	length=read_long(in);
	nextPosition = (int)ftell(in) + length;
	fmt_chunk->format_type=read_word(in);
	fmt_chunk->channel_numbers=read_word(in);
	fmt_chunk->sample_rate=read_long(in);
	fmt_chunk->bytes_per_second=read_long(in);
	fmt_chunk->bytes_per_sample=read_word(in);
	fmt_chunk->bits_per_sample=read_word(in);
	fseek(in, nextPosition, SEEK_SET);
	
	return 0;
}

int parse_data(FILE *in, struct fmt_chunk_t *fmt_chunk, int16_t *buffer, int *size)
{
	int t;
	int pos = (int)ftell(in);
	int16_t *position = buffer;
	int length = read_long(in);
	
	if(buffer == NULL)
	{
		*size = length / 2;
		fseek(in, pos, SEEK_SET);
		return 0;
	}
	
	if (fmt_chunk->bits_per_sample==16)
	{
		if(*size < length / 2)
			return -1;
		
		for (t=0; t<length/2; t++)
		{
			*position++ = getc(in)+(getc(in)<<8);
		}
	}
    else if (fmt_chunk->bits_per_sample==8)
	{
		if(*size < length)
			return -1;
		
		for (t=0; t<length; t++)
		{
			*position++ = getc(in) << 8;
		}
	}
	
	return 0;
}
