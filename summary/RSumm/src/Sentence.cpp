#include "../include/Sentence.h"

/* Os métodos de determinar a sentença original e a sentença modificada as separam em uma matriz de caracteres para que facilite a posterior análise. */

Sentence::Sentence(char *sentenca_original, int origem_texto, int posicao_original, int posicao_virtual, vector<char*> *stopwords, int topic, char nome_arq[2000]){

	this -> sentenca_original = NULL;
	this -> sentenca = NULL;

	this -> sumario = false;

	setProx(NULL);
	setAnt(NULL);

	if(!(this -> aresta = new vector<Edge*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		return;
	}

	setOriginalFile(nome_arq);
	setTopic(topic);
	setPosicaoOriginal(posicao_original);
	setPosicaoVirtual(posicao_virtual);
	setNumPalavras(0);
	setNumPalavrasStemmStop(0);
	setOrigemTexto(origem_texto);

	setSentencaOriginal(sentenca_original, stopwords);
	setSentenca(sentenca_original, stopwords);
}

Sentence::~Sentence(){

	for(int i = 0 ; i < this -> num_palavras; i++)	
		delete this -> sentenca[i];

	for(int i = 0; i < this -> aresta -> size(); i++)
		this -> aresta -> erase(this -> aresta -> begin() + i);

	delete this -> aresta;
	delete this -> sentenca;
}

void Sentence::setOriginalFile(char nome_arq[2000]){

	strcpy(this->file_name, nome_arq);
}

char* Sentence::getOriginalFile(){

	return this->file_name;
}

void Sentence::setTopic(int topic){

	this -> topic = topic;
}

void Sentence::setNumPalavrasStemmStop(int num_palavras_sem_stopwords){

	if(num_palavras_sem_stopwords >= 0)
		this -> num_palavras_sem_stopwords = num_palavras_sem_stopwords;
}

void Sentence::setNumPalavrasStemmStop(char *sentenca_original, vector<char*> *stopwords){

	int i = 0, contador = 0, j = 0, aux;
	char word[MAX_NUM_LETRAS];

	while(sentenca_original[i] != '\0'){

		if(sentenca_original[i] != ' ')
			word[j++] = sentenca_original[i++];

		else {

			word[j] = '\0';

			checkPunctuationMark(word);

			if(strcmp(word, "\0")){

				aux = 0;

				while(word[aux] != '\0'){

					if(!Util().letter(word)){

						if(isupper(word[aux]))
							word[aux] = tolower(word[aux]);
					}

					aux++;
				}

				/* Verificação de existência de stopwords. */
				if(!isStopWord(word, stopwords, 0, stopwords -> size()-1))
					this -> num_palavras_sem_stopwords++;
			}

			i++;
			j = 0;
		}
	}

	word[j] = '\0';

	aux = 0;

/* Como a última palavra não pôde ser computada, pois está ligada ao '.', mais uma análise tem de ser realizada. */
	checkPunctuationMark(word);

	if(strcmp(word, "\0")){

		while(word[aux] != '\0'){

			if(!Util().letter(word)){

				if(isupper(word[aux]))
					word[aux] = tolower(word[aux]);
			}

			aux++;
		}
		
		if(!isStopWord(word, stopwords, 0, stopwords -> size()-1))
			this -> num_palavras_sem_stopwords++;
	}
}

/* Se a sentença já pertence ao resumo. */
bool Sentence::getSumario(){

	return this -> sumario;
}

void Sentence::setSumario(bool sumario){

	this -> sumario = sumario;
}

void Sentence::setNumPalavras(int num_palavras){

	if(num_palavras >= 0)
		this -> num_palavras = num_palavras;
}

void Sentence::setNumPalavras(char *sentenca){

	int i = 0;

	while(sentenca[i] != '\0'){

		if(sentenca[i++] == ' ')
			this -> num_palavras++;
	}

	this -> num_palavras++;
}

void Sentence::setProx(Sentence *prox){

	this -> prox = prox;
}

void Sentence::setAnt(Sentence *ant){

	this -> ant = ant;
}

void Sentence::setSentencaOriginal(char *sentenca_original, vector<char*> *stopwords){

	setNumPalavras(sentenca_original);
	
	if(!(this -> sentenca_original = new char* [sizeof(char) * this -> num_palavras])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);
	}

	int i, j, k;

	for(i = 0; i < this -> num_palavras; i++){

		if(!(this -> sentenca_original[i] = new char[sizeof(char) * MAX_NUM_LETRAS])){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(1);
		}
	}

	i = 0;
	j = 0;
	k = 0;

	while(i < strlen(sentenca_original)){

		k = 0;

		while(sentenca_original[i] != ' ' && sentenca_original[i] != '\0')
			this -> sentenca_original[j][k++] = sentenca_original[i++];

		this -> sentenca_original[j][k] = '\0';

		j++;
		i++;
	}
}

