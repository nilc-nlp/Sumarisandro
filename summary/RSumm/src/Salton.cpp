#include "../include/Salton.h"

Salton::Salton(Graph *g){

	setSizeLimit(0.0f);
	
	this->g = g;

	this->vec_sent_denso = new vector<Sentence*>;
	this->vec_sent_denso_aux = new vector<Sentence*>;
	this->vec_sent_prof = new vector<Sentence*>;
	this->vec_sent_seg = new vector<Sentence*>;
	this->vec_sent_seg_aux = new vector<Sentence*>;
	this->vec_depth_topics = new vector<Sentence*>;

	if(!this->vec_sent_denso || 
		!this->vec_sent_denso_aux || 
			!this->vec_sent_prof ||
				!this->vec_sent_seg ||
					!this->vec_sent_seg_aux ||
						!this->vec_depth_topics){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}
}

Salton::~Salton(){

	int size = this->vec_sent_denso->size();

	for(int i = 0; i < size; i++)
		this->vec_sent_denso->pop_back();

	size = this->vec_sent_denso_aux->size();

	for(int i = 0; i < size; i++)
		this->vec_sent_denso_aux->pop_back();

	size = this->vec_sent_prof->size();

	for(int i = 0; i < size; i++)
		this->vec_sent_prof->pop_back();

	size = this->vec_sent_seg->size();

	for(int i = 0; i < size; i++)
		this->vec_sent_seg->pop_back();

	size = this->vec_sent_seg_aux->size();

	for(int i = 0; i < size; i++)
		this->vec_sent_seg_aux->pop_back();

	size = this->vec_depth_topics->size();

	for(int i = 0; i < size; i++)
		this->vec_depth_topics->pop_back();

	delete this->vec_sent_denso;
	delete this->vec_sent_denso_aux;
	delete this->vec_sent_prof;
	delete this->vec_sent_seg;
	delete this->vec_sent_seg_aux;
	delete this->vec_depth_topics;
}

void Salton::setSizeLimit(float size_limit){

	this->size_limit = size_limit;
}

float Salton::getSizeLimit(){

	return this->size_limit;
}

/* Neste método é calculado o limite de peso que será usado para determinar o quão próxima uma sentença pode estar de uma outra para ser, ou não, levada para o 
sumário. */
void Salton::calculateSizeLimit(){

	Sentence *s = this->g->getIni();

	float maior_unique, maior_total = 0.0f;
	float menor_unique, menor_total = 1.0f;

	while(s != NULL){

		maior_unique = s->getMaiorPesoTotal();
		menor_unique = s->getMenorPesoTotal();

		if(menor_unique < menor_total && menor_unique != 0.0f)
			menor_total = menor_unique;

		if(maior_unique > maior_total)
			maior_total = maior_unique;
	
		s = s->getProx();
	}

	/* [MaiorPeso + MenorPeso(>0.0)] / 2 */
	this->size_limit = (maior_total + menor_total) / 2.0f;
}

/* Para que os caminhos propostos por Salton possam ser usados em uma única execução, todas as sentenças são fixadas, novamente,
como não pertencentes ao sumário. */
void Salton::zerarExtrato(){

	Sentence *s = this->g->getIni();

	while(s != NULL){

		if(s->getSumario())
			s->setSumario(false);

		s = s->getProx();
	}
}

