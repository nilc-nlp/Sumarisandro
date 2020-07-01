#include "../include/Util.h"

Util::Util(){

}

void Util::quicksort(float *vec, int left, int right){

	if (right > left){

		int pivotIndex = median3(vec,left,right);
		int pivotNewIndex = partition(vec, left, right, pivotIndex);

		quicksort(vec, left, pivotNewIndex-1);
		quicksort(vec, pivotNewIndex+1, right);
	}
}

int Util::median3(float *vec,int left,int right){

	int center = (left + right) / 2;

	if(vec[center] < vec[left])
		swap(vec[left],vec[center]);

	if(vec[right] < vec[left])
		swap(vec[left],vec[right]);

	if(vec[right] < vec[center])
		swap(vec[center],vec[right]);

	swap(vec[center],vec[right-1]);

return center;
}

int Util::partition(float *vec, int left, int right, int pivot){

	float pivotValue = vec[pivot];

	swap(vec[pivot], vec[right]);

	int storeIndex = left;

	for (int i = left; i < right; i++){

		if (vec[i] > pivotValue){

			swap(vec[i], vec[storeIndex]);
			storeIndex++;
		}
	}

	swap(vec[storeIndex], vec[right]);

return storeIndex;
}

void Util::swap(float &val1, float &val2){

	float temp = val1;
	val1 = val2;
	val2 = temp;
}

bool Util::existe(float peso, float *vetor, int ini, int fim){

	if(ini > fim)
		return false;

	int meio = (ini + fim) / 2;
	float aux = peso - vetor[meio];

	if(aux == (float) 0.00)
		return true;
	
	if(aux < (float) 0.00)
		return existe(peso, vetor, ++meio, fim);

	return existe(peso, vetor, ini, --meio);
}

/* Produto escalar entre dois vetores. */
int Util::dotProduct(int *vetA, int *vetB, int size){

	int dot = 0;

	for(int i = 0; i < size; i++)
		dot += vetA[i] * vetB[i];

return dot;
}

/* Módulo do vetor em questão. */
float Util::magnitude(int *vet, int size){

	float mag = 0;

	for(int i = 0; i < size; i++)
		mag += (vet[i] * vet[i]);

return sqrt(mag);
}

/* Neste método é determinada a frequência (quantas vezes aparece) de uma palavra em um vetor. */
int Util::frequencia(char X[MAX_NUM_LETRAS], char **C, int sizeC){

	int freq = 0;

	for(int i = 0; i < sizeC; i++){

		if(!strcmp(C[i], X))
			freq++;
	}

return freq;
}

/* Método que confere se uma palavra já está contida em um determinado vetor. */
bool Util::jahTem(char X[MAX_NUM_LETRAS], char **C, int count){

	if(frequencia(X, C, count))
		return true;

return false;
}

/* A similaridade de cosseno fornece um valor entre 0 e 1 que indica o quanto uma sentença é similar à outra (1 = igual, 0 = diferente). Neste caso
tais valores binários são substituídos pela frequência das palavras em sua sentença. */
float Util::cosineSimilarity(char *A[MAX_NUM_LETRAS], char *B[MAX_NUM_LETRAS], int sizeA, int sizeB){

	char **C, word[MAX_NUM_LETRAS];
	int *vetA, *vetB, j = 0;
	int count = 0, a = 0;

	C = new char* [sizeof(char) * (sizeA + sizeB)];

	if(!C)
		cout << endl << "ERROR! Memory Overflow" << endl << endl;

	for(int i = 0; i < (sizeA + sizeB); i++){

		if(!(C[i] = new char[sizeof(char) * MAX_NUM_LETRAS]))
			cout << endl << "ERROR! Memory Overflow" << endl << endl;
	}

	bool f = false;

/* Adiciona somente palavras distintas do primeiro vetor a um outro (neste caso, C). */
	for(int i = 0; i < sizeA; i++){

		if(!jahTem(A[i], C, a)){

			strcpy(C[a], A[i]);
			C[a][strlen(C[a])] = '\0';

			a++;
		}

		else count++;
	}

/* Adiciona somente palavras distintas distintas do segundo vetor a outro (neste caso, C). */
	for(int j = 0; j < sizeB; j++){

		if(!jahTem(B[j], C, a)){

			strcpy(C[a], B[j]);
			C[a][strlen(C[a])] = '\0';

			a++;
		}

		else count++;
	}

/* Criação de vetores que armazenam a frequência de cada palavra de cada sentença. */
	vetA = new int [sizeof(int) * ((sizeA + sizeB) - count)];
	vetB = new int [sizeof(int) * ((sizeA + sizeB) - count)];

	if(!vetA || !vetB){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);
	}

	for(int i = 0; i < (sizeA + sizeB) - count; i++)
		vetA[i] = vetB[i] = 0;

	for(int i = 0; i < (sizeA + sizeB) - count; i++){

		for(int j = 0; j < sizeA; j++){

			if(!strcmp(C[i], A[j]))
				vetA[i] += frequencia(A[j], C, (sizeA + sizeB)-count);
		}
	}

	for(int i = 0; i < (sizeA + sizeB) - count; i++){

		for(int j = 0; j < sizeB; j++){

			if(!strcmp(C[i], B[j]))
				vetB[i] += frequencia(B[j], C, (sizeA + sizeB)-count);
		}
	}

	int dot = dotProduct(vetA, vetB, (sizeA + sizeB) - count);
	float magA, magB;

