#include "../include/Topic.h"

Topic::Topic(Graph *g){

	this -> g = g;
	this -> vec_seg = new vector<Segment*>();
	this -> vec_keys = new vector<char*>();
	this -> vec_change_topics = new vector<char*>();
}

Topic::~Topic(){

	int size = this -> vec_seg -> size();

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
}

int Topic::fillVector(vector<char*> *vec_words, vector<int*> *vec_freq, Sentence *s){

	int cont = 0;
	bool flag;

	for(int i = 0; i < s -> getNumPalavrasStemmStop() && !flag; i++){

		flag = false;

		for(int j = 0; j < vec_words -> size() && !flag; j++){

			if(!strcmp((s -> getSentenca())[i], vec_words -> at(j))){

//cout << "THE SAME! Sentenca: " << (s -> getSentenca())[i] << " -- VEC: " << vec_words -> at(j) << endl;
				int *aux_freq = vec_freq -> at(j);
				
				(*aux_freq)++;

				flag = true;
			}
		}

		if(!flag){

			int *aux_freq = new int;

			*aux_freq = 1;

			vec_words -> push_back((s -> getSentenca())[i]);
			vec_freq -> push_back(aux_freq);

			cont++;
		}
	}
//cout << "\nbefore (freq->size(): " << vec_freq -> size() << ")";
//cout << "\n---------------------------\n";

//	for(int i = 0; i < vec_words -> size(); i++){

//		cout << "W: " << vec_words -> at(i) << " -- freq: " << *(vec_freq -> at(i)) << endl;
//	}
	
//cout << "after\n";

	return cont;
}

float Topic::cosineSimilarity(vector<char*> *vec_words, vector<int*>* vec_freq1, vector<int*> *vec_freq2){

	return 0.0;
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

	Sentence *s = this -> g -> getIni();
	Sentence *prev_s = s;

	int cur_topic = s -> getTopic(), prev_topic, first_change = 0, cont;
	char **aux;
//cout << "\n111\n";
	while(prev_s != NULL){

		prev_s = s;
			
		while(s != NULL && cur_topic == s -> getTopic()){

			fillVector(vec_words, vec_freq1, s);
			s = s -> getProx();
		}

		prev_topic = cur_topic;
//cout << "VEC_WORDS:\n";
for(int i = 0; i < vec_words -> size(); i++)
	cout << vec_words -> at(i) << endl;


		while(s != NULL){

			cont = 0;
			cur_topic = s -> getTopic();

//cout << "\n222_new\n";
			while(s != NULL && cur_topic == s -> getTopic()){

				cont += fillVector(vec_words, vec_freq2, s);
				s = s -> getProx();
			}

//cout << "\n333\n";
			float cosine = cosineSimilarity(vec_words, vec_freq1, vec_freq2);

//cout << "\n444\n";
			if(cosine > 0.7f)
				changeTopic(prev_topic, cur_topic);

//cout << "\n555\n";
			removeFromVector(cont, vec_words);
			
//cout << "\n666\n";
			s = s -> getProx();
		}
	}
}

void Topic::changeTopic(int prev_topic, int cur_topic){

	char tmp[100];

	sprintf(tmp, "%d_%d", prev_topic, cur_topic);

	this -> vec_change_topics -> push_back(tmp);
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

cout << endl << "Comparison between topic " << keywords_1[0] << " and " << keywords_2[0] << endl;
cout << "Numero de palavras do topico " << keywords_1[0] << ": " << num_keys_1 << endl;
cout << "Numero de palavras do topico " << keywords_2[0] << ": " << num_keys_2 << endl;
cout << "Numero de palavras iguais: " << sim << endl;

	int bigger;

	if(num_keys_1 >= num_keys_2)
		bigger = num_keys_1;

	else
		bigger = num_keys_2;

cout << "Similarity between them: " << (sim / ((float) bigger)) << endl << endl; 
 
	return sim / ((float) bigger);
}

