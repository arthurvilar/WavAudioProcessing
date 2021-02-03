//GRR20197153 Arthur Henrique Canello Vilar

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "wav.h"
#define MAX 32767
#define MIN -32768

int main (int argc, char **argv) {

	FILE *input;
	FILE *output = stdout;
	wav_t *temp, *header, *principal;
	int i, j, pos_maior, maior = 0;
	int32_t soma;


	select_output(argc, argv, &output);   //checa se ta usando stdout ou -o, se for -o abre o arquivo
	
	//percorre os argumentos para achar o maior arquivo
	for (i = 1; i < argc; i++) { 
		if ((strcmp(argv[i], "-o") != 0) && (strcmp(argv[i-1], "-o") != 0)) {  //confere se não o arquivo nao é output
			
			//abre o arquivo
			if ((input = fopen(argv[i], "rb")) == NULL) {
				fprintf(stderr, "%s", "Erro ao abrir o input\n");
				exit(1);
			}
			//aloca espaço pro cabeçalho
			header = malloc(sizeof(wav_t));
			if (header == NULL) {
				fprintf(stderr, "%s", "Falha na alocação do cabeçalho\n");
				exit(1);
			}
			
			fread(header, 44, 1, input);
			//confere se o atual é maior que o maior e guarda a posição dele
			if (header->data_size > maior) {
				maior = header->data_size;
				pos_maior = i;
			}

			fclose(input);
			free(header);
		}
	}

	//le o maior arquivo wav e coloca no principal
	input = fopen(argv[pos_maior], "rb");
	principal = read_wav(input);

	//le o restante dos aquivos
	for (i = 1; i < argc; i++) { 
		if ((strcmp(argv[i], "-o") != 0) && (strcmp(argv[i-1], "-o") != 0)) {  //confere se o arquivo não é o output	
			
			if (i != pos_maior) {
				//abre o arquivo
				if ((input = fopen(argv[i], "rb")) == NULL) {
					fprintf(stderr, "%s", "Erro ao abrir o input\n");
					exit(1);
				}
				temp = read_wav(input);
				
				if (principal->sample_rate == temp->sample_rate) {
					//percorre o audio e soma as amostras do temp com o principal
					for (j = 0; j < temp->data_size/2; ++j) {
						soma = (int16_t)principal->audio_data[j] + (int16_t)temp->audio_data[j];
						//checa se aconteceu clipping
						if (soma > MAX)
							principal->audio_data[j] = MAX;
						else if (soma < MIN)
							principal->audio_data[j] = MIN;
						else
							principal->audio_data[j] = soma;
					}
				}
				else {
					fprintf(stderr, "%s", "Os arquivos nao tem a mesma taxa de amostragem\n");
					exit(1);
				}
				
				free_wav(temp);
				fclose(input);
			}
		}
	}
	

	//escreve os dados modificados no output
	fwrite(principal, 44, 1, output);
	fwrite(principal->audio_data, principal->data_size, 1, output);

	//fecha output caso tenha sido aberto
	if (output != stdout)
		fclose(output);

	//libera os espaços alocados
	free_wav(principal);

	return 1;
}