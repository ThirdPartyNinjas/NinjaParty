#ifndef WAV_H
#define WAV_H

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fmt_chunk_t
{
	unsigned short int format_type;
	unsigned short int channel_numbers;
	unsigned int sample_rate;
	unsigned int bytes_per_second;
	unsigned short int bytes_per_sample;
	unsigned short int bits_per_sample;
};

int read_chars(FILE *in, char *s, int n);
unsigned int read_long(FILE *in);
unsigned short int read_word(FILE *in);
int parse_header(FILE *in);
int parse_fmt(FILE *in, struct fmt_chunk_t *fmt_chunk);
int parse_data(FILE *in, struct fmt_chunk_t *fmt_chunk, int16_t *buffer, int *size);
int skip_chunk(FILE *in);

#ifdef __cplusplus
}
#endif

#endif//WAV_H