/* Neste método, o grafo com suas arestas ponderadas é contruído de forma que essa ponderação é feita com a similaridade de cosseno. A diferença é que ao invés de
calcular a presença ou não da palavra, sua frequência com relação as sentenças correntes. */ 
void Salton::ponderarGrafo(){

	Sentence *s1, *s2;

	char **aux_s1, **aux_s2;
	float cos;
	int total_arestas = 0;

	s1 = g->getIni();

	while(s1 != NULL){

		s2 = s1->getProx();
		aux_s1 = s1->getSentenca();
	
		while(s2 != NULL){
				
			aux_s2 = s2->getSentenca();

			cos = Util().cosineSimilarity(aux_s1, aux_s2, s1->getNumPalavrasStemmStop(), s2->getNumPalavrasStemmStop());

			/* Deixar o cosseno com somente duas casas decimais. */
			int aux_cos = (int) (cos * 100.0f);

			cos = ((float) aux_cos) / 100.0f;

			Edge *a1 = new Edge(s1, cos);
			Edge *a2 = new Edge(s2, cos);
				
			if(!a1 || !a2){

				cout << endl << "ERROR! Memory Overflow" << endl << endl;
				exit(EXIT_FAILURE);
			}

			if(aux_cos == 0.0f){

				a1->setCortada(true);
				a2->setCortada(true);
			}

			s1->setAresta(a2);
			s2->setAresta(a1);

			s2 = s2->getProx();
		}

		s1 = s1->getProx();
	}

	calculateSizeLimit();

	cortarArestas();
}

void Salton::cortarArestas(){

	/* Medida Ad-hoc = 1.5. */
	this->corte = 1.5 * this->g->getNumNos();

	/* Número de arestas (grafo completo) = (n * (n - 1)) / 2. */
	int total_arestas = (this->g->getNumNos() * (this->g->getNumNos() - 1) / 2);
	bool pare = false;

	float *vetor_maior;

	if(!(vetor_maior = new float[100])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	int cont = 0;
	bool aumentou;

	Sentence *s = this->g->getIni();

	while(s != NULL){

		for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

			Edge *a = s->getAresta(i);

			if(!a->getCortada()){

				aumentou = true;

				for(int j = 0; j <= cont; j++){
		
					if(a->getPeso() == vetor_maior[j])
						aumentou = false;
				}

				if(aumentou)		
					vetor_maior[cont++] = a->getPeso();
			}
		}

		s = s->getProx();
	}

	/* Ordenar o vetor para selecionar os 'this->corte' melhores pesos. */
	Util().quicksort(vetor_maior, 0, cont);

	s = this->g->getIni();

	while(s != NULL){

		for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

			Edge *a = s->getAresta(i);

			if(!a->getCortada()){

				if(Util().existe(a->getPeso(), vetor_maior, 0, this->corte-1))
					a->setCortada(false);

				else 
					a->setCortada(true);
			}
		}

		s = s->getProx();
	}

	delete[] vetor_maior;
}

float Salton::maiorCos(Sentence *s1, Sentence *s2){

	Sentence *s = g->getIni();
	float aux = -1, maior = -1;
	int index_aux, index;

	while(s != NULL){

		index_aux =  s->getMaiorPesoTotal(&aux);

		if(aux > maior){

			maior = aux;
			index = index_aux;
			s1 = s;	
		}

		s = s->getProx();
	}

return maior;
}

