#include "../include/Topic.h"

Topic::Topic(Graph *g){

	this -> g = g;
	this -> vec_seg = new vector<Segment*>();
	this -> vec_keys = new vector<char*>();
	this -> vec_change_topics = new vector<char*>();
	this -> vec_cst = new vector<char*>();
	this -> vec_cst_aux = new vector<char*>();
	this -> vec_topics = NULL;
	
	if(!vec_seg || !vec_keys || !vec_change_topics || !vec_cst || !vec_cst_aux){

		cout << endl << "ERROR! Memory Overflow!" << endl << endl;
		exit(1);
	}
}

Topic::~Topic(){

	int size;

	if(this -> vec_topics != NULL)
		delete[] this -> vec_topics;

	size = this -> vec_seg -> size();

	for(int i = 0; i < size; i++)
		this -> vec_seg -> pop_back();

	delete this -> vec_seg;

	size = this -> vec_keys -> size();

	for(int i = 0; i < size; i++)
		this -> vec_keys -> pop_back();

	delete this -> vec_keys;

	size = this -> vec_change_topics -> size();

	for(int i = 0; i < size; i++)
		this -> vec_change_topics -> pop_back();

	delete this -> vec_change_topics;

	size = this -> vec_cst -> size();

	for(int i = 0; i < size; i++)
		this -> vec_cst -> pop_back();

	delete this -> vec_cst;

	size = this -> vec_cst_aux -> size();

	for(int i = 0; i < size; i++)
		this -> vec_cst_aux -> pop_back();

	delete this -> vec_cst_aux;
}

int Topic::fillVector(vector<char*> *vec_words, Sentence *s){

	for(int i = 0; i < s -> getNumPalavrasStemmStop(); i++)
			vec_words -> push_back((s -> getSentenca())[i]);

	return s -> getNumPalavrasStemmStop();
}

void Topic::addSegment(Segment *segment){

	this -> vec_seg -> push_back(segment);
}

vector<char*>* Topic::getVecKeys(){

		return this -> vec_keys;
}

void Topic::addKeyword(int topic, char **words, int frequency[100000]){

	char *keyword = (char*) malloc(sizeof(char) * (MAX_NUM_LETRAS + 1000));
	int bigger = -1;


	if(sprintf(keyword, "%d", topic) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(1);
	}
//cout << "\n\n------------------------------------------------------------\n" << "Topic: " << topic << endl;
	for(int i = 0; frequency[i] != -1; i++){

//cout << "Word: " << words[i] << "  - Frequency: " << frequency[i] << endl;

		if(frequency[i] > bigger)
			bigger = frequency[i];
	}
//cout << "\n\n------------------------------------------------------------\n";
	for(int i = 0; frequency[i] != -1; i++){

		if(frequency[i] == bigger){
		
			strcat(keyword, "_");
			strcat(keyword, words[i]);
		}
	}

	strcat(keyword, "\0");

	vec_keys -> push_back(keyword);
//cout << "\n----------\n" << "Topic: " << topic << endl;
//	for(int i = 0; i < vec_keys -> size(); i++)
//		cout << "Keywords(internal): " << vec_keys -> at(i) << endl;

}

void Topic::findSimilarTopics(){

	vector<char*> *vec_words;
	vector<int*> *vec_freq1, *vec_freq2;

	if(!(vec_words = new vector<char*>()) || 
		!(vec_freq1 = new vector<int*>()) || 
			!(vec_freq2 = new vector<int*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);
	}

	char **A, **B;
	int size_A, size_B;

	Sentence *s = this -> g -> getIni();
	Sentence *prev_s = s;

	int cur_topic = s -> getTopic(), prev_topic, first_change = 0, cont;
	char **aux;

	while(prev_s != NULL){
			
		while(s != NULL && cur_topic == s -> getTopic()){

			fillVector(vec_words, s);

cout << "\nSentenca: ";
for(int indexx = 0; indexx < s -> getNumPalavrasStemmStop(); indexx++)
	cout << (s -> getSentenca())[indexx] << endl;
cout << endl << endl;
			s = s -> getProx();
		}

		prev_topic = cur_topic;
cout << "\nPrev-Topic: " << prev_topic << endl << endl;
 
cout << "VEC_WORDS:\n";
for(int i = 0; i < vec_words -> size(); i++)
	cout << vec_words -> at(i) << endl;

		while(s != NULL){

			cont = 0;
			cur_topic = s -> getTopic();

cout << "\nCur-Topic: " << cur_topic << endl << endl;

			while(s != NULL && cur_topic == s -> getTopic()){

				cont += fillVector(vec_words, s);
				s = s -> getProx();
			}

cout << "\nSize_VEC_WORDS: " << vec_words -> size() << endl;
cout << "VEC_WORDS:\n";
for(int i = 0; i < vec_words -> size(); i++)
	cout << vec_words -> at(i) << endl;

			size_A = size_B = 0;

	cout << "\n\nCONT: " << cont << endl;
	//cout << "VEC_FREQ1_SIZE: " << vec_freq1 -> size() << endl;
	cout << "VEC_WORDS_SIZE: " << vec_words -> size() << endl << endl;

			size_A = vec_words -> size() - cont;
			size_B = cont;

			A = new char* [sizeof(char) * size_A];
			B = new char* [sizeof(char) * size_B];

			if(!A || !B){

				cout << endl << "ERROR! Memory Overflow" << endl << endl;
				exit(1);
			}

			for(int aaa = 0; aaa < size_A; aaa++){

				if(!(A[aaa] = new char[sizeof(char) * MAX_NUM_LETRAS])){

					cout << endl << "ERROR! Memory Overflow" << endl << endl;
					exit(1);
				}
			}

			for(int bbb = 0; bbb < size_B; bbb++){

				if(!(B[bbb] = new char[sizeof(char) * MAX_NUM_LETRAS])){

					cout << endl << "ERROR! Memory Overflow" << endl << endl;
					exit(1);
				}
			}

cout << "\nSIZE_A: " << size_A << endl;
cout << "SIZE_B: " << size_B << endl;

			for(int aaa = 0; aaa < size_A; aaa++){

				strcpy(A[aaa], vec_words -> at(aaa));
				cout << "A[" << aaa << "]: " << A[aaa] << endl;
			}

			for(int bbb = 0; bbb < size_B; bbb++){
				strcpy(B[bbb], vec_words -> at(bbb + size_A));
				cout << "B[" << bbb << "]: " << B[bbb] << endl;

			}

			float cosine = Util().cosineSimilarity(A, B, size_A, size_B);

cout << "\nCosine: " << cosine << endl << endl;

			//if(cosine >= 0.7f){
//cout << "\nIS BIGGER!\n";
				changeTopic(prev_topic, cur_topic, cosine);
	//		}

			removeFromVector(cont, vec_words);
		}

		while(prev_s != NULL && prev_s -> getTopic() == prev_topic)
			prev_s = prev_s -> getProx();

		if(prev_s != NULL){

			cur_topic = prev_s -> getTopic();
			s = prev_s;
		
			while(s != NULL && s -> getTopic() == cur_topic)
				s = s -> getProx();
		}

		removeFromVector(vec_words -> size(), vec_words);
		
cout << "\nS: " << s;
cout << "\nPREV: " << prev_s << endl << endl;

		s = prev_s;
	}

	clusteringTopics();
}

