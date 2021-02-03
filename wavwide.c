//GRR20197153 Arthur Henrique Canello Vilar

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "wav.h"
#define MAX 32767
#define MIN -32768

int main(int argc, char **argv) {

	FILE *input = stdin;
	FILE *output = stdout;
	int i;
	int64_t temp;
	float k = 1, diff;

	select_input(argc, argv, &input);	  //checa se ta usando stdin ou -i e abre o aqruivo
	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo

	wav_t *header = read_wav(input);	  //le o cabeçalho do .wav e coloca na variavel header

	fclose(input);

	//checa se o som é estereo
	if (header->num_channels != 2) {
		fprintf(stderr, "%s", "Som não é estereo.\n");
	}

	//le o level
	for (i = 0; i < argc; i++)
		if (strcmp(argv[i], "-l") == 0)
			k = atof(argv[i+1]);

	//checa se level tem o valor adequado
	if ((k < 0) || (k > 10)) {
		fprintf(stderr, "%s", "Valor de k inadequado.\n");
	}

	//percorre o audio modificando as amostras
	for (i = 0; i < header->data_size/2; i = i+2) {

		diff = (int16_t)header->audio_data[i+1] - (int16_t)header->audio_data[i]; //diff = R[i] - L[i]
		
		temp = (int16_t)header->audio_data[i] - k * diff; //temp = L[i] - k * diff
		//confere se estourou o audio do canal esquerdo
		if (temp > MAX)
			header->audio_data[i] = MAX;
		else if (temp < MIN)
			header->audio_data[i] = MIN;
		else
			header->audio_data[i] = temp;

		temp = (int16_t)header->audio_data[i+1] + k * diff; //temp = R[i] + k * diff
		//confere se estourou o audio do canal direito
		if (temp > MAX)
			header->audio_data[i+1] = MAX;
		else if (temp < MIN)
			header->audio_data[i+1] = MIN;
		else
			header->audio_data[i+1] = temp;
	}

	//escreve os dados modificados no output
	fwrite(header, 44, 1, output);
	fwrite(header->audio_data, header->data_size, 1, output);

	//fecha output caso tenha sido aberto
	if (output != stdout)
		fclose(output);

	//libera os espaços alocados
	free_wav(header);

	return 1;
}