void Salton::sortSentences(){

	int lesser, index, vec_aux_i[10000];
	bool flag = false, first = true, flag_vec_i;

	vec_aux_i[0] = -1;

	while(!flag){

		lesser = 10000;
		index = -1;

		for(int i = 0; i < this->vec_sent_denso->size(); i++){
/////////////////////////////////////////////////////////
			//for(int jj = 0; vec_aux_i[jj] != -1; jj++)
			//	cout << vec_aux_i[jj] << " ";
////////////////////////////////////////////////////////

			if(first){

				if(this->vec_sent_denso->at(i)->getPosicaoOriginal() < lesser){

					lesser = this->vec_sent_denso->at(i)->getPosicaoOriginal();
					index = i;
				}

				else if(this->vec_sent_denso->at(i)->getPosicaoOriginal() == lesser){

					if(this->vec_sent_denso->at(i)->getNumPalavrasStemmStop() < this->vec_sent_denso->at(index)->getNumPalavrasStemmStop())
						index = i;
				}

				if(index != -1)
					first = false;
			}

			else {

				flag_vec_i = false;

				for(int j = 0; vec_aux_i[j] != -1; j++){

					if(i == vec_aux_i[j])
						flag_vec_i = true;
				}

				if(!flag_vec_i){

					if(this->vec_sent_denso->at(i)->getPosicaoOriginal() < lesser){

						lesser = this->vec_sent_denso->at(i)->getPosicaoOriginal();
						index = i;
					}

					else if(this->vec_sent_denso->at(i)->getPosicaoOriginal() == lesser){

						if(this->vec_sent_denso->at(i)->getNumPalavrasStemmStop() < this->vec_sent_denso->at(index)->getNumPalavrasStemmStop())
							index = i;
					}
				}
			}
		}

		if(index == -1)
			flag = true;

		else {

/*
cout << "\nbefore\n";
for(int jj = 0; vec_aux_i[jj] != -1; jj++)
	cout << vec_aux_i[jj] << " ";
*/
			int k;

			for(k = 0; vec_aux_i[k] != -1; k++);

			vec_aux_i[k] = index;
			vec_aux_i[k+1] = -1;

/*
cout << "\nafter\n";
for(int jj = 0; vec_aux_i[jj] != -1; jj++)
	cout << vec_aux_i[jj] << " ";
*/	
			//cout << "\nIndex: " << index << endl;
			//getchar();

			this->vec_sent_denso_aux->push_back(this->vec_sent_denso->at(index));
		}
	}
}

vector<Sentence*>* Salton::caminhoDenso(int taxa_compressao, char desempate[13]){

	int aux = 0;
	bool flag = false;

	while(aux < taxa_compressao && !flag){

		Sentence *s = noMaisDenso(desempate);

		if(s != NULL){

			s->setSumario(true);

			if(!temSimilar(s, this->vec_sent_denso)){

				aux += s->getNumPalavras();

				this->vec_sent_denso->push_back(s);
			}
		}

		else
			flag = true;
	}

	//sortSentences();

return this->vec_sent_denso_aux;
}

vector<Sentence*>* Salton::caminhoDenso(char desempate[13]){

	int aux = 0;
	bool flag = false;

	while(aux++ < g->getNumNos() && !flag){

		Sentence *s = noMaisDenso(desempate);

		if(s != NULL){

			s->setSumario(true);
	
			if(!temSimilar(s, this->vec_sent_denso))
				this->vec_sent_denso->push_back(s);
		}

		else
			flag = true;
	}

return this->vec_sent_denso;
}

vector<Sentence*>* Salton::caminhoProfundo(int taxa_compressao, char desempate[13]){

	int aux = 0;
	bool flag = false;

	while(aux < taxa_compressao && !flag){

		Sentence *parent = noMaisDenso(desempate);

		if(parent != NULL){

			parent->setSumario(true);

			if(!temSimilar(parent, this->vec_sent_prof)){

				aux += parent->getNumPalavras();

				this->vec_sent_prof->push_back(parent);
			}
		}

		else
			flag = true;

		if(!flag){

			bool tem_filhos = true;

			while(aux < taxa_compressao && tem_filhos){

				Sentence *s = noMaisSimilarDerivado(parent, desempate);

				if(s != NULL){

					s->setSumario(true);

					if(!temSimilar(s, this->vec_sent_prof)){

						aux += s->getNumPalavras();

						this->vec_sent_prof->push_back(s);

						parent = s;
					}

					if(!temMaisFilhos(parent, 'p'))
						tem_filhos = false;
				}

				else
					flag = true;
			}
		}
	}

	return this->vec_sent_prof;
}

