//GRR20197153 Arthur Henrique Canello Vilar

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "wav.h"

//função para ler e armazenar o arquivo .wav em uma struct
wav_t *read_wav(FILE *wav_file) {

	//aloca espaço para o cabeçalho
	wav_t *header = malloc(sizeof(wav_t));
	if (header == NULL) {
		fprintf(stderr, "%s", "Falha na alocação do cabeçalho\n");
		exit(1);
	}

	//faz a leitura do cabeçalho
	fread(header, 44, 1, wav_file);

	//aloca espaço para o audio e armazena no audio_data
	header->audio_data = malloc(header->data_size);
	if (header->audio_data == NULL) {
		fprintf(stderr, "%s", "Falha na alocação do audio\n");
		exit(1);
	}

	//faz a leitura do audio data
	fread(header->audio_data, header->data_size, 1, wav_file);

	//calcula o sample channel(numero de amostras)
	header->sample_channel = header->data_size/header->block_align;

	return header;
}


//checa se foi especificado um arquivo de entrada com "-i", se não usa a stdin
void select_input(int argc, char **argv, FILE **input) {

	int i;
	
	for (i = 0; i < argc; i++)
		if (strcmp(argv[i], "-i") == 0)
			if ((*input = fopen(argv[i+1], "rb")) == NULL) {
				fprintf(stderr, "%s", "Erro ao abrir input\n");
				exit(1);
			}
}


//checa se foi especificado um arquivo de saida com "-o", se não usa a stdout
void select_output(int argc, char **argv, FILE **output) {

	int i;

	for (i = 0; i < argc; i++)
		if (strcmp(argv[i], "-o") == 0)
			if ((*output = fopen(argv[i+1], "wb")) == NULL) {
				fprintf(stderr, "%s", "Erro ao abrir o output\n");
				exit(1);
			}
}


//função para liberar os espaços alocados pela struct
int free_wav(wav_t *header) {

	if (header->audio_data != NULL)
		free(header->audio_data);
	free(header);
	
	return 1;
}