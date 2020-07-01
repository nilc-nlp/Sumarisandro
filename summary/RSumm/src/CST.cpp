
#include "../include/CST.h"

CST::CST(vector<char*> *relacoes){

	if(!(this->rels = new vector<char*>()) || !(this->pesos = new vector<float>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		return;
	}

	carregarRelacoes(relacoes);
}

CST::~CST(){

	for(int i = 0; i < this->rels->size(); i++)
		this->rels->erase(this->rels->begin() + i);

	for(int i = 0; i < this->pesos->size(); i++)
		this->pesos->erase(this->pesos->begin() + i);

	delete this->rels;
	delete this->pesos;
}

void CST::carregarRelacoes(vector<char*> *relacoes){

	char *str_peso, *str_rel, *aux;
	int j, k;


	for(int i = 0; i < relacoes->size(); i++){

		if(!(aux = (char*) malloc(sizeof(char) * MAX_NUM_LETRAS)) || 
			!(str_peso = (char*) malloc(sizeof(char) * MAX_NUM_LETRAS)) ||
				!(str_rel = (char*) malloc(sizeof(char) * MAX_NUM_LETRAS))){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(1);
		}

		strcpy(aux, relacoes->at(i));

		k = 0;
		j = 0;

		while(aux[j] != '=')
			str_rel[k++] = aux[j++];
	
		str_rel[k] = '\0';
		
		for(int aux_k = 0; aux_k < k; aux_k++)
			str_rel[aux_k] = tolower(str_rel[aux_k]);

		this->rels->push_back(str_rel);

		k = 0;
		
		/* Não pega o '='. */
		j++;

		while(aux[j] != '\0')
			str_peso[k++] = aux[j++];
	
		str_peso[k] = '\0';

		this->pesos->push_back(atof(str_peso));

		aux = NULL;
		str_peso = NULL;
		str_rel = NULL;
	}

	free(aux);
	free(str_peso);
	free(str_rel);
}

vector<char*>* CST::getRelacoes(){

	return this->rels;
}

vector<float>* CST::getPesos(){

	return this->pesos;
}

char* CST::getRelacao(int index){

	return this->rels->at(index);
}

float CST::getPeso(int index){

	return this->pesos->at(index);
}

/* Método que identifica quais relações CST há entre todas as sentenças de todos os textos e cria as respectivas arestas com a ponderação dada no arquivo 
'CST_Rels.txt'.*/
void CST::leituraRelacoes(FILE *arq, Graph *g){

	char aux_str[MAX_NUM_LETRAS * 10], rel[MAX_NUM_LETRAS * 10];
	int i, doc_ini, doc_fim, sent_ini, sent_fim;

	doc_ini = doc_fim = -1;
	sent_ini = sent_fim = -1;
	strcpy(rel, "nada");

	while(!feof(arq)){

		if(fscanf(arq, "%s", aux_str) >= 0){

			/* Descobrimento o primeiro documento que a primeira sentença é derivada. */
			if(aux_str[0] == 'S' && aux_str[1] == 'D'){

				i = 2;

				while(aux_str[i++] != '"');
				
				i++;

				char c[1];
	
				c[0] = aux_str[i];
				c[1] = '\0';

				doc_ini = atoi(c) - 1;
			}

			/* Descobrimento a primeira sentença que compõe a relação .*/
			if(aux_str[0] == 'S' && aux_str[1] == 'S'){

				i = 2;

				while(aux_str[i++] != '"');

				char c[1];

				c[0] = aux_str[i];
				c[1] = '\0';

				sent_ini = atoi(c) - 1;
			}

			/* Descobrimento o segundo documento que a segunda sentença é derivada. */
			if(aux_str[0] == 'T' && aux_str[1] == 'D'){

				i = 2;

				while(aux_str[i++] != '"');
				
				i++;

				char c[1];
	
				c[0] = aux_str[i];
				c[1] = '\0';

				doc_fim = atoi(c) - 1;
			}

			/* Descobrimento a segunda sentença que compõe a relação .*/
			if(aux_str[0] == 'T' && aux_str[1] == 'S'){

				i = 2;

				while(aux_str[i++] != '"');

				char c[1];

				c[0] = aux_str[i];
				c[1] = '\0';

				sent_fim = atoi(c) - 1;
			}

			/* Descobrimento do nome da relação CST. */
			if(aux_str[0] == 'T' && aux_str[1] == 'Y'){

				i = 2;
				int j = 0;

				while(aux_str[i++] != '"');

				while(aux_str[i+1] != '\0')
					rel[j++] = tolower(aux_str[i++]);

				rel[j] = '\0';
cout << "Doc_ini: " << doc_ini << " - Doc_fim: " << doc_fim << endl;
cout << "Sent_ini: " << sent_ini << " - Sent_fim: " << sent_fim << endl;
cout << "Rel: " << rel << endl << endl;
				ponderarGrafoCST(g, doc_ini, doc_fim, sent_ini, sent_fim, rel);
			}			
		}
	}
}

void CST::ponderarGrafoCST(Graph *g, int doc_ini, int doc_fim, int sent_ini, int sent_fim, char rel[MAX_NUM_LETRAS]){

	Sentence *s1 = g->getIni();
	Sentence *s2;

	while(s1 != NULL){

		/* Seleciona as sentenças que correspondem à relação CST. */
		if(s1->getOrigemTexto() == doc_ini && s1->getPosicaoOriginal() == sent_ini){

			s2 = g->getIni();

			while(s2 != NULL){

				/* Seleciona a segunda sentença correspondente no grafo. */
				if(s2->getOrigemTexto() == doc_fim && s2->getPosicaoOriginal() == sent_fim){

					Edge *a1, *a2;

					/* Seleção da aresta entre S1 e S2. */
					for(int count = 0; count < s1->getSizeVecArestaTotal(); count++){

						if(s1->getAresta(count)->getS() == s2){

							for(int k = 0; k < this->rels->size(); k++){

								/* Criação da aresta CST com a ponderação determinada. */
								if(!strcmp(rel, this->rels->at(k))){
cout << "Inside(1)-> D" << s1->getOrigemTexto() << "_S" << s1->getPosicaoOriginal() << " - CST: " << rel << endl;
									a1 = s1->getAresta(count);
									a1->setCST(this->pesos->at(k));
									//a1->setCSTRelation((char*)rel);
								}
							}
						}
					}

					/* Garantia que a aresta também vá de S2 para S1. */
					for(int count = 0; count < s2->getSizeVecArestaTotal(); count++){

						if(s2->getAresta(count)->getS() == s1){

							for(int k = 0; k < this->rels->size(); k++){

								if(!strcmp(rel, this->rels->at(k))){
cout << "Inside(2)-> D" << s2->getOrigemTexto() << "_S" << s2->getPosicaoOriginal() << " - CST: " << rel << endl << endl;
									a2 = s2->getAresta(count);
									a2->setCST(this->pesos->at(k));
									//a2->setCSTRelation((char*)rel);
								}
							}
						}
					}
				}

				s2 = s2->getProx();
			}
		}

		s1 = s1->getProx();
	}
}
