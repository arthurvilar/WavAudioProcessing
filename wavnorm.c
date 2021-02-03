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
	float fator;
	int16_t *temp;
	int64_t maior = 0;

	select_input(argc, argv, &input);	  //checa se ta usando stdin ou -i e abre o aqruivo
	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo

	wav_t *header = read_wav(input);	  //le o cabeçalho do .wav e coloca na variavel header

	fclose(input);

	temp = (int16_t*)header->audio_data;

	//percorre a amostra de audio e acha o valor do maior pico absoluto
	for (i = 0; i < header->sample_channel; ++i)
		if (abs(temp[i]) > maior)
			maior = abs(temp[i]);

	//divide 98% do valor maximo (32767) pelo valor do maior pico
	fator = (0.98 * MAX)/maior;
	fprintf(stderr, "Fator de correção = %f\n", fator);

	//multiplica as amostras pelo fator
	for (i = 0; i < header->sample_channel; ++i)
		header->audio_data[i] = (int16_t)header->audio_data[i] * fator;

	//escreve os dados modificados no output
	fwrite(header, 44, 1, output);
	fwrite(header->audio_data, header->data_size, 1, output);

	if (output != stdout)
		fclose(output);

	free_wav(header);

	return 1;
}