#ifndef SENTENCE_H
#define SENTENCE_H

#include "../include/Library.h"
#include "../include/Edge.h"
#include "../include/Util.h"

class Edge;

class Sentence {

	private:

	/* A ideia de ter duas matrizes de caracteres é que em uma armazenamos a sentença original ('sentenca_original'), sem mudanças, para que ela possa ser  
resgatada, se selecionada, para compor o sumário. A outra ('sentenca') serve para analisarmos somente as palavras relevantes de cada sentença. */
		char **sentenca;
		char **sentenca_original;
		int posicao_original;
		int posicao_virtual;
		bool sumario;
		int topic;
		int origem_texto;
		int num_palavras;
		int num_palavras_sem_stopwords;
		Sentence *prox, *ant;
		char file_name[2000];

		/* Arestas do grafo. */		
		vector<Edge*> *aresta;

	public:
		Sentence(char *sentenca, int origem_texto, int posicao_original, int posicao_virtual, vector<char*> *stopwords, int topic, char nome_arq[2000]);
		~Sentence();

		void setOriginalFile(char nome_arq[2000]);
		void setTopic(int topic);
		void setProx(Sentence *prox);
		void setAnt(Sentence *ant);
		void setPosicaoOriginal(int posicao_original);
		void setPosicaoVirtual(int posicao_virtual);
		void setOrigemTexto(int origem_texto);
		void setNumPalavrasStemmStop(char *sentenca_original, vector<char*> *stopwords);
		void setNumPalavrasStemmStop(int num_palavras_sem_stopwords);
		void setNumPalavras(char *sentenca);
		void setNumPalavras(int num_palavras);
		void setSentencaOriginal(char *sentenca_original, vector<char*> *stopwords);
		void setSentenca(char *sentenca, vector<char*> *stopwords);
		void setAresta(Edge *a);
		void setPeso(float peso);
		void setSumario(bool sumario);
		void setWord(int posicao, char word[MAX_NUM_LETRAS]);

		void checkPunctuationMark(char *word);
		bool isStopWord(char *word, vector<char*> *stopwords, int ini, int fim);

		char* getOriginalFile();
		int getTopic();
		Sentence* getProx();
		Sentence* getAnt();
		Sentence* getRelProx();
		Sentence* getRelAnt();
		char** getSentenca();
		char** getSentencaOriginal();
		int getPosicaoOriginal();
		int getPosicaoVirtual();
		int getOrigemTexto();
		int getNumPalavras();
		int getNumPalavrasStemmStop();
		Edge* getAresta(int pos);
		int getMaiorPesoTotal(float *maior);
		int getMaiorPesoCortado(float *maior);
		int getMenorPesoTotal(float *maior);
		int getMenorPesoCortado(float *maior);
		float getMaiorPesoTotal();
		float getMaiorPesoCortado();
		float getMenorPesoTotal();
		float getMenorPesoCortado();
		float getTotalPesosCortados();
		float getTotalPesosTotal();
		int getSizeVecArestaTotal();
		int getSizeVecArestaCortada();
		bool getSumario();
		char* getWord(int posicao);
};

#endif
