//GRR20197153 Arthur Henrique Canello Vilar

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "wav.h"

int main (int argc, char **argv) {

	FILE *input = stdin;
	FILE *output = stdout;

	select_input(argc, argv, &input);	  //checa se ta usando stdin ou -i e abre o aqruivo
	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo

	wav_t *header = read_wav(input);	  //le o cabeçalho do .wav e coloca na variavel header

	fclose(input);

	//imprime as informações 
	fprintf(output, "riff tag 		: %c%c%c%c\n", header->riff_tag[0], header->riff_tag[1], header->riff_tag[2], header->riff_tag[3]);
	fprintf(output, "riff size 		: %d\n", header->riff_size);
	fprintf(output, "wave tag 		: %c%c%c%c\n", header->wave_tag[0], header->wave_tag[1], header->wave_tag[2], header->wave_tag[3]);
	fprintf(output, "form tag 		: %c%c%c%c\n", header->form_tag[0], header->form_tag[1], header->form_tag[2], header->form_tag[3]);
	fprintf(output, "fmt_size 		: %d\n", header->fmt_size);
	fprintf(output, "audio_format		: %d\n", header->audio_format);
	fprintf(output, "num_channels		: %d\n", header->num_channels);
	fprintf(output, "sample_rate		: %d\n", header->sample_rate);
	fprintf(output, "byte_rate		: %d\n", header->byte_rate);
	fprintf(output, "block_align		: %d\n", header->block_align);
	fprintf(output, "bits_per_sample		: %d\n", header->bits_per_sample);
	fprintf(output, "data tag 		: %c%c%c%c\n", header->data_tag[0], header->data_tag[1], header->data_tag[2], header->data_tag[3]);
	fprintf(output, "data size 		: %d\n", header->data_size);
	fprintf(output, "bytes per sample	: %d\n", header->bits_per_sample/8);
	fprintf(output, "samples per channel	: %d\n", header->data_size/header->block_align);

	if (output != stdout)
		fclose(output);

	free_wav(header);

	return 1;
}	