vector<Sentence*>* Salton::caminhoProfundo(char desempate[13]){

	int aux = 0;
	bool flag = false;

	while(aux < this->g->getNumNos() && !flag){

		Sentence *parent = noMaisDenso(desempate);

		if(parent != NULL){

			parent->setSumario(true);

			if(!temSimilar(parent, this->vec_sent_prof)){

				aux++;
	
				this->vec_sent_prof->push_back(parent);
			}
		}

		else
			flag = true;

		if(!flag){

			bool tem_filhos = true;

			while(aux < this->g->getNumNos() && tem_filhos){

				Sentence *s = noMaisSimilarDerivado(parent, desempate);

				if(s != NULL){

					s->setSumario(true);

					if(!temSimilar(s, this->vec_sent_prof)){

						aux++;

						this->vec_sent_prof->push_back(s);

						parent = s;
					}

					if(!temMaisFilhos(parent))
						tem_filhos = false;
				}

				else
					flag = true;
			}
		}
	}

return this->vec_sent_prof;
}

bool Salton::temMaisFilhos(Sentence *s){

	for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

		Edge *a = s->getAresta(i);

		if(!a->getCortada() && !(a->getS())->getSumario())
			return true;
	}

return false;
}


bool Salton::temMaisFilhos(Sentence *s, char way){

	for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

		Edge *a = s->getAresta(i);

		if((a->getS())->getPosicaoVirtual() > s->getPosicaoVirtual() && !a->getCortada() && !(a->getS())->getSumario())
			return true;
	}

return false;
}


/* Confere se no sumário já existe alguma sentença similiar comparada a uma dada. */
bool Salton::temSimilar(Sentence *s, vector<Sentence*> *vec_similar){

	if(vec_similar->size() > 0){

		int count = 0;

		while(count < vec_similar->size()){

			Sentence *s_aux = vec_similar->at(count++);

			for(int i = 0; i < s_aux->getSizeVecArestaTotal(); i++){

				Edge *a = s_aux->getAresta(i);

				if(a->getS() == s){

					// Redundancy Removal Method: Jackson
					//return redundancyRemoval(s, s_aux);
					
					// Redundancy Removal Method: COSINE
					if(a->getPeso() >= getSizeLimit())
						return true;
				}
			}
		}
	}

	return false;
}


/* Método que calcula o nó que tem mais arestas. */
Sentence* Salton::noMaisDenso(char desempate[13]){

	float aux, maior, total, total_ant;
	Sentence *s = NULL, *s_aux = NULL, *ini;

	ini = this->g->getIni();

	maior = -1;

	while(ini != NULL){

		aux = ini->getSizeVecArestaCortada();
		total = ini->getTotalPesosCortados();

		if(!ini->getSumario() && aux >= maior){

			if(aux == maior){

		/* Se outra sentença tiver a mesma densidade resolve-se pela soma dos pesos. */
				if(total_ant < total)
					s = ini;

		/* Se ainda houver empate, tenta resolver pela CST. */
				else if(total_ant == total){

					if(!strcmp(desempate, "-unitaria")){

						int total_cst_ini = 0, total_cst_s = 0;

						for(int i = 0; i < ini->getSizeVecArestaTotal(); i++){

							Edge *a = ini->getAresta(i);

							total_cst_ini += a->getSizeVecCST();
						}

						for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

							Edge *a = s->getAresta(i);

							total_cst_s += a->getSizeVecCST();
						}

						if(total_cst_ini < total_cst_s)
							s = ini;
					} 

					if(!strcmp(desempate, "-ponderada")){

						int total_cst_ini = 0, total_cst_s = 0;

						for(int i = 0; i < ini->getSizeVecArestaTotal(); i++){

							Edge *a = ini->getAresta(i);

							total_cst_ini += a->getTotalPesosCST();
						}

						for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

							Edge *a = s->getAresta(i);

							total_cst_s += a->getTotalPesosCST();
						}

						if(total_cst_ini < total_cst_s)
							s = ini;
					} 

			/* Se não der, pega a primeira sentença. */				
				}
			}

			else {

				total_ant = total;
				maior = aux;
				s = ini;
			}
		}

		ini = ini->getProx();
	}

return s;
}