void Topic::changeTopic(int prev_topic, int cur_topic, float cosine){

	char *tmp;

	if(!(tmp = new char [sizeof(char) * 10000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);
	}

	if(sprintf(tmp, "%d_%d_%f", prev_topic, cur_topic, cosine) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(1);
	}

	cout << "\n-----------------------------------------------TOPIC CHANGED => " << tmp << endl;

	this -> vec_change_topics -> push_back(tmp);

cout << "\n-----------------------------------------------TOPIC CHANGED => " << this -> vec_change_topics -> at(this -> vec_change_topics -> size()-1) << endl;
}

float Topic::similarity(char *keywords_1, char *keywords_2){

	int num_keys_1 = 0, num_keys_2 = 0, index_1 = 0, index_2 = 0, i, j;
	char aux_1[MAX_NUM_LETRAS], aux_2[MAX_NUM_LETRAS];
	bool has_more_1 = false, has_more_2 = false;
	float sim = 0.0;

	for(i = 2; keywords_1[i] != '\0'; i++){

		if(keywords_1[i] == '_'){

			has_more_1 = true;
			aux_1[index_1] = '\0';

	//cout << "topico 0: " << aux_1 << endl;

			has_more_2 = false;

			for(j = 2; keywords_2[j] != '\0'; j++){

				if(keywords_2[j] == '_'){

					has_more_2 = true;
					aux_2[index_2] = '\0';

			//cout << "topico 1: " << aux_2 << endl;

					if(!strcmp(aux_1, aux_2))
						sim++;

					index_2 = 0;
				}

				else {

					aux_2[index_2] = keywords_2[j];
					index_2++;
				}
			}

			if(!has_more_2 || keywords_2[j] == '\0'){

				aux_2[index_2] = '\0';
				
				if(!strcmp(aux_1, aux_2))
					sim++;

				index_2 = 0;
			}

			has_more_1 = false;
			index_1 = 0;
		}

		else {
			
			aux_1[index_1] = keywords_1[i];
			index_1++;
		}
	}

	if(!has_more_1 || keywords_1[i] == '\0'){

		has_more_2 = false;
		aux_1[index_1] = '\0';
	//cout << "topico 0: " << aux_1 << endl;

		for(j = 2; keywords_2[j] != '\0'; j++){

			if(keywords_2[j] == '_'){

				has_more_2 = true;
				aux_2[index_2] = '\0';

	//cout << "topico 1: " << aux_2 << endl;	

				if(!strcmp(aux_1, aux_2))
					sim++;
				
				index_2 = 0;
			}

			else {

				aux_2[index_2] = keywords_2[j];
				index_2++;
			}
		}

		if(!has_more_2 || keywords_2[j] == '\0'){
			
			aux_2[index_2] = '\0';
//cout << "topic 1: " << aux_2 << endl;

			if(!strcmp(aux_1, aux_2))
				sim++;
		}
	}

	for(i = 2; keywords_1[i] != '\0'; i++){

		if(keywords_1[i] == '_')
			num_keys_1++;
	}

	num_keys_1++;

	for(i = 2; keywords_2[i] != '\0'; i++){

		if(keywords_2[i] == '_')
			num_keys_2++;
	}

	num_keys_2++;

cout << endl << "Comparação entre os tópicos " << keywords_1[0] << " e " << keywords_2[0] << endl;
cout << "Numero de palavras do tópico " << keywords_1[0] << ": " << num_keys_1 << endl;
cout << "Numero de palavras do tópico " << keywords_2[0] << ": " << num_keys_2 << endl;
cout << "Numero de palavras iguais: " << sim << endl;

	int bigger;

	if(num_keys_1 >= num_keys_2)
		bigger = num_keys_1;

	else
		bigger = num_keys_2;

cout << "Similaridade entre eles: " << (sim / ((float) bigger)) << endl << endl; 
 
	return sim / ((float) bigger);
}

float Topic::calculateTresholdKeywords(char *type){

	int count = 0;
	float sum = 0.0, sim;
	float bigger = 0.0, lesser = 1.0;

	for(int i = vec_keys -> size()-1; i >= 1; i--){

		for(int j = i-1; j >= 0; j--){

			sim = similarity(vec_keys -> at(i), vec_keys -> at(j));

	cout << "\nSIM: " << sim << endl;

			if(!strcmp(type, "average")){

				count++;
				sum += sim;
			}

			if(!strcmp(type, "mm2")){

				if(sim > bigger)
					bigger = sim;

				if(sim < lesser)
					lesser = sim;
			}
		}
	}

	if(!strcmp(type, "average"))
		return sum / ((float) count);	

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2.0;

	return 0.0;
}

/* Cluster topics depending on their keywords. */
void Topic::clusteringKeywords(){

	Sentence *s;

	int from = -1, to = -1;
	float treshold = calculateTresholdKeywords((char*)"average");

cout << "\nTRESHOLD: " << treshold << endl << endl;

	for(int i = vec_keys -> size()-1; i >= 1; i--){

		from = -1;
		to = -1;

		for(int j = i-1; j >= 0; j--){

			float sim = similarity(vec_keys -> at(i), vec_keys -> at(j));
cout << "\nSimilarity between \"" << vec_keys -> at(i) << "\" and \"" << vec_keys -> at(j) << "\" is \"" << sim << "\""<< endl;
			if(sim >= treshold){

				from = i;
				to = j;

				s = this -> g -> getIni();

				bool found = false;
				Sentence *s_aux = NULL;

				while(s != NULL && !found){

					if(s -> getTopic() == to){

						s_aux = s;
						found = true;
					}

					else
						s = s -> getProx();
				}

cout << "\nBEFORE\n";
s = this -> g -> getIni();
while(s != NULL){

	cout << "\nTopic: " << s -> getTopic()+1;
	s = s -> getProx();
}
cout << "\nSSSS: " << s;

				s = this -> g -> getIni();
				bool stop = false;

				while(s != NULL && !stop){
cout << "\nTopic: " << s -> getTopic() << " -- Origem: " << s -> getOrigemTexto() << endl;

					if(s -> getTopic() == from){ 

						if(s -> getOrigemTexto() != s_aux -> getOrigemTexto() ||
							(s -> getOrigemTexto() == s_aux -> getOrigemTexto() && abs(s -> getTopic() - s_aux -> getTopic()) != 1)){

	cout << "\nOrigem s: " << s -> getOrigemTexto() << " -- Origem s_aux: " << s_aux -> getOrigemTexto() << endl;
	cout << "Topic s: " << s -> getTopic() << " -- Topic s_aux: " << s_aux -> getTopic() << " -- Abs: " << abs(s -> getTopic() - s_aux -> getTopic()) << endl << endl;


			cout << "\nTopic change from \"" << from << "\" to \"" << to << "\""<< endl;

							s -> setTopic(to);
						}

						else
							stop = true;
					}

					s = s -> getProx();
				}
cout << "\nAFTER\n";
s = this -> g -> getIni();
while(s != NULL){

	cout << "\nTopic: " << s -> getTopic()+1;
	s = s -> getProx();
}
			}
		}	
	}
}

float Topic::calculateTresholdTopics(char *type){

	float sum = 0.0, value = 0.0;
	float bigger = 0.0, lesser = 1.0;

	for(int i = 0; i < this -> vec_change_topics -> size(); i++){

		int j, k, int_aux = 0;
		char aux[10000];

		for(j = 0; (this -> vec_change_topics -> at(i))[j] != '_'; j++);
		for(k = j+1; (this -> vec_change_topics -> at(i))[k] != '_'; k++);
		
		for(int m = k+1; (this -> vec_change_topics -> at(i))[m] != '\0'; m++)
			aux[int_aux++] = (this -> vec_change_topics -> at(i))[m];

		aux[int_aux] = '\0';
		value = atof(aux);

//cout << "\nVALUE: " << value << endl;

		if(!strcmp(type, "average"))
			sum += value;

		if(!strcmp(type, "mm2")){

			if(value > bigger)
				bigger = value;

			if(value < lesser)
				lesser = value;
		}
	}

	if(!strcmp(type, "average"))
		return sum / ((float) this -> vec_change_topics -> size());

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2.0;

	return 0.0;
}

/* Cluster topics depending on the cosine similarity between groups */
void Topic::clusteringTopics(){

	Sentence *s = this -> g -> getIni();
	char aux[10000];
	int from, to;
	float value, treshold = calculateTresholdTopics((char*)"average");

cout << "\nTRESHOLD: " << treshold << endl << endl;

	for(int i = 0; i < this -> vec_change_topics -> size(); i++)
		cout << "\nvec_topics_before: " << vec_change_topics -> at(i);

	for(int i = this -> vec_change_topics -> size()-1; i >= 0; i--){

		int j;

cout << "\nvec: " << vec_change_topics -> at(i);

		for(j = 0; (this -> vec_change_topics -> at(i))[j] != '_'; j++)
			aux[j] = (this -> vec_change_topics -> at(i))[j];

		aux[j] = '\0';
		to = atoi(aux);

		int k, i_aux = 0;

		for(k = j+1; (this -> vec_change_topics -> at(i))[k] != '_'; k++)
			aux[i_aux++] = (this -> vec_change_topics -> at(i))[k];

		aux[i_aux] = '\0';
		from = atoi(aux);

		i_aux = 0;

		for(int m = k+1; (this -> vec_change_topics -> at(i))[m] != '\0'; m++)
			aux[i_aux++] = (this -> vec_change_topics -> at(i))[m];

		aux[i_aux] = '\0';
		value = atof(aux);

		if(value >= treshold && from != to){

		cout << "\n(((((((TENTANTTIIIVAAA)))))) FROM: " << from << " -- TO: " << to << endl << endl;
		getchar();
			bool found = false;
			Sentence *s = this -> g -> getIni(), *s_aux = NULL;

			while(s != NULL && !found){

				if(s -> getTopic() == to){

					s_aux = s;
					found = true;
				}

				else
					s = s -> getProx();
			}

			s = this -> g -> getIni();
			bool stop = false;

			while(s != NULL && !stop){

				if(s -> getTopic() == from){

					if(s -> getOrigemTexto() != s_aux -> getOrigemTexto() ||
							(s -> getOrigemTexto() == s_aux -> getOrigemTexto() && abs(s -> getTopic() - s_aux -> getTopic()) != 1)){
					
cout << "\n(((CONSEGUIU))) S->FROM: " << s -> getTopic() << " -- S->TO: " << to << endl;
getchar();
	
						s -> setTopic(to);
					}

					else {cout << "\nNÃO CONSEGUIUUUU!!!\n";getchar();
						stop = true;}
				}

				s = s -> getProx();
			}

			/* Mudar em todo o vec_change_topics */
			for(int ii = this -> vec_change_topics -> size()-1; ii >= 0; ii--){

				for(j = 0; (this -> vec_change_topics -> at(ii))[j] != '_'; j++);

				char aux_to[10000];

				if(sprintf(aux_to, "%d", to) < 0){

					cout << endl << "ERROR! Conversion Failed" << endl << endl;
					exit(1);
				}

				k, i_aux = 0;

				for(k = j+1; (this -> vec_change_topics -> at(ii))[k] != '_'; k++)
					aux[i_aux++] = (this -> vec_change_topics -> at(ii))[k];

				aux[i_aux] = '\0';
				
				if(atoi(aux) == from){

					i_aux = 0;

					for(int m = k+1; (this -> vec_change_topics -> at(ii))[m] != '\0'; m++)
						aux[i_aux++] = (this -> vec_change_topics -> at(ii))[m];

					aux[i_aux] = '\0';

					for(j = 0; (this -> vec_change_topics -> at(ii))[j] != '_'; j++);

					(this -> vec_change_topics -> at(ii))[j+1] = '\0';
					strcat((this -> vec_change_topics -> at(ii)), aux_to);
					strcat((this -> vec_change_topics -> at(ii)), "_"); 
					strcat((this -> vec_change_topics -> at(ii)), aux);
					strcat((this -> vec_change_topics -> at(ii)), "\0");

cout << "\nNEW_VEC ------> " << (this -> vec_change_topics -> at(ii)) << endl << endl;
				}
			}
//////////////////////////////////////////////////////////////
				s = this -> g -> getIni();

				while(s != NULL){

					cout << "Topic: " << s -> getTopic() << endl;
					s = s -> getProx();
				}
/////////////////////////////////////////////////////////////
		}
	}
cout << endl << endl;
	s = this -> g -> getIni();

	while(s != NULL){

		cout << "Topic: " << s -> getTopic()+1 << endl;
		s = s -> getProx();
	}
}


void Topic::removeFromVector(int cont, vector<char*> *vec){

	for(int i = 0; i < cont && vec -> size() >= 0; i++)
		vec -> pop_back();
}

int Topic::hasWord(char **words, char *aux){
//cout << "\n\nComparison:\n";
	for(int i = 0; strcmp(words[i], "end_of_matrix\0"); i++){

//cout << "Words[" << i << "]: " << words[i] << " -- Aux: " << aux << endl;

		if(!strcmp(words[i], aux))
			return i;
	}

	return -1;
}

int Topic::averageKeys(char *type){

	int bigger = 0, lesser = 10000, sum = 0;

	for(int i = 0; i < this -> vec_keys -> size(); i++){

		char *aux = this -> vec_keys -> at(i);
		bool flag = false;
		int num = 1;
//cout << "\nAUX: " << aux << endl;
		for(int j = 0; aux[j] != '\0'; j++){
//cout << "\neach: " << aux[j];
			if(aux[j] == '_'){

		/* Skip the topic identification number */
				if(flag)
					num++;

				else
					flag = true;
			}
		}

//cout << "\nNUM: " << num << endl << endl;

		if(!strcmp(type, "average"))
			sum += num;

		if(!strcmp(type, "mm2")){

			if(num > bigger)
				bigger = num;

			if(num < lesser)
				lesser = num;
		}
	}

	if(!strcmp(type, "average"))
		return sum / this -> vec_keys -> size();

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2;

	return 0;
}

void Topic::roundWords(int number){

	int num_aux;
	bool flag, stop;
	char *aux;

cout << "\n---------------BEFORE!-------------\n";
for(int i = 0; i < this -> vec_keys -> size(); i++)
	cout << "\nVEC: " << this -> vec_keys -> at(i);
cout << endl;
	for(int i = 0; i < this -> vec_keys -> size(); i++){

		num_aux = 1;
		flag = false;
		stop = false;
		aux = this -> vec_keys -> at(i);

		for(int j = 0; aux[j] != '\0' && !stop; j++){

			if(aux[j] == '_'){

				if(flag)
					num_aux++;

				else
					flag = true;
			}

			if(num_aux > number){

				aux[j] = '\0';
				stop = true;
			}
		}
	}

cout << "\n---------------AFTER!-------------\n";
for(int i = 0; i < this -> vec_keys -> size(); i++)
	cout << "\nVEC: " << this -> vec_keys -> at(i);
cout << endl;
}

void Topic::findKeywords(){

	char **words;
	int frequency[100000];

	if(!(words = new char* [sizeof(char) * 100000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);	
	}

	for(int i = 0; i < 100000; i++){

		if(!(words[i] = new char[sizeof(char) * MAX_NUM_LETRAS])){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(1);
		}
	}
	
	Sentence *s = this -> g -> getIni();
	int cur_topic = s -> getTopic(), index = 0;

	strcpy(words[0], "end_of_matrix\0");
	frequency[0] = -1;

	while(s != NULL){
		
//cout << endl << endl;
//cout << cur_topic << endl;

//for(int i = 0; i < s -> getNumPalavrasStemmStop(); i++)
//	cout << (s->getSentenca())[i] << " ";

		if(cur_topic != s -> getTopic()){

//cout << "TOPIC CHANGE: " << cur_topic << " -> " << s -> getTopic() << endl;
			addKeyword(cur_topic, words, frequency);

			cur_topic = s -> getTopic();
			strcpy(words[0], "end_of_matrix\0");
			frequency[0] = -1;
			index = 0;
		}
	
//cout << "\nNum_palavras: " << s -> getNumPalavrasStemmStop() << endl;
	
		for(int i = 0; i < s -> getNumPalavrasStemmStop(); i++){

//cout << "\nWord: " << aux[i] << "\n";

			int has_word = hasWord(words, s -> getWord(i));

			if(has_word == -1){

//cout << "Nao tinha (" << words[index] << ")\n";
				strcpy(words[index], s -> getWord(i));
				frequency[index] = 1;
//cout << "\nfrequency BEFORE: " << frequency[index] << " -- words: " << words[index] <<  endl;
//cout << "index BEFORE: " << index << endl;

				strcpy(words[index+1], "end_of_matrix\0");
				frequency[index+1] = -1;

				index++;

//cout << "index AFTER: " << index;
//cout << "\nVEC_WORDS:\n";

//for(int j = 0; j <= index; j++)
//	cout << "J: " << j << " -- " << words[j] << "(freq: " << frequency[j] << "); ";

			}

			else {

//			cout << "\nHEREEEE (TRUE)!!\n";
//			cout << "INDICE DA PALAVRA: " << has_word << endl;
//			cout << "WORD MATCHED: " << words[has_word] << endl;
//			cout << "FREQ MATCHED: " << frequency[has_word] << endl;
				frequency[has_word]++;
			
//			cout << "FREQ AFTEEEERR!: " << frequency[has_word] << endl;
			}
		}

		s = s -> getProx();
	}

	/* Add last topic */
	addKeyword(cur_topic, words, frequency);

	roundWords(averageKeys((char*)"average"));

	for(int i = 0; i < 100000; i++)
		delete words[i];

	delete words;

	clusteringKeywords();
	findNewKeywords();
}

int Topic::getNumTopics(){

	Sentence *s = this -> g -> getIni();
	int cur_topic = s -> getTopic(), num_topics = 0;

	while(s != NULL){

		if(cur_topic != s -> getTopic()){

			cur_topic = s -> getTopic();
			num_topics++;
		}

		s = s -> getProx();
	}

	return ++num_topics;
}

void Topic::getTopics(){

	Sentence *s = this -> g -> getIni();
	int num_topics = getNumTopics(), index = 0;
	bool flag;

	this -> vec_topics[index] = s -> getTopic();
	this -> vec_topics[++index] = -1;

	while(s != NULL){

		flag = false;

		for(int i = 0; this -> vec_topics[i] != -1 && !flag; i++){

			if(s -> getTopic() == this -> vec_topics[i])
				flag = true;
		}

		if(!flag){

			this -> vec_topics[index] = s -> getTopic();
			this -> vec_topics[++index] = -1;
		}

		s = s -> getProx();
	}
}

void Topic::findNewKeywords(){

	char **words;
	int frequency[100000];

	if(!(words = new char* [sizeof(char) * 100000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);	
	}

	for(int i = 0; i < 100000; i++){

		if(!(words[i] = new char[sizeof(char) * MAX_NUM_LETRAS])){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(1);
		}
	}
	
	Sentence *s = this -> g -> getIni();
	int cur_topic, num_topics = getNumTopics(), index = 0;
	bool flag;

	if(!(this -> vec_topics = new int[sizeof(int) * num_topics])){

		cout << endl << "ERROR! Memory Overflow!" << endl << endl;
		exit(1);
	}

cout << "\nNUM_TOPICS: " << num_topics-1 << endl;

	getTopics();

	removeFromVector(this -> vec_keys -> size(), this -> vec_keys);

	strcpy(words[0], "end_of_matrix\0");
	frequency[0] = -1;

	for(int i = 0; this -> vec_topics[i] != -1; i++){

		cur_topic = this -> vec_topics[i];
		s = this -> g -> getIni();

cout << "\nCur_topic: " << cur_topic << endl << endl;
	
		while(s != NULL){

			if(s -> getTopic() == cur_topic){
	
				for(int i = 0; i < s -> getNumPalavrasStemmStop(); i++){

					int has_word = hasWord(words, s -> getWord(i));

					if(has_word == -1){

						strcpy(words[index], s -> getWord(i));
						frequency[index] = 1;

						strcpy(words[index+1], "end_of_matrix\0");
						frequency[index+1] = -1;

						index++;
					}

					else 
						frequency[has_word]++;
				}
			}

			s = s -> getProx();
		}

		addKeyword(cur_topic, words, frequency);

		strcpy(words[0], "end_of_matrix\0");
		frequency[0] = -1;
		index = 0;
	}

	roundWords(averageKeys((char*)"average"));

	for(int i = 0; i < 100000; i++)
		delete words[i];

	delete words;
}

void Topic::findCSTRelations(char *type){

	Sentence *s = this -> g -> getIni();
	int bigger_i = 0, sum_i = 0, cur_topic = -1, prev_topic = -1;
	bool flag;
	float bigger_f = 0.0, sum_f = 0.0;

	Edge *e = NULL;
	Sentence *s_aux = NULL;

	while(s != NULL){

		flag = false;

		for(int i = 0; i < s -> getSizeVecArestaTotal(); i++){

			e = s -> getAresta(i);
			s_aux = e -> getS();

			if(s_aux -> getTopic() != s -> getTopic()){

				cur_topic = s_aux -> getTopic();

				if(!flag){

					prev_topic = cur_topic = s_aux -> getTopic();
					flag = true;
				}

				if(prev_topic == cur_topic){

					if(!strcmp(type, "value")){

						for(int j = 0; j < e -> getSizeVecCST(); j++)
							sum_f += e -> getCST(j);
					}

					else
						sum_i += e -> getSizeVecCST();
				}

				else {

//cout << "\nTexto: " << s -> getOrigemTexto() << " -- Pos: " << s -> getPosicaoOriginal() << endl;
//cout << "\nSENT: ";
//for(int ii = 0; ii < s -> getNumPalavras(); ii++)
//	cout << (s -> getSentencaOriginal())[ii] << " ";

//cout << "\nAUX: texto-> " << s_aux -> getOrigemTexto() << " -- pos-> " << s_aux -> getPosicaoOriginal() << endl;
//for(int ii = 0; ii < s_aux -> getNumPalavras(); ii++)
//	cout << (s_aux -> getSentencaOriginal())[ii] << " ";

					if(!strcmp(type, "value")){

						cout << "\nTopic_Change: " << s -> getTopic() << "_" << prev_topic << "_" << sum_f << endl;

						addCSTChange(s -> getTopic(), prev_topic, sum_f);
						sum_f = 0.0;

						for(int j = 0; j < e -> getSizeVecCST(); j++)
							sum_f += e -> getCST(j);
					}

					else {

						cout << "\nTopic_Change: " << s -> getTopic() << "_" << prev_topic << "_" << sum_i << endl;

						addCSTChange(s -> getTopic(), prev_topic, sum_i);
						sum_i = e -> getSizeVecCST();

					}

					prev_topic = cur_topic;
				}
			}
		}

		if(!s_aux -> getProx()){

			if(!strcmp(type, "value")){

				cout << "\nTopic_Change: " << s -> getTopic() << "_" << prev_topic << "_" << sum_f << endl;

				addCSTChange(s -> getTopic(), prev_topic, sum_f);
				sum_f = 0.0;

				for(int j = 0; j < e -> getSizeVecCST(); j++)
					sum_f += e -> getCST(j);
			}

			else {

				cout << "\nTopic_Change: " << s -> getTopic() << "_" << prev_topic << "_" << sum_i << endl;

				addCSTChange(s -> getTopic(), prev_topic, sum_i);
				sum_i = e -> getSizeVecCST();
			}
		}

		s = s -> getProx();
	}

	sumCSTResults();

	////////////////// ARRUMAR PAR o sumCSTResults também trabalhar com floats!!!!!!!!!!!!!!!
	if(!strcmp(type, "value"))
		clusteringCSTValue();

	else
		clusteringCSTUnit();
}

void Topic::sumCSTResults(){

	vector<char*> *vec_to_from;

	if(!(vec_to_from = new vector<char*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);
	}

	int to_1, from_1, value_1;
	int to_2, from_2, value_2;

	for(int i = 0; i < this -> vec_cst_aux -> size()-1; i++){

		getToFromValue(this -> vec_cst_aux -> at(i), &to_1, &from_1, &value_1);

		for(int j = i+1; j < this -> vec_cst_aux -> size(); j++){

			getToFromValue(this -> vec_cst_aux -> at(j), &to_2, &from_2, &value_2);

			if(to_1 == to_2 && from_1 == from_2)
				value_1 += value_2;
		}

		if(!hasVerified(vec_to_from, to_1, from_1)){

			addVec(vec_to_from, to_1, from_1, -1);
			addVec(this -> vec_cst, to_1, from_1, value_1);
		}
	}

	int size = vec_to_from -> size();

	for(int i = 0; i < size; i++)
		vec_to_from -> pop_back();

	delete vec_to_from;
}

void Topic::addVec(vector<char*> *vec, int to, int from, int value){

	char *aux;

	if(!(aux = new char [sizeof(char) * 10000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);	
	}

	if(value >= 0){

		if(sprintf(aux, "%d_%d_%d", to, from, value) < 0){

			cout << endl << "ERROR! Conversion Failed" << endl << endl;
			exit(1);
		}
	}

	else {

		if(sprintf(aux, "%d_%d", to, from) < 0){

			cout << endl << "ERROR! Conversion Failed" << endl << endl;
			exit(1);
		}
	}

	vec -> push_back(aux);
}

bool Topic::hasVerified(vector<char*> *vec, int to, int from){

	char aux[10000];

	if(sprintf(aux, "%d_%d", to, from) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(1);
	}

	for(int i = 0; i < vec -> size(); i++){

		if(!strcmp(vec -> at(i), aux))
			return true;
	}

	return false;
}

void Topic::getToFromValue(char *str, int *to, int *from, int *value){

	char aux[10000];
	int k;

	for(k = 0; str[k] != '_'; k++)
		aux[k] = str[k];

	aux[k] = '\0';
	*to = atoi(aux);

	int m, i_aux = 0;

	for(m = k+1; str[m] != '_'; m++)
		aux[i_aux++] = str[m];

	aux[i_aux] = '\0';
	*from = atoi(aux);

	i_aux = 0;

	for(int n = m+1; str[n] != '\0'; n++)
		aux[i_aux++] = str[n];

	aux[i_aux] = '\0';

	*value = atoi(aux);
}

void Topic::clusteringCSTUnit(){

	Sentence *s = this -> g -> getIni();
	char aux[10000];
	int from, to;
	int value, treshold = calculateTresholdCSTUnit((char*)"average");

cout << "\nTRESHOLD: " << treshold << endl << endl;

	for(int i = 0; i < this -> vec_cst -> size(); i++)
		cout << "\nvec_cst_before: " << this -> vec_cst -> at(i);

	for(int i = this -> vec_cst -> size()-1; i >= 0; i--){

		int j;

cout << "\nvec: " << this -> vec_cst -> at(i);

		for(j = 0; (this -> vec_cst -> at(i))[j] != '_'; j++)
			aux[j] = (this -> vec_cst -> at(i))[j];

		aux[j] = '\0';
		to = atoi(aux);

		int k, i_aux = 0;

		for(k = j+1; (this -> vec_cst -> at(i))[k] != '_'; k++)
			aux[i_aux++] = (this -> vec_cst -> at(i))[k];

		aux[i_aux] = '\0';
		from = atoi(aux);

		i_aux = 0;

		for(int m = k+1; (this -> vec_cst -> at(i))[m] != '\0'; m++)
			aux[i_aux++] = (this -> vec_cst -> at(i))[m];

		aux[i_aux] = '\0';

		value = atoi(aux);

////////// VERIFICAR SE O ALGARISMO 0 (ZERO) PODE ENTRAR NA JOGADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(value >= treshold && from != to){

		cout << "\nFROM: " << from << " -- TO: " << to << endl << endl;

			Sentence *s = this -> g -> getIni();

			while(s != NULL){

				if(s -> getTopic() == from){

					cout << "\nS->FROM: " << s -> getTopic() << " -- S->TO: " << to << endl;
					s -> setTopic(to);
				}

				s = s -> getProx();
			}

			/* Mudar em todo o vec_change_topics */
			for(int ii = this -> vec_cst -> size()-1; ii >= 0; ii--){

				for(j = 0; (this -> vec_cst -> at(ii))[j] != '_'; j++);

				char aux_to[10000];

				if(sprintf(aux_to, "%d", to) < 0){

					cout << endl << "ERROR! Conversion Failed" << endl << endl;
					exit(1);
				}

				k, i_aux = 0;

				for(k = j+1; (this -> vec_cst -> at(ii))[k] != '_'; k++)
					aux[i_aux++] = (this -> vec_cst -> at(ii))[k];

				aux[i_aux] = '\0';
				
				if(atoi(aux) == from){

					i_aux = 0;

					for(int m = k+1; (this -> vec_cst -> at(ii))[m] != '\0'; m++)
						aux[i_aux++] = (this -> vec_cst -> at(ii))[m];

					aux[i_aux] = '\0';

					for(j = 0; (this -> vec_cst -> at(ii))[j] != '_'; j++);

					(this -> vec_cst -> at(ii))[j+1] = '\0';
					strcat((this -> vec_cst -> at(ii)), aux_to);
					strcat((this -> vec_cst -> at(ii)), "_"); 
					strcat((this -> vec_cst -> at(ii)), aux);
					strcat((this -> vec_cst -> at(ii)), "\0");

cout << "\nNEW_VEC ------> " << (this -> vec_cst -> at(ii)) << endl << endl;
				}
			}
//////////////////////////////////////////////////////////////
				s = this -> g -> getIni();

				while(s != NULL){

					cout << "Topic: " << s -> getTopic() << endl;
					s = s -> getProx();
				}
/////////////////////////////////////////////////////////////
		}
	}

cout << endl;

	s = this -> g -> getIni();

	while(s != NULL){

		cout << "Topic: " << s -> getTopic()+1 << endl;
		s = s -> getProx();
	}
}

void Topic::clusteringCSTValue(){

	Sentence *s = this -> g -> getIni();
	char aux[10000];
	int from, to;
	float value, treshold = calculateTresholdCSTValue((char*)"average");

cout << "\nTRESHOLD: " << treshold << endl << endl;

	for(int i = 0; i < this -> vec_cst -> size(); i++)
		cout << "\nvec_cst_before: " << vec_cst -> at(i);

	for(int i = this -> vec_cst -> size()-1; i >= 0; i--){

		int j;

cout << "\nvec: " << vec_cst -> at(i);

		for(j = 0; (this -> vec_cst -> at(i))[j] != '_'; j++)
			aux[j] = (this -> vec_cst -> at(i))[j];

		aux[j] = '\0';
		to = atoi(aux);

		int k, i_aux = 0;

		for(k = j+1; (this -> vec_cst -> at(i))[k] != '_'; k++)
			aux[i_aux++] = (this -> vec_cst -> at(i))[k];

		aux[i_aux] = '\0';
		from = atoi(aux);

		i_aux = 0;

		for(int m = k+1; (this -> vec_cst -> at(i))[m] != '\0'; m++)
			aux[i_aux++] = (this -> vec_cst -> at(i))[m];

		aux[i_aux] = '\0';

		value = atof(aux);

////////// VERIFICAR SE O ALGARISMO 0 (ZERO) PODE ENTRAR NA JOGADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(value >= treshold && from != to){

		cout << "\nFROM: " << from << " -- TO: " << to << endl << endl;

			Sentence *s = this -> g -> getIni();

			while(s != NULL){

				if(s -> getTopic() == from){

					cout << "\nS->FROM: " << s -> getTopic() << " -- S->TO: " << to << endl;
					s -> setTopic(to);
				}

				s = s -> getProx();
			}

			/* Mudar em todo o vec_change_topics */
			for(int ii = this -> vec_cst -> size()-1; ii >= 0; ii--){

				for(j = 0; (this -> vec_cst -> at(ii))[j] != '_'; j++);

				char aux_to[10000];

				if(sprintf(aux_to, "%d", to) < 0){

					cout << endl << "ERROR! Conversion Failed" << endl << endl;
					exit(1);
				}

				k, i_aux = 0;

				for(k = j+1; (this -> vec_cst -> at(ii))[k] != '_'; k++)
					aux[i_aux++] = (this -> vec_cst -> at(ii))[k];

				aux[i_aux] = '\0';
				
				if(atoi(aux) == from){

					i_aux = 0;

					for(int m = k+1; (this -> vec_cst -> at(ii))[m] != '\0'; m++)
						aux[i_aux++] = (this -> vec_cst -> at(ii))[m];

					aux[i_aux] = '\0';

					for(j = 0; (this -> vec_cst -> at(ii))[j] != '_'; j++);

					(this -> vec_cst -> at(ii))[j+1] = '\0';
					strcat((this -> vec_cst -> at(ii)), aux_to);
					strcat((this -> vec_cst -> at(ii)), "_"); 
					strcat((this -> vec_cst -> at(ii)), aux);
					strcat((this -> vec_cst -> at(ii)), "\0");

cout << "\nNEW_VEC ------> " << (this -> vec_cst -> at(ii)) << endl << endl;
				}
			}
//////////////////////////////////////////////////////////////
				s = this -> g -> getIni();

				while(s != NULL){

					cout << "Topic: " << s -> getTopic() << endl;
					s = s -> getProx();
				}
/////////////////////////////////////////////////////////////
		}
	}

	s = this -> g -> getIni();

	while(s != NULL){

		cout << "Topic: " << s -> getTopic() << endl;
		s = s -> getProx();
	}
}

void Topic::addCSTChange(int to, int from, int value){

	char *cst_change = (char*) malloc(sizeof(char) * (MAX_NUM_LETRAS + 1000));

	if(sprintf(cst_change, "%d_%d_%d", to, from, value) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(1);
	}

	this -> vec_cst_aux -> push_back(cst_change);
}

void Topic::addCSTChange(int to, int from, float value){

	char *cst_change = (char*) malloc(sizeof(char) * (MAX_NUM_LETRAS + 1000));

	if(sprintf(cst_change, "%d_%d_%f", to, from, value) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(1);
	}

	this -> vec_cst_aux -> push_back(cst_change);
}

vector<char*>* Topic::getVecCST(){

	return this -> vec_cst;
}

int Topic::calculateTresholdCSTUnit(char *type){

	int sum = 0, value = 0;
	int bigger = 0, lesser = 1;

	for(int i = 0; i < this -> vec_cst -> size(); i++){

		int j, k, int_aux = 0;
		char aux[10000];

		for(j = 0; (this -> vec_cst -> at(i))[j] != '_'; j++);
		for(k = j+1; (this -> vec_cst -> at(i))[k] != '_'; k++);
		
		for(int m = k+1; (this -> vec_cst -> at(i))[m] != '\0'; m++)
			aux[int_aux++] = (this -> vec_cst -> at(i))[m];

		aux[int_aux] = '\0';
		value = atoi(aux);

//cout << "\nVALUE: " << value << endl;

		if(!strcmp(type, "average"))
			sum += value;

		if(!strcmp(type, "mm2")){

			if(value > bigger)
				bigger = value;

			if(value < lesser)
				lesser = value;
		}
	}

cout << "\nSUM: " << sum << endl;
cout << "\nSIZE: " << this -> vec_cst -> size() << endl;
cout << "\nSUM/VEC_CST-SIZE: " << sum / this -> vec_cst -> size() << endl;
//getchar();


	if(!strcmp(type, "average"))
		return sum / this -> vec_cst -> size();	

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2;

	return 0;
}

float Topic::calculateTresholdCSTValue(char *type){

	float sum = 0.0, value = 0.0;
	float bigger = 0.0, lesser = 1.0;

	for(int i = 0; i < this -> vec_cst -> size(); i++){

		int j, k, int_aux = 0;
		char aux[10000];

		for(j = 0; (this -> vec_cst -> at(i))[j] != '_'; j++);
		for(k = j+1; (this -> vec_cst -> at(i))[k] != '_'; k++);
		
		for(int m = k+1; (this -> vec_cst -> at(i))[m] != '\0'; m++)
			aux[int_aux++] = (this -> vec_cst -> at(i))[m];

		aux[int_aux] = '\0';
		value = atof(aux);

//cout << "\nVALUE: " << value << endl;

		if(!strcmp(type, "average"))
			sum += value;

		if(!strcmp(type, "mm2")){

			if(value > bigger)
				bigger = value;

			if(value < lesser)
				lesser = value;
		}
	}

	if(!strcmp(type, "average"))
		return sum / this -> vec_cst -> size();	

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2.0;

	return 0.0;
}



