/* Se caso o produto escalar dos vetores não for nulo (completamente diferentes) a magnitude é calculada,
caso contrário é determinado um valor inteiro qualquer. */
	if(dot){

		magA = magnitude(vetA, (sizeA + sizeB) - count);
		magB = magnitude(vetB, (sizeA + sizeB) - count);
	}
	
	else {

	/* Valor qualquer. */
		magA = magB = 1;
	}

	delete vetA;
	delete vetB;

	for(int i = 0; i < (sizeA + sizeB) - count; i++)
		delete C[i];

/* Fórmula da similaridade de cosseno. */
return (dot / (magA * magB));
}

float Util::precisionRecall(float intersect, float num_sent){

	return (intersect / num_sent);
}

float Util::calcularF_measure(float intersect, float num_sent_aut, float num_sent_ext){

	if(intersect)
		return ((2.0f * precisionRecall(intersect, num_sent_aut) * precisionRecall(intersect, num_sent_ext)) / 
					(precisionRecall(intersect, num_sent_aut) + precisionRecall(intersect, num_sent_ext)));

	return 0.0f;
}

int Util::numSentExt(vector<Sentence*> *vec, int *intersect){

	FILE *arq;

	char dir[MAX_NUM_LETRAS];
	
	strcpy(dir, "../Extracts/C");

	char num_cluster[MAX_NUM_LETRAS];

	cout << "\nNúmero do cluster [1, 50]: ";
	cin >> num_cluster;

	strcat(dir, num_cluster);
	strcat(dir, ".txt\0");

	if(arq = fopen(dir, "r")){

		int num_sent_ext = 0, index = 0, old_index = 0;
		vector<char*> *vec_ext = new vector<char*>();
		char *str_aux;

		while(!feof(arq)){

			bool flag = false;

			str_aux = NULL;

			if(str_aux = (char*) malloc(sizeof(char) * MAX_NUM_LETRAS)){

				if(fscanf(arq, "%s", str_aux) >= 0){

					bool flag_aux = false;

						if(str_aux[strlen(str_aux)-1] == '>')
							flag_aux = true;

					if(str_aux[0] != '<' && !flag_aux){

						vec_ext->push_back(str_aux);
						//fprintf(x, "%s ", str_aux);
						index++;
					}

					if(flag_aux){

						for(int i = 0; i < vec->size(); i++){

							Sentence *s = vec->at(i);
							char **auxiliar = s->getSentencaOriginal();

							if(s->getNumPalavras() == vec_ext->size() - old_index){
		
								bool flag_equal = true;

								for(int j = 0; j < s->getNumPalavras() && flag_equal; j++){

									if(strcmp(auxiliar[j], vec_ext->at(j+old_index)))
										flag_equal = false;
								}

								if(flag_equal)
									(*intersect) += 1;
							}
						}

						old_index += index;
						index = 0;
					}
				}
			}

			strcat(str_aux, "\0");

			for(int i = 0; str_aux[i] != '\0'; i++){

				if(str_aux[i] == '<')
					flag = true;
			}

			if(flag)
				num_sent_ext++;
		}

		fclose(arq);

		delete vec_ext;

		return num_sent_ext;
	}

	else {

		cout << endl << "ERROR! Couldn't open Extractive Summary file" << endl << endl;
		exit(1);
	}
}

