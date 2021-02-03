//GRR20197153 Arthur Henrique Canello Vilar

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "wav.h"

int main(int argc, char **argv) {

	FILE *input = stdin;
	FILE *output = stdout;
	int i, k = 0;

	select_input(argc, argv, &input);	  //checa se ta usando stdin ou -i e abre o aqruivo
	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo

	wav_t *header = read_wav(input);	  //le o cabeçalho do .wav e coloca na variavel header

	fclose(input);

	//rev recebe a amostra de audio invertida
	int16_t *rev = malloc(header->data_size);
	//escreve o audio data de forma reversa no rev
	for (i = header->data_size/2; i >= 0; i = i - 2) {
		rev[k] = header->audio_data[i-1];
		rev[k+1] = header->audio_data[i];
		k = k + 2;
	}

	//escreve os dados modificados no output
	fwrite(header, 44, 1, output);
	fwrite(rev, header->data_size, 1, output);

	//fecha output caso tenha sido aberto
	if (output != stdout)
		fclose(output);

	//libera os espaços alocados
	free(rev);
	free_wav(header);

	return 1;
}