void Sentence::setSentenca(char *sentenca, vector<char*> *stopwords){

	setNumPalavrasStemmStop(sentenca, stopwords);

	if(!(this -> sentenca = new char* [sizeof(char) * this -> num_palavras_sem_stopwords])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);
	}

	int i, j, k, aux;
	char word[MAX_NUM_LETRAS];
	bool flag_stop = false;
	
	for(i = 0; i < this -> num_palavras_sem_stopwords; i++){

		if(!(this -> sentenca[i] = new char[sizeof(char) * MAX_NUM_LETRAS])){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			return;
		}

		else
			flag_stop = true;
	}

	if(flag_stop){

		i = 0;
		j = 0;
		k = 0;

		while(sentenca[i] != '\0'){

			if(sentenca[i] != ' ')
				word[j++] = sentenca[i++];

			else {

				word[j] = '\0';

				aux = 0;

				checkPunctuationMark(word);

			/* Se a palavra não for composta somente de uma pontuação. */
				if(strcmp(word, "\0")){

					while(word[aux] != '\0'){

						if(!Util().letter(word)){

							if(isupper(word[aux]))	
								word[aux] = tolower(word[aux]);
						}

						aux++;
					}

					/* Verificação de existência de stopwords. */
					if(!isStopWord(word, stopwords, 0, stopwords -> size()-1))
						strcpy(this -> sentenca[k++], word);
				}

				i++;
				j = 0;
			}
		}

		word[j] = '\0';

		aux = 0;

	/* Como a última palavra não pôde ser computada, pois está ligada ao '.', mais uma análise tem de ser realizada. */

		checkPunctuationMark(word);

		if(strcmp(word, "\0")){
	
			while(word[aux] != '\0'){

				if(!Util().letter(word)){

					if(isupper(word[aux]))
						word[aux] = tolower(word[aux]);
				}

				aux++;
			}

			if(!isStopWord(word, stopwords, 0, stopwords -> size()-1))
				strcpy(this -> sentenca[k], word);
		}
	}
}

void Sentence::setWord(int posicao, char word[MAX_NUM_LETRAS]){

//cout << "keeping word: " << word << " -- at: << " << posicao << endl;

	strcpy(this -> sentenca[posicao], word);
}

int Sentence::getTopic(){

	return this -> topic;
}

char* Sentence::getWord(int posicao){

//	cout << "retrieving word:" << this -> sentenca[posicao] << " -- at: " << posicao << endl;

	return this -> sentenca[posicao];
}

void Sentence::checkPunctuationMark(char *word){

/* . , : ; / \ ? ! @ # ^ & * ` ~ ' " “ ” ( ) [ ] { } | + = - _ < > ¨ ´ 
			33 caracteres

	O caracter faltante foi '\', que não pôde ser adicionada por seu caráter de cancelamento 
*/

	char punctmarks[] = ". , : ; / ? ! @ # ^ & * ` ~ ' ( ) [ ] { } | + = - _ < > “ ” ¨ ´ ", word_aux[strlen(word)];
	int k = 0;
	bool flag = false;

	/* Como as aspas duplas não podem ser adicionadas acima, precisa-se da linha de código abaixo. */
	punctmarks[strlen(punctmarks)] = '"';

	for(int i = 0; i < strlen(word); i++){

		for(int j = 0; j < strlen(punctmarks) && !flag; j++){

			if(word[i] == punctmarks[j])
				flag = true;
		}

		if(!flag)
			word_aux[k++] = word[i];
	}

	word_aux[k] = '\0';

	strcpy(word, word_aux);
}