/* Cluster topics depending on their keywords. */
void Topic::clusteringKeywords(){

	Segment *seg = new Segment();

	Sentence *s = this -> g -> getIni();

	seg -> setSignature(vec_keys -> at(0));
	seg -> setTopic(0);

cout << "\nSIG: " << seg -> getSignature() << endl;
getchar();

	while(s != NULL && s -> getTopic() == 0){

		seg -> addSentence(s);
		s = s -> getProx();
	}

	vec_seg -> push_back(seg);

	bool flag;
	float bigger;
	int index_i, index_j;

	for(int i = 1; i < vec_keys -> size(); i++){

		flag = false;
		bigger = 0.0;

		for(int j = 0; j < vec_seg -> size(); j++){

			if(i != j){

		cout << "\nCOMP! Vec_keys: " << vec_keys -> at(i) << " -- SIG: " << vec_seg -> at(j) -> getSignature() << endl;
		getchar();

				float sim = similarity(vec_keys -> at(i), vec_seg -> at(j) -> getSignature());

				if(sim >= 0.5f){

					if(sim > bigger){

						bigger = sim;
						index_i = i;
						index_j = j;

						flag = true;
					}
				}
			}
		}

		if(flag){
	
			int seg_aux = 0;

			seg = vec_seg -> at(seg_aux);

			while(seg_aux < vec_seg -> size() &&
					seg -> getTopic() != atoi(&(vec_keys -> at(index_i))[0])){

				seg_aux++;

				if(seg_aux < vec_seg -> size())
					seg = vec_seg -> at(seg_aux);
			}

			s = this -> g -> getIni();

			/* Enquanto não encontrar a primeira sentenca do topico correspondente */
			while(s != NULL && s -> getTopic() != atoi(&(vec_keys -> at(index_j))[0]))
				s = s -> getProx();

cout << "\nSeg_aux: " << seg_aux << endl;
cout << "Index_j: " << index_j << endl;
cout << "Sentence: ";

for(int u = 0; u < s -> getNumPalavras(); u++)
	cout << s -> getSentencaOriginal()[u] << " ";

cout << endl;

getchar();

			/* Quando achar */
			while(s != NULL && s -> getTopic() == atoi(&(vec_keys -> at(index_j))[0])){

				seg -> addSentence(s);
				s = s -> getProx();
			}
		}

		else {

			seg = NULL;
			seg = new Segment();
			
			seg -> setSignature(vec_keys -> at(i));
			seg -> setTopic(atoi(&(vec_keys -> at(i))[0]));
cout << "SEG_SIG: " << seg -> getSignature() << endl;
cout << "SEG_TOPIC: " << seg -> getTopic() << endl;
cout << "SEG_[0]: " << atoi(&(seg -> getSignature())[0]) << endl << endl;

			s = this -> g -> getIni();

			/* Enquanto não encontrar a primeira sentenca do topico correspondente */
			while(s != NULL && s -> getTopic() != seg -> getTopic())
				s = s -> getProx();

cout << "SEG_SIZE: " << seg -> getSize() << endl;

			/* Quando achar */
			while(s != NULL && s -> getTopic() == seg-> getTopic()){
//cout << "\n\nSENTENCE: ";
//for(int aaaa = 0; aaaa < s -> getNumPalavrasStemmStop(); aaaa++)
//cout << s -> getSentenca()[aaaa] << " ";

//cout << endl << endl;

				seg -> addSentence(s);
				s = s -> getProx();
			}

			vec_seg -> push_back(seg);
		}
	}

cout << "\nNumero de segmentos (new topics): " << vec_seg -> size() << endl;

///////////////////////////////////////////////////////////////////////////////////
	for(int i = 0; i < vec_seg -> size(); i++){

		Segment *ss = vec_seg -> at(i);

		cout << "\n\nNew Segment!!!\n"; 

		for(int j = 0; j < ss -> getSize(); j++){

			cout << "Sentenca (topic=" << ss -> getTopic() << "): "; 

			Sentence *sent = ss -> getSentence(j);

			for(int k = 0; k < sent -> getNumPalavras(); k++)
				cout << sent -> getSentencaOriginal()[k] << " ";

			cout << endl;
		
		}
	}
//////////////////////////////////////////////////////////////////////////////////
}

/* Cluster topics depending on the cosine similarity between groups */
void Topic::clusteringTopics(){

	Segment *seg = new Segment();

	Sentence *s = this -> g -> getIni();

	seg -> setTopic(0);

	while(s != NULL && s -> getTopic() == 0){

		seg -> addSentence(s);
		s = s -> getProx();
	}

	vec_seg -> push_back(seg);

	for(int i = 0; i < vec_keys -> size()-1; i++){

		for(int j = i+1; j < vec_keys -> size(); j++){

			if(similarity(vec_keys -> at(i), vec_keys -> at(j)) >= 0.5f){

				int seg_aux = 0;

				seg = vec_seg -> at(seg_aux);

				while(seg_aux < vec_seg -> size() &&
						seg -> getTopic() != atoi(&(vec_keys -> at(i))[0])){

					seg_aux++;

					if(seg_aux < vec_seg -> size())
						seg = vec_seg -> at(seg_aux);
				}

				s = this -> g -> getIni();

				/* Enquanto não encontrar a primeira sentenca do topico correspondente */
				while(s != NULL && s -> getTopic() != atoi(&(vec_keys -> at(j))[0]))
					s = s -> getProx();

				/* Quando achar */
				while(s != NULL && s -> getTopic() == atoi(&(vec_keys -> at(j))[0])){

					seg -> addSentence(s);
					s = s -> getProx();
				}
			}

			else {

				seg = new Segment();

				seg -> setTopic(atoi(&(vec_keys -> at(j))[0]));
				s = this -> g -> getIni();

				/* Enquanto não encontrar a primeira sentenca do topico correspondente */
				while(s != NULL && s -> getTopic() != atoi(&(vec_keys -> at(j))[0]))
					s = s -> getProx();

				/* Quando achar */
				while(s != NULL && s -> getTopic() == atoi(&(vec_keys -> at(j))[0])){

					seg -> addSentence(s);
					s = s -> getProx();
				}

				vec_seg -> push_back(seg);
			}
		}
	}

cout << "\nNumero de segmentos (new topics): " << vec_seg -> size() << endl;

///////////////////////////////////////////////////////////////////////////////////
	for(int i = 0; i < vec_seg -> size(); i++){

		Segment *ss = vec_seg -> at(i);

		cout << "\n\nNew Segment!!!\n"; 

		for(int j = 0; j < ss -> getSize(); j++){

			cout << "Sentenca (topic=" << ss -> getTopic() << "): "; 

			Sentence *sent = ss -> getSentence(j);

			for(int k = 0; k < sent -> getNumPalavras(); k++)
				cout << sent -> getSentencaOriginal()[k] << " ";

			cout << endl;
		
		}
	}
//////////////////////////////////////////////////////////////////////////////////
}


void Topic::removeFromVector(int cont, vector<char*> *vec_words){

	for(int i = 0; i < cont && vec_words -> size() >= 0; i++)
		vec_words -> pop_back();
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

	for(int i = 0; i < 100000; i++)
		delete words[i];

	delete words;


	clusteringKeywords();

}