/* Calcula o nó filho mais denso de um dado nó parent */
Sentence* Salton::noMaisSimilarDerivado(Sentence *parent, char desempate[13]){

	Sentence *filho = NULL;

	if(parent != NULL && parent->getSizeVecArestaCortada()){

		float total, total_ant = 0, maior_peso = -1;
		int aux_densidade, maior_densidade = 0;

		for(int i = 0; i < parent->getSizeVecArestaTotal(); i++){

			Edge *a = parent->getAresta(i);

			bool pode_analisar = false;

			/* Se o parent e o filho forem de documentos diferentes, pode analisar. */
			if(parent->getOrigemTexto() != a->getS()->getOrigemTexto())
				pode_analisar = true;

			/* Se o parent e o filho forem do mesmo documento, a ordem deve ser cronológica, ou seja, 
			o parent tem que vir antes do filho. */
			if(parent->getOrigemTexto() == a->getS()->getOrigemTexto() && 
					parent->getPosicaoOriginal()+1 < a->getS()->getPosicaoVirtual()+1)
				pode_analisar = true;

			if(!a->getCortada() && pode_analisar && a->getPeso() < this->size_limit){

				Sentence *aux_sent = a->getS();

				total = aux_sent->getTotalPesosCortados();
				aux_densidade = aux_sent->getSizeVecArestaCortada();

				if(!aux_sent->getSumario() && aux_densidade >= maior_densidade){

					if(aux_densidade == maior_densidade){

				/* Se outra sentença tive a mesma densidade e a mesma soma de pesos, 
				continua-se pegando a primeira sentença. */
						if(total_ant < total)
							filho = aux_sent;

				/* Se ainda houver empate, tenta resolver pela CST. */
						else if(total_ant == total){

							if(!strcmp(desempate, "-unitaria")){

								int total_cst_aux_sent = 0, total_cst_filho = 0;

								for(int i = 0; i < aux_sent->getSizeVecArestaTotal(); i++){

									Edge *a = aux_sent->getAresta(i);

									total_cst_aux_sent += a->getSizeVecCST();
								}

								for(int i = 0; i < filho->getSizeVecArestaTotal(); i++){

									Edge *a = filho->getAresta(i);

									total_cst_filho += a->getSizeVecCST();
								}

								if(total_cst_aux_sent < total_cst_filho)
									filho = aux_sent;
							}

							if(!strcmp(desempate, "-ponderada")){

								int total_cst_aux_sent = 0, total_cst_filho = 0;

								for(int i = 0; i < aux_sent->getSizeVecArestaTotal(); i++){

									Edge *a = aux_sent->getAresta(i);

									total_cst_aux_sent += a->getTotalPesosCST();
								}

								for(int i = 0; i < filho->getSizeVecArestaTotal(); i++){

									Edge *a = filho->getAresta(i);

									total_cst_filho += a->getTotalPesosCST();
								}

								if(total_cst_aux_sent < total_cst_filho)
									filho = aux_sent;
							} 

					/* Se não der, pega a primeira sentença. */
						}
					}

					else {

						total_ant = total;
						maior_densidade = aux_densidade;
						filho = aux_sent;
					}
				}
			}
				
		}
	}

	return filho;
}

Sentence* Salton::depthFirstTopic(Sentence *parent){

	Sentence *child = NULL;

	if(parent != NULL && parent->getSizeVecArestaCortada()){

		float total, total_ant = -1.0f, maior_peso = -1;
		int aux_densidade, maior_densidade = -1;

		for(int i = 0; i < parent->getSizeVecArestaTotal(); i++){

			Edge *e = parent->getAresta(i);

			if(!e->getCortada() && parent->getTopic() == e->getS()->getTopic()){

				Sentence *aux_sent = e->getS();

				total = aux_sent->getTotalPesosCortados();
				aux_densidade = aux_sent->getSizeVecArestaCortada();

				if(!aux_sent->getSumario()){

					if(!temSimilar(aux_sent, this->vec_sent_seg_aux) && !temSimilar(aux_sent, this->vec_depth_topics)){

						if(aux_densidade >= maior_densidade){

							if(aux_densidade == maior_densidade){

								if(total_ant < total)
									child = aux_sent;
							}

							else {

								total_ant = total;
								maior_densidade = aux_densidade;
								child = aux_sent;
							}
						}
					}

					else
						aux_sent->setSumario(true);
				}
			}
		}
	}

	return child;
}

