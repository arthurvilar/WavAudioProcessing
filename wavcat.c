//GRR20197153 Arthur Henrique Canello Vilar

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "wav.h"

int main (int argc, char **argv) {

	int i, j = 0, riffsize_geral = 0, datasize_geral = 0, old_datasize = 0;
	FILE *output = stdout;
	FILE *files[argc];
	wav_t *waves[argc];

	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo

	//abre todas as entradas em um vetor de arquivos
	for (i = 1; i < argc; i++) { 
		if ((strcmp(argv[i], "-o") != 0) && (strcmp(argv[i-1], "-o") != 0)) {  //confere se não é output
			//confere se abriu corretamente
			if ((files[j] = fopen(argv[i], "rb")) == NULL) {
				fprintf(stderr, "%s", "Erro ao abrir o arquivo\n");
				exit(1);
			}
			j++;
		}
	}

	//le os arquivos .wav que estao no vetor files em um vetor do tipo wav_t
	for (i = 0; i < j; i++) {
		waves[i] = read_wav(files[i]);	
		riffsize_geral += waves[i]->riff_size;	//variavel para contar o tamanho final do riff size
		datasize_geral += waves[i]->data_size;	//variavel para contar o tamanho final do data size
		fclose(files[i]);
	}
	
	old_datasize = waves[0]->data_size;		//guarda o tamanho original do primeiro audio
	waves[0]->riff_size = riffsize_geral;	//modifica o cabeçalho do primeiro audio para ser o cabeçalho final de todos os audios juntos
	waves[0]->data_size = datasize_geral;

	//escreve o cabeçalho final e o primeiro audio usando o tamanho original dele
	fwrite(waves[0], 44, 1, output);
	fwrite(waves[0]->audio_data, old_datasize, 1, output);
	//escreve o restante dos audios em seguida
	for (i = 1; i < j; i++) {
		fwrite(waves[i]->audio_data, waves[i]->data_size, 1, output);
		free_wav(waves[i]);
	}

	free_wav(waves[0]);
	
	//fecha output caso tenha sido aberto
	if (output != stdout)
		fclose(output);


	return 1;
}