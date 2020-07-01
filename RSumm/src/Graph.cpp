#include "../include/Graph.h"

Graph::Graph(){

	this -> ini = this -> fim = NULL;
	this -> num_nos = 0;
	this -> topic = -1;
}

Graph::~Graph(){

}

int Graph::getNumTopics(){

	return this -> topic;
}

void Graph::setNumNos(int num_nos){

	this -> num_nos = num_nos;
}

int Graph::getNumNos(){

	return this -> num_nos;
}

void Graph::setIni(Sentence *no){

	this -> ini = no;
}

Sentence* Graph::getIni(){

	return this -> ini;
}

void Graph::setFim(Sentence *no){

	this -> fim = no;
}

Sentence* Graph::getFim(){

	return this -> fim;
}

/* Contrói o grafo correspondente a todos os textos de mesmo assunto. */
void Graph::carregar(FILE *arq, int origem_texto, vector<char*> *stopwords, int posicao, int *dezena, char nome_arq[2000]){

	/* O vetor 'str' comportará toda a sentença original lida. */
	char str[1000000], aux_char = 'x';
	int num_sent = 0;
	bool flag_stopped = true;
	Sentence *s_stopped;

	/* Quando houve a troca de textos, adiciona um para contabilizar outro tópico. */ 
	this -> topic++;

	/* Leitura do número total de sentenças (Ex: ^10 -> ^ - caracter delimitador; 10 - número de sentenças contidas em 'arq').*/
	//while(aux_char != '^')
//		fscanf(arq, "%c", &aux_char);

//	fscanf(arq, "%d", &num_sent);
	
	rewind(arq);

	FILE *arq_stemm;

					// ../Stemmer RSumm NORMAL
					// Stemmer RSummNews
	if(!(arq_stemm = fopen("../Stemmer/arq_stemm", "w+")))
		cout << endl << "ERROR! Couldn't open auxiliar archive text file" << endl << endl;

	while(fscanf(arq, "%[^\n]\n", str) > 0){

		strcat(str, "\0");
	
		/* Problema com strings que tem <t>\n .*/
		int aux_i;

		for(aux_i = 1; str[aux_i] != 13 && str[aux_i] != '\0'; aux_i++);

		str[aux_i] = '\0';

		/* Se a primeira posição não for o '\n', então há uma sentença a ser processada. */
		if(str[0] != '\0' && str[0] != 13 && strcmp(str, "<t>\0")){

			/* 'dezena' serve apenas para a posterior conferência dos dados (ela permite saber se a sentença número 1 é a 
			primeira do texto 1 ou se é a 18 (primeira do texto 2). */

//cout << "Sentenca(topic=" << this -> topic << "): " << str << endl;
 
			Sentence *s = new Sentence(str, origem_texto, posicao++, (*dezena)++, stopwords, this -> topic, nome_arq);

			if(flag_stopped == true){

				s_stopped = s;
				flag_stopped = false;
			}

			for(int j = 0; j < s -> getNumPalavrasStemmStop(); j++)
				fprintf(arq_stemm, "%s\n", s -> getWord(j));

			if(!s){

				cout << endl << "ERROR! Memory Overflow" << endl << endl;
				exit(EXIT_FAILURE);
			}

			if(this -> ini == NULL)
				this -> ini = s;	

			else
				this -> fim -> setProx(s);
	
			this -> fim = s;
			this -> fim -> setProx(NULL);

			setNumNos(getNumNos()+1);
		}

		else if(!strcmp(str, "<t>\0")){
	
			num_sent--;
			this -> topic++;
		}

		if(str[0] == 13)
			num_sent--;

		num_sent++;
	}

	this->topic--;

	fclose(arq_stemm);

	/* Chamada do TextTiling para a segmentação topical dos textos. */
			// ../Stemmer RSumm NORMAL					../Stemmer RSumm NORMAL	../Stemmer RSumm NORMAL
			// Stemmer RSummNews						Stemmer RSummNews		Stemmer RSummNews
	if(system("./../Stemmer/stemwords -l portuguese -i ../Stemmer/arq_stemm -o ../Stemmer/arq_stemm.stemmed")){

		cout << endl << "ERROR! Stemmer" << endl << endl;
		exit(EXIT_FAILURE);
	}

						// ../Stemmer RSumm NORMAL
						// Stemmer RSummNews
	if(!(arq_stemm = fopen("../Stemmer/arq_stemm.stemmed", "r")))
		cout << endl << "ERROR! Couldn't open segmented text file" << endl << endl;

	Sentence *s = s_stopped;
	char str_stemm[MAX_NUM_LETRAS];

	for(int i = 0; i < num_sent; i++){

		int num_palavras = s -> getNumPalavrasStemmStop();
		int aux = 0;
//cout << "NumPalavras: " << num_palavras << endl;
		while(aux < num_palavras && !feof(arq_stemm)){

			fscanf(arq_stemm, "%[^\n]\n", str);

			int aux_i;

			for(aux_i = 0; str[aux_i] != 13 && str[aux_i] != '\0'; aux_i++);

			str[aux_i] = '\0';
//cout << "\nparou!!\n";
//cout << "AUX: " << aux << endl;
//cout << "STR: " << str << endl;
//cout << "WORD: " << s -> getWord(aux) << endl;
			s -> setWord(aux, str);

			aux++;

			//cout << "STEMMED WORD: " << s -> getWord(aux-1) << endl;
		}

		if(aux < s -> getNumPalavrasStemmStop())
			s -> setNumPalavrasStemmStop(aux);

		s = s -> getProx();
	}

	fclose(arq_stemm);
/////////////////////////////////////////////////////////////////////////
//s = this -> ini;
//	while(s != NULL){

//		for(int i = 0; i < s -> getNumPalavrasStemmStop(); i++)
//			cout << "Stemmed (retr): " << s -> getWord(i) << endl;

//	s = s -> getProx();
//	}
/////////////////////////////////////////////////////////////////////////
}