bool Sentence::isStopWord(char word[MAX_NUM_LETRAS], vector<char*> *stopwords, int ini, int fim){

	if(ini > fim)
		return false;

	int meio = (ini + fim) / 2;

	strcat(word, "\0");
	strcat(stopwords -> at(meio), "\0");

	int aux = strcmp(word, stopwords -> at(meio));

	if(!aux)
		return true;
	
	if(aux > 0)
		return isStopWord(word, stopwords, ++meio, fim);

	return isStopWord(word, stopwords, ini, --meio);
}

void Sentence::setPosicaoOriginal(int posicao_original){

	this -> posicao_original = posicao_original;
}

void Sentence::setPosicaoVirtual(int posicao_virtual){

	this -> posicao_virtual = posicao_virtual;
}

void Sentence::setOrigemTexto(int origem_texto){

	if(origem_texto >= 0)
		this -> origem_texto = origem_texto;
}

int Sentence::getNumPalavras(){

	return this -> num_palavras;
}

int Sentence::getNumPalavrasStemmStop(){

	return this -> num_palavras_sem_stopwords;
}

Sentence* Sentence::getProx(){

	return prox;
}

Sentence* Sentence::getAnt(){

	return ant;
}

char** Sentence::getSentenca(){

	return this -> sentenca;
}

char** Sentence::getSentencaOriginal(){

	return this -> sentenca_original;
}

int Sentence::getPosicaoOriginal(){

	return this -> posicao_original;
}

int Sentence::getPosicaoVirtual(){

	return this -> posicao_virtual;
}

int Sentence::getOrigemTexto(){

	return this -> origem_texto;
}

void Sentence::setAresta(Edge *a){

	(this -> aresta) -> push_back(a);
}

Edge* Sentence::getAresta(int pos){

	return (this -> aresta) -> at(pos);
}

int Sentence::getSizeVecArestaTotal(){

	return this -> aresta -> size();
}

int Sentence::getSizeVecArestaCortada(){

	int tam = 0;

	for(int i = 0; i < this -> aresta -> size(); i++){

		Edge *a = this -> aresta -> at(i);

		if(!a -> getCortada())
			tam++;
	}

return tam;
}

int Sentence::getMaiorPesoTotal(float *maior){

	int index;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		float aux = a -> getPeso();

		if(aux > *maior){

			(*maior) = aux;
			index = i;
		}
	}

return index;
}

int Sentence::getMaiorPesoCortado(float *maior){

	int index;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		if(!a -> getCortada()){

			float aux = a -> getPeso();

			if(aux > *maior){

				(*maior) = aux;
				index = i;
			}
		}
	}

return index;
}

float Sentence::getMaiorPesoTotal(){

	float maior = 0;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		float aux = a -> getPeso();

		if(aux > maior)
			maior = aux;
	}

return maior;
}

float Sentence::getMaiorPesoCortado(){

	float maior = 0;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		if(!a -> getCortada()){

			float aux = a -> getPeso();

			if(aux > maior)
				maior = aux;
		}
	}

return maior;
}

int Sentence::getMenorPesoTotal(float *menor){

	int index;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		float aux = a -> getPeso();

		if(aux < *menor && aux != 0){
			(*menor) = aux;
			index = i;
		}
	}

return index;
}

int Sentence::getMenorPesoCortado(float *menor){

	int index;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		if(!a -> getCortada()){

			float aux = a -> getPeso();

			if(aux < *menor && aux != 0){
				(*menor) = aux;
				index = i;
			}
		}
	}

return index;
}

float Sentence::getMenorPesoTotal(){

	float menor = 1;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		float aux = a -> getPeso();

		if(aux < menor && aux != 0)
			menor = aux;
	}

return menor;
}

float Sentence::getMenorPesoCortado(){

	float menor = 1.00;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		if(!a -> getCortada()){

			float aux = a -> getPeso();

			if(aux < menor && aux != 0)
				menor = aux;
		}
	}

return menor;
}

float Sentence::getTotalPesosTotal(){

	float total = 0;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		total += a -> getPeso();
	}

return total;
}

float Sentence::getTotalPesosCortados(){

	float total = 0;

	for(int i = 0; i < (this -> aresta) -> size(); i++){

		Edge *a = (this -> aresta) -> at(i);

		if(!a -> getCortada())
			total += a -> getPeso();
	}

return total;
}