Sentence* Salton::bushyNodeTopic(int topic){

	float aux, maior, total, total_ant;
	Sentence *s = NULL, *s_aux = NULL, *ini;
	bool stop = false, flag_aux;

	while(!stop){

		ini = this->g->getIni();
		maior = -1;
		flag_aux = false;

		while(ini != NULL){

			if(ini->getTopic() == topic && !ini->getSumario()){

				flag_aux = true;
				aux = ini->getSizeVecArestaCortada();
				total = ini->getTotalPesosCortados();

				if(!ini->getSumario() && aux >= maior){

					if(aux == maior){

						if(total_ant < total)
							s = ini;
					}

					else {

						total_ant = total;
						maior = aux;
						s = ini;
					}
				}
			}

			ini = ini->getProx();
		}

		if(flag_aux){

			if(!temSimilar(s, this->vec_sent_seg_aux))
				stop = true;

			else
				s->setSumario(true);
		}

		else {

			stop = true;
			s->setSumario(true);
		}
	}

	return s;
}

Sentence* Salton::transNodeTopic(Sentence *s){

	Sentence *s_trans = NULL;
	float bigger = -1.0;

	for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

		Edge *e = s->getAresta(i);

		if(e->getS()->getTopic() == s->getTopic() &&
			e->getS()->getPosicaoOriginal() <= s->getPosicaoOriginal() &&
				e->getPeso() <= getSizeLimit() && e->getPeso() > bigger){
//cout << "\nPeso: " << e->getPeso();
//cout << "\nPosicao_Sentenca: " << e->getS()->getPosicaoOriginal(); 
//cout << "\nTexto_Sentenca: " << e->getS()->getOrigemTexto() << endl << endl;

				bigger = e->getPeso();
				s_trans = e->getS();
		}
	}

	return s_trans;
}

