//GRR20197153 Arthur Henrique Canello Vilar

#ifndef __WAV__
#define __WAV__

typedef struct {
	char riff_tag[4];          // riff string
	uint32_t riff_size;        // tamanho do arquivo em bytes
	char wave_tag[4];          // wave string
	char form_tag[4];          // cabeçalho do chunk fmt
	uint32_t fmt_size;         // tamanho do chunk fmt
	uint16_t audio_format;     // codificação do audio
	uint16_t num_channels;     // numero de canais de audio
	uint32_t sample_rate;      // taxa de amostragem
	uint32_t byte_rate;        // taxa de bytes por segundo
	uint16_t block_align;      // numero de bytes por amostra
	uint16_t bits_per_sample;  // bits por amostra
	char data_tag[4];          // cabeçalho do chunk
	uint32_t data_size;        // espaço ocupado pelas amostras em bytes
	uint16_t *audio_data;	   // amostras de audio
	size_t sample_channel;	   // numero de amostras
} wav_t;

//le o arquivo .wav em um ponteiro do tipo wav_t
wav_t *read_wav(FILE *wav_file);

//libera o espaço alocado 
int free_wav(wav_t *header);

//checa se foi especificado um arquivo de entrada com "-i", se não usa a stdin
void select_input(int argc, char **argv, FILE **input);

//checa se foi especificado um arquivo de saida com "-o", se não usa a stdout
void select_output(int argc, char **argv, FILE **output);

#endif