#ifndef CST_H
#define CST_H

#include "../include/Library.h"
#include "../include/Graph.h"

class CST {

	private:

		vector<char*> *rels;
		vector<float> *pesos;

	public:

		CST(vector<char*> *relacoes);
		~CST();

		void carregarRelacoes(vector<char*> *relacoes);
		void ponderarGrafoCST(Graph *g, int doc_ini, int doc_fim, int sent_ini, int sent_fim, char rel[MAX_NUM_LETRAS]);;

		/* Entrada: ponteiro do arquivo que contém as relações CST entre todas as sentenças dos textos e ponteiro do grafo dos mesmos. */
		void leituraRelacoes(FILE *arq, Graph *g);

		vector<char*>* getRelacoes();
		vector<float>* getPesos();
		char* getRelacao(int index);
		float getPeso(int index);
		
};

#endif