vector<Sentence*>* Salton::caminhoSegmentado(int taxa_compressao, char desempate[13], char type[10]){

	Topic *t;
	int global_clust_man = -1;

	if(!(t = new Topic(this->g))){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	if(!strcmp(type, "keywords")){

		t->findKeywords();
	
		vector<char*> *vec_keys = t->getVecKeys();

		for(int i = 0; i < vec_keys->size(); i++)
			cout << "Keyword: " << vec_keys->at(i) << endl;
	}

	else if(!strcmp(type, "all"))
		t->findSimilarTopics();

		/* Choices: unit || value  -- average || mm2 */ 
	else if(!strcmp(type, "cst"))
		t->findCSTRelations((char*)"value", (char*)"average");

	else if(!strcmp(type, "manual")){

		FILE *arq_clust_man;
		char char_clust_man[200], topic_clust_man[100], doc_clust_man[100], sent_clust_man[100];
		char num_clust_man[10];
	
		cout << "Cluster MANUAL: ";
		scanf(" %s", num_clust_man);

		strcpy(char_clust_man, "/home/ribaldo/Desktop/AgrupamentoDaRst/Agrupamento/C");
		//strcpy(char_clust_man, "../../../../AgrupamentoDaRst/Agrupamento/C");
		strcat(char_clust_man, num_clust_man);
								// Or 'man', depending on the file
		strcat(char_clust_man, "_agrupamento_auto.txt\0");
arq_clust_man = fopen("/home/ribaldo/Desktop/AgrupamentoDaRst/Agrupamento/C1_agrupamento_auto.txt", "r");
cout << "\nARQ: " << arq_clust_man << endl << endl;
		if(arq_clust_man){

			bool stop_clust_man = false;

			while(!stop_clust_man){

				if(!stop_clust_man && fscanf(arq_clust_man, "%s", topic_clust_man) > 0){

					if(!stop_clust_man && fscanf(arq_clust_man, "%s", doc_clust_man) > 0){
	
						if(!stop_clust_man && fscanf(arq_clust_man, "%s", sent_clust_man) > 0){

							if(atoi(topic_clust_man) > global_clust_man)
								global_clust_man = atoi(topic_clust_man);

							Sentence *sss = this->g->getIni();
							bool found_clust_man = false;

							while(sss != NULL && !found_clust_man){

//cout << sss->getTopic() << " " << sss->getOrigemTexto() << " " << sss->getPosicaoOriginal() << endl;
								if((sss->getOrigemTexto() == atoi(doc_clust_man) - 1) &&
										sss->getPosicaoOriginal() == atoi(sent_clust_man) - 1){

										sss->setTopic(atoi(topic_clust_man) - 1);
										found_clust_man = true;
								}

								sss = sss->getProx();
							}
						}

						else
							stop_clust_man = true;
					}

					else
						stop_clust_man = true;
				}

				else
					stop_clust_man = true;
			}

			fclose(arq_clust_man);
		}

		else {

			cout << endl << "ERROR! Arq Clust Man" << endl << endl;
			exit(EXIT_FAILURE);
		}
	}

	else {

		cout << endl << "ERROR! Segmented Path Type" << endl << endl;
		exit(EXIT_FAILURE);		
	}

	int	aux_taxa_compressao = 0, aux_index_topics = 0, global_topics = t->getGlobalTopics();
	bool stop = false, flag_summary, flag_in;
	Sentence *s, *s_trans;

	if(!strcmp(type, "manual"))
		global_topics = global_clust_man;

	//cout << "\n\nGlobal_num_topics: " << global_topics;
	//cout << "\nTaxa de Compressão: " << taxa_compressao;
	//cout << "\nSize_limit: " << getSizeLimit() << endl << endl;

	while(aux_index_topics < global_topics && !stop){

		flag_summary = false;
		flag_in = false;
		s = bushyNodeTopic(aux_index_topics);

//cout << "\nAUX: " << aux_index_topics << endl;

		if(s != NULL && !s->getSumario()){

			if(aux_index_topics > 0){

				s_trans = transNodeTopic(s);

				if(s_trans != NULL && !s_trans->getSumario()){
//cout << "\n\n--Trans--\n";
					flag_in = true;
					char **auxiliar = s_trans->getSentencaOriginal();

					if(aux_taxa_compressao + s_trans->getNumPalavras() > taxa_compressao){

						if(abs(aux_taxa_compressao + s_trans->getNumPalavras() - taxa_compressao) <= abs(taxa_compressao - aux_taxa_compressao)){

							for(int j = 0; j < s_trans->getNumPalavras(); j++){

								//cout << auxiliar[j] << " ";
								aux_taxa_compressao++;
							}

						//cout << endl;
							flag_summary = true;
							s_trans->setSumario(true);
							this->vec_sent_seg_aux->push_back(s_trans);
						}

						else
							stop = true;
					}

					else {

						for(int j = 0; j < s_trans->getNumPalavras(); j++){

							//cout << auxiliar[j] << " ";
							aux_taxa_compressao++;
						}

					//cout << endl;

						flag_summary = true;
						s_trans->setSumario(true);
						this->vec_sent_seg_aux->push_back(s_trans);
					}
				}

				if(!stop && flag_in){

//cout << "\n\n--Bushy(2)--\n";
					char **auxiliar = s->getSentencaOriginal();

					if(aux_taxa_compressao + s->getNumPalavras() > taxa_compressao){

						if(abs(aux_taxa_compressao + s->getNumPalavras() - taxa_compressao) <= abs(taxa_compressao - aux_taxa_compressao)){

							for(int j = 0; j < s->getNumPalavras(); j++){

								//cout << auxiliar[j] << " ";
								aux_taxa_compressao++;
							}

						//cout << endl;
							flag_summary = true;
							s->setSumario(true);
							this->vec_sent_seg_aux->push_back(s);
						}

						else
							stop = true;
					}

					else {

						for(int j = 0; j < s->getNumPalavras(); j++){

							//cout << auxiliar[j] << " ";
							aux_taxa_compressao++;
						}

					//cout << endl;

						flag_summary = true;
						s->setSumario(true);
						this->vec_sent_seg_aux->push_back(s);
					}
				}
			}

			else {

				if(!stop){

//cout << "\n\n--Bushy(1)--\n";
					char **auxiliar = s->getSentencaOriginal();

					if(aux_taxa_compressao + s->getNumPalavras() > taxa_compressao){

						if(abs(aux_taxa_compressao + s->getNumPalavras() - taxa_compressao) <= abs(taxa_compressao - aux_taxa_compressao)){

							for(int j = 0; j < s->getNumPalavras(); j++){

								//cout << auxiliar[j] << " ";
								aux_taxa_compressao++;
							}

						//cout << endl;
							flag_summary = true;
							s->setSumario(true);
							this->vec_sent_seg_aux->push_back(s);
						}

						else
							stop = true;
					}

					else {

						for(int j = 0; j < s->getNumPalavras(); j++){

							//cout << auxiliar[j] << " ";
							aux_taxa_compressao++;
						}

					//cout << endl;

						flag_summary = true;
						s->setSumario(true);
						this->vec_sent_seg_aux->push_back(s);
					}
				}
			}
		}

		if(aux_taxa_compressao >= taxa_compressao || !flag_summary)
			stop = true;

		aux_index_topics++;
	}

	// Depth-first

	Sentence *s_child = NULL;
	stop = false;
	int i_topics;

//cout << "\n\n--Depth-first--\n";
	while(!stop){

		i_topics = 0;
		flag_in = false;

		while(i_topics < this->vec_sent_seg_aux->size() && !stop){

			s_child = depthFirstTopic(this->vec_sent_seg_aux->at(i_topics));

			if(s_child != NULL && !s_child->getSumario()){
				
				flag_in = true;
				char **auxiliar = s_child->getSentencaOriginal();

				if(aux_taxa_compressao + s_child->getNumPalavras() > taxa_compressao){

					if(abs(aux_taxa_compressao + s_child->getNumPalavras() - taxa_compressao) <= abs(taxa_compressao - aux_taxa_compressao)){

						for(int j = 0; j < s_child->getNumPalavras(); j++){

							//cout << auxiliar[j] << " ";
							aux_taxa_compressao++;
						}

					//cout << endl;
						flag_summary = true;
						s_child->setSumario(true);
						this->vec_depth_topics->push_back(s_child);
					}

					else
						stop = true;
				}

				else {

					for(int j = 0; j < s_child->getNumPalavras(); j++){

						//cout << auxiliar[j] << " ";
						aux_taxa_compressao++;
					}

				//cout << endl;

					flag_summary = true;
					s_child->setSumario(true);
					this->vec_depth_topics->push_back(s_child);
				}
			}

			i_topics += 2;
		}

		if(!flag_in)
			stop = true;
	}

	for(int ii = 0; ii < this->vec_sent_seg_aux->size(); ii++)
		this->vec_sent_seg->push_back(this->vec_sent_seg_aux->at(ii));

	for(int ii = 0; ii < this->vec_depth_topics->size(); ii++)
		this->vec_sent_seg->push_back(this->vec_depth_topics->at(ii));

	//sortSentences();
	delete t;

	return this->vec_sent_seg;
}
