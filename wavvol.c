//GRR20197153 Arthur Henrique Canello Vilar

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "wav.h"
#define MAX 32767
#define MIN -32768

int main (int argc, char **argv) {

	FILE *input = stdin;
	FILE *output = stdout;
	int i;
	float vol = 1;
	int64_t temp;

	select_input(argc, argv, &input);	  //checa se ta usando stdin ou -i e abre o aqruivo
	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo

	wav_t *header = read_wav(input);	  //le o cabeçalho do .wav e coloca na variavel header
	
	fclose(input);

	//le o volume
	for (i = 0; i < argc; i++)
		if (strcmp(argv[i], "-l") == 0)
			vol = atof(argv[i+1]);

	//checa se volume tem o valor adequado
	if ((vol < 0) || (vol > 10)) {
		fprintf(stderr, "%s", "Volume nao esta entre 0 e 10.\n");
	}

	//multiplica as amostras de audio pelo vol
	for (i = 0; i < header->sample_channel; ++i) {
		
		temp = vol * (int16_t)header->audio_data[i];
		//confere se o valor ultrapassou o maximo ou minimo
		if (temp > MAX)
			header->audio_data[i] = MAX;
		else if (temp < MIN)
			header->audio_data[i] = MIN;
		else
			header->audio_data[i] = temp;
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