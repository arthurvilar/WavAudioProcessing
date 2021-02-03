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
	int deslocamento, i, delay = 1000;
	int64_t temp;
	float level = 0.5; 

	//le o delay e o level
	for (i = 0; i <argc; i++) {

		if (strcmp(argv[i], "-t") == 0)
			delay = atoi(argv[i+1]);
		
		if (strcmp(argv[i], "-l") == 0)
			level = atof(argv[i+1]);
	}

	//confere se o delay é valido
	if (delay < 0) {
		fprintf(stderr, "%s", "Valor de delay impróprio.");
		exit(1);
	}

	//confere se level tem o valor adequado
	if ((level < 0) || (level > 1)) {
		fprintf(stderr, "%s", "Valor de level impróprio.\n");
		exit(1);
	}

	select_input(argc, argv, &input);	  //checa se ta usando stdin ou -i e abre o aqruivo
	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo

	wav_t *header = read_wav(input);	  //le o cabeçalho do .wav e coloca na variavel header

	fclose(input);

	//calcula o atraso do echo
	deslocamento = header->sample_rate * delay / 1000;

	//percorre o audio modificando as amostras aplicando o efeito de echo
	for (i = deslocamento + 1; i < header->data_size/2; ++i) {
		temp = (int16_t)header->audio_data[i] + ((int16_t)header->audio_data[i - deslocamento] * level);
		//confere se teve clipping
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