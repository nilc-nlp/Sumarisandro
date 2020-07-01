#ifndef SALTON_H
#define SALTON_H

#include "../include/Library.h"
#include "../include/Graph.h"
#include "../include/Util.h"
#include "../include/Topic.h"

class Salton {

	private:
		Graph *g;
		Topic *t;
		vector<Sentence*> *vec_sent_denso, *vec_sent_denso_aux, *vec_sent_prof, *vec_sent_seg, *vec_sent_seg_aux, *vec_depth_topics;
		float size_limit;
		int corte;

	public:
		Salton(Graph *g);
		~Salton();

		void ponderarGrafo();
		void cortarArestas();
		void sortSentences();

		Sentence *depthFirstTopic(Sentence *parent);
		Sentence* transNodeTopic(Sentence *s);
		Sentence* bushyNodeTopic(int topic);
		float maiorCos(Sentence *s1, Sentence *s2);
		Sentence* noMaisDenso(char desempate[13]);
		Sentence* noMaisSimilarDerivado(Sentence *pai, char desempate[13]);
		vector<Sentence*>* caminhoDenso(int taxa_compressao, char desempate[13]);
		vector<Sentence*>* caminhoDenso(char desempate[13]);
		vector<Sentence*>* caminhoProfundo(int taxa_compressao, char desempate[13]);
		vector<Sentence*>* caminhoProfundo(char desempate[13]);
		vector<Sentence*>* caminhoSegmentado(int taxa_compressao, char desempate[13], char type[10]);
		vector<Sentence*>* caminhoSegmentado(char desempate[13]);
		
		/* Entrada: sentença a ser verificada, caracter identificador do tipo de caminho: denso (d) ou profundo (p))
		   Saída: valor verdadeiro se a sentença já estiver no sumário ou se há alguma similiar a ela, e falso caso constrário. */
		bool temSimilar(Sentence *s, vector<Sentence*> *vec_similar);
		void zerarExtrato();
		void setSizeLimit(float size_limit);
		float getSizeLimit();
		void calculateSizeLimit();
		bool temMaisFilhos(Sentence *s);
		bool temMaisFilhos(Sentence *s, char way);
};

#endif