void Util::gerarBaseline(Graph *g, int total){

	FILE *arq_summary;
///////////////////////////////////////////////////
	char num_cluster[5], nome_baseline[1000];
	cout << "Numero do cluster: ";
	cin >> num_cluster;

	strcpy(nome_baseline, "../Summaries/C");
	strcat(nome_baseline, num_cluster);
	strcat(nome_baseline, ".baseline\0");
///////////////////////////////////////////////////	
	if(arq_summary = fopen(nome_baseline, "w+")){

		int aux_taxa_compressao = 0, jah_tem[g->getNumNos()];
		bool stop = false;

		int count_i;
		
		for(count_i = 0; count_i < g->getNumNos(); count_i++)
			jah_tem[count_i] = -1;

		while(!stop){

			int index;
			bool tem = true;

			while(tem){

				index = rand() % g->getNumNos();

				for(count_i = 0; jah_tem[count_i] != -1; count_i++){
	
					if(jah_tem[count_i] == index)
						tem = false;
				}

				if(!tem)
					tem = true;

				else
					tem = false;
			}

			jah_tem[count_i] = index;

			int aux_index = 0;

			Sentence *s = g->getIni();

			while(aux_index++ < index)
				s = s->getProx();

			char **auxiliar = s->getSentencaOriginal();

			if(aux_taxa_compressao + s->getNumPalavras() > total){

				if(abs(aux_taxa_compressao + s->getNumPalavras() - total) <= abs(total - aux_taxa_compressao)){

					for(int j = 0; j < s->getNumPalavras(); j++){

						fprintf(arq_summary, "%s ", auxiliar[j]);
						printf("%s ", auxiliar[j]);
						aux_taxa_compressao++;
					}

					printf("(S%d)\n", s->getPosicaoVirtual()+1);
				}

				else
					stop = true;
			}

			else {
				for(int j = 0; j < s->getNumPalavras(); j++){

					fprintf(arq_summary, "%s ", auxiliar[j]);
					printf("%s ", auxiliar[j]);
					aux_taxa_compressao++;
				}

				printf("(S%d)\n", s->getPosicaoVirtual()+1);
			}
		}

		fclose(arq_summary);
	}

	else
		cout << endl << "Couldn't open baseline file" << endl << endl;
}

bool Util::letter(char word[MAX_NUM_LETRAS]){

	if(!strcmp(word, "Á"))
		strcpy(word, "á");

	else if(!strcmp(word, "É"))
		strcpy(word, "é");

	else if(!strcmp(word, "Í"))
		strcpy(word, "í");

	else if(!strcmp(word, "Ó"))
		strcpy(word, "ó");

	else if(!strcmp(word, "Ú"))
		strcpy(word, "ú");

	else if(!strcmp(word, "Â"))
 		strcpy(word, "â");

	else if(!strcmp(word, "Ê"))
		strcpy(word, "ê");

	else if(!strcmp(word, "Î"))
		strcpy(word, "î");

	else if(!strcmp(word, "Ô"))
		strcpy(word, "ô");

	else if(!strcmp(word, "Û"))
		strcpy(word, "û");

	else if(!strcmp(word, "Ã"))
		strcpy(word, "ã");

	else if(!strcmp(word, "Õ"))
		strcpy(word, "õ");

	else if(!strcmp(word, "À"))
		strcpy(word, "à");

	else if(!strcmp(word, "È"))
		strcpy(word, "è");

	else if(!strcmp(word, "Ì"))
		strcpy(word, "ì");

	else if(!strcmp(word, "Ò"))
		strcpy(word, "ò");
	
	else if(!strcmp(word, "Ù"))
		strcpy(word, "ù");

	else if(!strcmp(word, "Ä"))
		strcpy(word, "ä");

	else if(!strcmp(word, "Ë"))
		strcpy(word, "ë");

	else if(!strcmp(word, "Ï"))
		strcpy(word, "ï");

	else if(!strcmp(word, "Ö"))
		strcpy(word, "ö");

	else if(!strcmp(word, "Ü"))
		strcpy(word, "ü");

	else if(!strcmp(word, "Ç"))
		strcpy(word, "ç");

	else if(!strcmp(word, "Ñ"))
		strcpy(word, "ñ");

	else if(!strcmp(word, "ª"))
		strcpy(word, "ª");

	else return false;

	strcat(word, "\0");

	return true;
}

