#ifndef UTIL_H
#define UTIL_H

#include "../include/Library.h"
#include "../include/Sentence.h"
#include "../include/Graph.h"

class Graph;

class Util {

	private:

	public:
		Util();

		int median3(float *vec,int left,int right);
		int partition(float *vec, int left, int right, int pivot);
		void swap(float &val1, float &val2);
		void quicksort(float *vec, int left, int right);

		/* Entrada: primeiro vetor, segundo vetor e o tamanho (os dois vetores devem ser iguais)
		   Saída: produto escalar entre os vetores dados. */
		int dotProduct(int *vetA, int *vetB, int size);

		/* Entrada: vetor e seu tamanho
		   Saída: módulo do vetor dado. */
		float magnitude(int *vet, int size);

		/* Entrada: palavra a ser conferida, matriz a ser pesquisada e seu tamanho (número de linhas)
		   Saída: frequência da palavra dada na matriz (representada como uma sentença). */
		int frequencia(char X[MAX_NUM_LETRAS], char **C, int sizeC);

		/* Entrada: palavra a ser conferida, matriz a ser pesquisada e seu tamanho (número de linhas)
		   Saída: valor verdadeiro se a palavra está contida na matriz e falso caso constrário. */
		bool jahTem(char X[MAX_NUM_LETRAS], char **C, int count);

		/* Entrada: primeira sentença, segunda sentença, tamanho da primeira sentença, tamanho da sengunda sentença
		   Saída: valor do cosseno entre as sentenças. */		
		float cosineSimilarity(char *A[MAX_NUM_LETRAS], char *B[MAX_NUM_LETRAS], int sizeA, int sizeB);

		bool existe(float peso, float *vetor, int ini, int fim);

		float precisionRecall(float intersect, float num_sent);
		float calcularF_measure(float intersect, float num_sent_aut, float num_sent_ext);

		int numSentExt(vector<Sentence*> *vec, int *intersect);

		void gerarBaseline(Graph *g, int total);

		bool letter(char word[MAX_NUM_LETRAS]);
};

#endif
