#include "../include/Topic.h"

Topic::Topic(Graph *g){

	this->g = g;
	this->vec_seg = new vector<Segment*>();
	this->vec_keys = new vector<char*>();
	this->vec_change_topics = new vector<char*>();
	this->vec_cst = new vector<char*>();
	this->vec_cst_aux = new vector<char*>();
	this->vec_topics = NULL;
	this->vec_groups_k = NULL;
	this->vec_groups_t = NULL;
	this->vec_groups_cst = NULL;
	this->vec_treshold_cst = NULL;
	global_topics = -1;

	if(!vec_seg || !vec_keys || !vec_change_topics || !vec_cst || !vec_cst_aux){

		cout << endl << "ERROR! Memory Overflow!" << endl << endl;
		exit(EXIT_FAILURE);
	}
}

Topic::~Topic(){

	int size;

	if(this->vec_topics != NULL)
		delete[] this->vec_topics;

	size = this->vec_seg->size();

	for(int i = 0; i < size; i++)
		this->vec_seg->pop_back();

	delete this->vec_seg;

	size = this->vec_keys->size();

	for(int i = 0; i < size; i++)
		this->vec_keys->pop_back();

	delete this->vec_keys;

	size = this->vec_change_topics->size();

	for(int i = 0; i < size; i++)
		this->vec_change_topics->pop_back();

	delete this->vec_change_topics;

	size = this->vec_cst->size();

	for(int i = 0; i < size; i++)
		this->vec_cst->pop_back();

	delete this->vec_cst;

	size = this->vec_cst_aux->size();

	for(int i = 0; i < size; i++)
		this->vec_cst_aux->pop_back();

	delete this->vec_cst_aux;
}

int Topic::fillVector(vector<char*> *vec_words, Sentence *s){

	for(int i = 0; i < s->getNumPalavrasStemmStop(); i++)
			vec_words->push_back((s->getSentenca())[i]);

	return s->getNumPalavrasStemmStop();
}

void Topic::addSegment(Segment *segment){

	this->vec_seg->push_back(segment);
}

vector<char*>* Topic::getVecKeys(){

		return this->vec_keys;
}

void Topic::addKeyword(int topic, char **words, int frequency[100000]){

	char *keyword = (char*) malloc(sizeof(char) * (MAX_NUM_LETRAS + 1000));
	int bigger = -1, freq_cont = 0;

	for(int i = 0; frequency[i] != -1; i++)
		freq_cont++;

	if(sprintf(keyword, "%d", topic) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(EXIT_FAILURE);
	}
//cout << "\n\n------------------------------------------------------------\n" << "Topic: " << topic << endl;
	for(int i = 0; i < freq_cont; i++){

//cout << "Word: " << words[i] << "  - Frequency: " << frequency[i] << endl;

		if(frequency[i] > bigger)
			bigger = frequency[i];
	}
//cout << "\n\n------------------------------------------------------------\n";
//getchar();

	int num = 0;

	while(num < 10){

		for(int i = 0; i < freq_cont; i++){

			if(frequency[i] == bigger){
		
				strcat(keyword, "_");
				strcat(keyword, words[i]);
			}
		}

		strcat(keyword, "\0");

		bool flag = false;

		for(int i = 0; keyword[i] != '\0'; i++){

				if(keyword[i] == '_'){

			/* Skip the topic identification number */
					if(flag)
						num++;

					else
						flag = true;
				}
			}

		strcat(keyword, "\0");
		bigger--;

//cout << "\nKeyword: " << keyword << endl;
//getchar();
	}

	vec_keys->push_back(keyword);
//cout << "\n----------\n" << "Topic: " << topic << endl;
//	for(int i = 0; i < vec_keys->size(); i++)
//		cout << "Keywords(internal): " << vec_keys->at(i) << endl;

}

void Topic::findSimilarTopics(){

	vector<char*> *vec_words;
	vector<int*> *vec_freq1, *vec_freq2;

	if(!(vec_words = new vector<char*>()) || 
		!(vec_freq1 = new vector<int*>()) || 
			!(vec_freq2 = new vector<int*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	char **A, **B;
	int size_A, size_B;

	Sentence *s = this->g->getIni();
	Sentence *prev_s = s;

	int cur_topic = s->getTopic(), prev_topic, first_change = 0, cont;
	char **aux;

	while(prev_s != NULL){
			
		while(s != NULL && cur_topic == s->getTopic()){

			fillVector(vec_words, s);

//cout << "\nSentenca: ";
//for(int indexx = 0; indexx < s->getNumPalavrasStemmStop(); indexx++)
	//cout << (s->getSentenca())[indexx] << endl;
//cout << endl << endl;
			s = s->getProx();
		}

		prev_topic = cur_topic;
//cout << "\nPrev-Topic: " << prev_topic << endl << endl;
 
//cout << "VEC_WORDS:\n";
//for(int i = 0; i < vec_words->size(); i++)
	//cout << vec_words->at(i) << endl;

		while(s != NULL){

			cont = 0;
			cur_topic = s->getTopic();

//cout << "\nCur-Topic: " << cur_topic << endl << endl;

			while(s != NULL && cur_topic == s->getTopic()){

				cont += fillVector(vec_words, s);
				s = s->getProx();
			}

//cout << "\nSize_VEC_WORDS: " << vec_words->size() << endl;
//cout << "VEC_WORDS:\n";
//for(int i = 0; i < vec_words->size(); i++)
	//cout << vec_words->at(i) << endl;

			size_A = size_B = 0;

	//cout << "\n\nCONT: " << cont << endl;
	//cout << "VEC_FREQ1_SIZE: " << vec_freq1->size() << endl;
	//cout << "VEC_WORDS_SIZE: " << vec_words->size() << endl << endl;

			size_A = vec_words->size() - cont;
			size_B = cont;

			A = new char* [sizeof(char) * size_A];
			B = new char* [sizeof(char) * size_B];

			if(!A || !B){

				cout << endl << "ERROR! Memory Overflow" << endl << endl;
				exit(EXIT_FAILURE);
			}

			for(int aaa = 0; aaa < size_A; aaa++){

				if(!(A[aaa] = new char[sizeof(char) * MAX_NUM_LETRAS])){

					cout << endl << "ERROR! Memory Overflow" << endl << endl;
					exit(EXIT_FAILURE);
				}
			}

			for(int bbb = 0; bbb < size_B; bbb++){

				if(!(B[bbb] = new char[sizeof(char) * MAX_NUM_LETRAS])){

					cout << endl << "ERROR! Memory Overflow" << endl << endl;
					exit(EXIT_FAILURE);
				}
			}

//cout << "\nSIZE_A: " << size_A << endl;
//cout << "SIZE_B: " << size_B << endl;

			for(int aaa = 0; aaa < size_A; aaa++){

				strcpy(A[aaa], vec_words->at(aaa));
	//			cout << "A[" << aaa << "]: " << A[aaa] << endl;
			}

			for(int bbb = 0; bbb < size_B; bbb++){
		
				strcpy(B[bbb], vec_words->at(bbb + size_A));
	//			cout << "B[" << bbb << "]: " << B[bbb] << endl;

			}

			float cosine = Util().cosineSimilarity(A, B, size_A, size_B);

//cout << "\nCosine: " << cosine << endl << endl;

			//if(cosine >= 0.7f){
//cout << "\nIS BIGGER!\n";
				changeTopic(prev_topic, cur_topic, cosine);
	//		}

			removeFromVector(cont, vec_words);
		}

		while(prev_s != NULL && prev_s->getTopic() == prev_topic)
			prev_s = prev_s->getProx();

		if(prev_s != NULL){

			cur_topic = prev_s->getTopic();
			s = prev_s;
		
			while(s != NULL && s->getTopic() == cur_topic)
				s = s->getProx();
		}

		removeFromVector(vec_words->size(), vec_words);
		
//cout << "\nS: " << s;
//cout << "\nPREV: " << prev_s << endl << endl;

		s = prev_s;
	}

	clusteringTopics();
}

void Topic::changeTopic(int prev_topic, int cur_topic, float cosine){

	char *tmp;

	if(!(tmp = new char [sizeof(char) * 10000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	if(sprintf(tmp, "%d_%d_%f", prev_topic, cur_topic, cosine) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(EXIT_FAILURE);
	}

	//cout << "\n-----------------------------------------------TOPIC CHANGED => " << tmp << endl;

	this->vec_change_topics->push_back(tmp);

//cout << "\n-----------------------------------------------TOPIC CHANGED => " << this->vec_change_topics->at(this->vec_change_topics->size()-1) << endl;
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

//cout << endl << "Comparação entre os tópicos " << keywords_1[0] << " e " << keywords_2[0] << endl;
//cout << "Numero de palavras do tópico " << keywords_1[0] << ": " << num_keys_1 << endl;
//cout << "Numero de palavras do tópico " << keywords_2[0] << ": " << num_keys_2 << endl;
//cout << "Numero de palavras iguais: " << sim << endl;

	int bigger;

	if(num_keys_1 >= num_keys_2)
		bigger = num_keys_1;

	else
		bigger = num_keys_2;

//cout << "Similaridade entre eles: " << (sim / ((float) bigger)) << endl << endl; 
 
	return sim / ((float) bigger);
}

float Topic::calculateTresholdKeywords(char *type){

	int count = 0;
	float sum = 0.0, sim;
	float bigger = 0.0, lesser = 1.0;

	for(int i = vec_keys->size()-1; i >= 1; i--){

		for(int j = i-1; j >= 0; j--){

			sim = similarity(vec_keys->at(i), vec_keys->at(j));

//	cout << "\nSIM: " << sim << endl;

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

int Topic::separateKeywords(char *keywords, char aux_topics[100000]){

	int i, j, k = 0;

	if(aux_topics != NULL){

		for(i = 0; keywords[i] != '_'; i++);

		for(j = i+1; keywords[j] != '\0'; j++)
			aux_topics[k++] = keywords[j];

		aux_topics[k] = '\0';
	}

	char buffer[50];
	int m = 0;

	for(int l = 0; keywords[l] != '_'; l++)
		buffer[m++] = keywords[l];

	buffer[m] = '\0';

	return atoi(buffer);
}

/* Cluster topics depending on their keywords. */
void Topic::clusteringKeywords(){

	Sentence *s;
	int id = -1;
	this->vec_groups_k = new vector<Group_K>();

	if(!vec_groups_k){

		cout << endl << "ERROR! Memory Overflow!" << endl << endl;
		exit(EXIT_FAILURE);
	}
///////////////////////////////////////////////////////////////////////// HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!/////////////////////////////////////////////
	int aux_id = -1, aux_keys = -1, topic_a, topic_b, doc_a, doc_b, i_a;
	float bigger, treshold = calculateTresholdKeywords((char*)"average");
	bool flag_possible, flag_a, flag_b, flag_stop;
	char buffer_a[50], buffer_b[50];

//cout << "\nTRESHOLD: " << treshold << endl << endl;
	
	for(int i = 0; i < vec_keys->size(); i++){

		bigger = -1.0;
		aux_keys = i;

		for(int j = 0; j < vec_groups_k->size(); j++){

			s = this->g->getIni();
			flag_possible = false;
			flag_a = false;
			flag_b = false;
			flag_stop = false;
			topic_a = -1;
			topic_b = -1;
			doc_a = -1;
			doc_b = -1;

			while(s != NULL && !flag_stop){

				int n = sprintf(buffer_b, "%d", s->getTopic());
				buffer_b[n] = '\0';

				for(i_a = 0; (vec_keys->at(i))[i_a] != '_'; i_a++)
					buffer_a[i_a] = (vec_keys->at(i))[i_a];

				buffer_a[i_a] = '\0';

				if(!(strcmp(buffer_a, buffer_b)) && !flag_a){

					topic_a = s->getTopic();
					doc_a = s->getOrigemTexto();
					flag_a = true;
				}

				for(i_a = 0; (vec_keys->at(i))[i_a] != '_'; i_a++)
					buffer_a[i_a] = (vec_keys->at(j))[i_a];

				buffer_a[i_a] = '\0';

				if(!(strcmp(buffer_a, buffer_b)) && !flag_b){

					topic_b = s->getTopic();
					doc_b = s->getOrigemTexto();
					flag_b = true;
				}

				if(flag_a && flag_b)
					flag_stop;

				s = s->getProx();
			}

//cout << "!!!!!!!!!!!!!!!!!!!!!\n";
//cout << "I: " << i << endl;
//cout << "J: " << j << endl;
//cout << "Topic_A: " << topic_a << endl;
//cout << "Topic_B: " << topic_b << endl;
//cout << "Doc_A: " << doc_a << endl;
//cout << "Doc_B: " << doc_b << endl;
//cout << "!!!!!!!!!!!!!!!!!!!!!\n";

			if((doc_a == doc_b && abs(topic_a - topic_b) != 1) || doc_a != doc_b)
				flag_possible = true;

//cout << "Abs: " << abs(topic_a - topic_b) << endl;
//cout << "Flag_Possible: " << flag_possible << endl;
//getchar();

			if(flag_possible){
//cout << "Got in!\n";
//getchar();
				float sim = similarity(vec_keys->at(i), vec_groups_k->at(j).keywords);
	//cout << "\nSimilarity between \"" << vec_keys->at(i) << "\" and \"" << vec_groups_k->at(j).keywords << "\" is \"" << sim << "\""<< endl;
	//getchar();
				if(sim >= treshold && sim > bigger){

					aux_id = j;
					bigger = sim;
	//cout << "\nCHANGE! -- AUX_ID: " << aux_id << endl;
	//getchar();
				}
			}
		}

// If 'bigger' is set, no need to create a new group (SinglePass); otherwise, create a new one.

		// Add into an existing group
		if(bigger != -1.0){

			char aux_topics[100000];
			int k;

//			strcat(this->vec_groups->at(aux_id).keywords, "_");
	//		strcat(this->vec_groups->at(aux_id).keywords, this->vec_keys->at(aux_keys));
		//	strcat(this->vec_groups->at(aux_id).keywords, "\0");

//			cout << "VEC_GROUPS: " << this->vec_groups->at(aux_keys).keywords << endl;
	//		getchar();

			for(k = 0; this->vec_groups_k->at(aux_id).topics[k] != -1; k++);

			// Guarantee
			strcat(this->vec_keys->at(aux_keys), "\0");

			this->vec_groups_k->at(aux_id).topics[k] = separateKeywords(this->vec_keys->at(aux_keys), aux_topics);
			this->vec_groups_k->at(aux_id).topics[k+1] = -1;
			strcat(this->vec_groups_k->at(aux_id).keywords, "_");			
			strcat(this->vec_groups_k->at(aux_id).keywords, aux_topics);
			strcat(this->vec_groups_k->at(aux_id).keywords, "\0");

			//cout << "VEC_GROUPS: " << this->vec_groups_k->at(aux_id).keywords << endl;
			//getchar();
		}

		// Create new group
		else {

			Group_K aux_group;

			aux_group.id = ++id;			
			aux_group.topics[0] = separateKeywords(this->vec_keys->at(aux_keys), NULL);
			aux_group.topics[1] = -1;
			strcpy(aux_group.keywords, this->vec_keys->at(aux_keys));
			strcat(aux_group.keywords, "\0");

			this->vec_groups_k->push_back(aux_group);
		}
/*
	for(int i = vec_keys->size()-1; i >= 1; i--){

		from = -1;
		to = -1;
		bigger = -1.0;

		for(int j = i-1; j >= 0; j--){

			float sim = similarity(vec_keys->at(i), vec_keys->at(j));
cout << "\nSimilarity between \"" << vec_keys->at(i) << "\" and \"" << vec_keys->at(j) << "\" is \"" << sim << "\""<< endl;
//getchar();	
			if(sim >= treshold && sim > bigger){

				from = i;
				to = j;
				bigger = sim;
cout << "\nCHANGE! -- From: " << i+1 << "->TO: " << to+1 << endl;
//getchar();
			}
		}

// If 'bigger' is set, no need to create a new group (SinglePass); otherwise, create a new one.

		// Add into an existing group
		if(bigger != -1.0){

			
		}

		// Create new group
		else {
		
			Group aux_group;

			aux_group.id = ++id;
//			aux_group.topics = vec_keys->(;

			this->vec_groups->push(aux_group);
		}
/*
		s = this->g->getIni();

		bool found = false;
		Sentence *s_aux = NULL;

		while(s != NULL && !found){

			if(s->getTopic() == to){

				s_aux = s;
				found = true;
			}

			else
				s = s->getProx();
		}

cout << "\nBEFORE\n";
s = this->g->getIni();
while(s != NULL){

cout << "\nTopic: " << s->getTopic()+1;
s = s->getProx();
}
cout << "\nSSSS: " << s;

		s = this->g->getIni();
		bool stop = false;

		while(s != NULL && !stop){
cout << "\nTopic: " << s->getTopic() << " -- Origem: " << s->getOrigemTexto() << endl;

			if(s->getTopic() == from){ 

				if(s->getOrigemTexto() != s_aux->getOrigemTexto() ||
					(s->getOrigemTexto() == s_aux->getOrigemTexto() && abs(s->getTopic() - s_aux->getTopic()) != 1)){

cout << "\nOrigem s: " << s->getOrigemTexto() << " -- Origem s_aux: " << s_aux->getOrigemTexto() << endl;
cout << "Topic s: " << s->getTopic() << " -- Topic s_aux: " << s_aux->getTopic() << " -- Abs: " << abs(s->getTopic() - s_aux->getTopic()) << endl << endl;


	cout << "\nTopic change from \"" << from << "\" to \"" << to << "\""<< endl;

					s->setTopic(to);
				}

				else
					stop = true;
			}

			s = s->getProx();
		}
cout << "\nAFTER\n";
s = this->g->getIni();
while(s != NULL){

cout << "\nTopic: " << s->getTopic()+1;
s = s->getProx();
}


*/

	}

	bool flag_found;

//cout << "\n\nBefore!\n";
//s = this->g->getIni();

//while(s != NULL){

//cout << "Topic: " << s->getTopic() << " " << s->getOrigemTexto() << " " << s->getPosicaoOriginal() << endl;
//s = s->getProx();
//}
	s = this->g->getIni();

	while(s != NULL){

		flag_found = false;

		for(int i = 0; i < this->vec_groups_k->size() && !flag_found; i++){

			for(int j = 0; this->vec_groups_k->at(i).topics[j] != -1 && !flag_found; j++){

//cout << "Vec_K: " << this->vec_groups_k->at(i).topics[j] << endl;
//cout << "GetTopic: " << s->getTopic() << endl;
//cout << "Vec_K(ID): " << this->vec_groups_k->at(i).id << endl;
//getchar();
				if(this->vec_groups_k->at(i).topics[j] == s->getTopic()){
//cout << "Got In!" << endl;
//getchar();
					flag_found = true;
					s->setTopic(this->vec_groups_k->at(i).id);
				}
			}
		}

		s = s->getProx();
	}

//cout << "\n\nAfter!\n";
//s = this->g->getIni();

//while(s != NULL){

//cout << "Topic: " << s->getTopic() << " ";
//s = s->getProx();
//}


	//cout << "\nbegin!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;

	//for(int i = 0; i < this->vec_groups_k->size(); i++){
		//cout << "ID: " << this->vec_groups_k->at(i).id << endl;
		//cout << "Topics: ";

		//for(int j = 0; this->vec_groups_k->at(i).topics[j] != -1; j++)
		//	cout << this->vec_groups_k->at(i).topics[j] << " ";

//		cout << "\nKeywords: " << this->vec_groups_k->at(i).keywords << endl;
	//}

	//cout << "end!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
//getchar();

	int size = this->vec_groups_k->size();
	setGlobalTopics(size);

	for(int i = 0; i < size; i++)
		this->vec_groups_k->pop_back();

	delete this->vec_groups_k;
}

float Topic::calculateTresholdTopics(char *type){

	float sum = 0.0, value = 0.0;
	float bigger = 0.0, lesser = 1.0;

	for(int i = 0; i < this->vec_change_topics->size(); i++){

		int j, k, int_aux = 0;
		char aux[10000];

		for(j = 0; (this->vec_change_topics->at(i))[j] != '_'; j++);
		for(k = j+1; (this->vec_change_topics->at(i))[k] != '_'; k++);
		
		for(int m = k+1; (this->vec_change_topics->at(i))[m] != '\0'; m++)
			aux[int_aux++] = (this->vec_change_topics->at(i))[m];

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
		return sum / ((float) this->vec_change_topics->size());

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2.0;

	return 0.0;
}

void Topic::concatenateVecs(int index_topics, int size_groups, char **A, int size_A){

	char **B;
	int size_B = size_groups + size_A;

	B = new char* [sizeof(char) * size_B];

	if(!B){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	for(int bbb = 0; bbb < size_B; bbb++){

		if(!(B[bbb] = new char[sizeof(char) * MAX_NUM_LETRAS])){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(EXIT_FAILURE);
		}
	}

	//cout << "\nSIZE_B: " << size_B << endl;

	for(int bbb = 0; bbb < size_groups; bbb++){

		strcpy(B[bbb], this->vec_groups_t->at(index_topics).topics[bbb]);
		//cout << "B[" << bbb << "]: " << B[bbb] << endl;
	}

	delete this->vec_groups_t->at(index_topics).topics;

	this->vec_groups_t->at(index_topics).topics = B;
	
	int j = size_groups;

	for(int i = 0; i < size_A; i++)
		this->vec_groups_t->at(index_topics).topics[j++] = A[i];

	this->vec_groups_t->at(index_topics).size = size_groups;
	B = NULL;
}

void Topic::clusteringTopics(){

	float treshold = calculateTresholdTopics((char*)"average");

	//cout << "\nTRESHOLD: " << treshold << endl << endl;

	vector<char*> *vec_words = new vector<char*>();
	this->vec_groups_t = new vector<Group_T>();

	if(!vec_words || !this->vec_groups_t){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	char **A;
	int size_A;

	Sentence *s = this->g->getIni();

	int cur_topic = s->getTopic(), id = 0, cur_doc = -1;
	bool flag_first = false;
	char **aux;

	while(s != NULL){
			
		while(s != NULL && cur_topic == s->getTopic()){

			fillVector(vec_words, s);
			cur_doc = s->getOrigemTexto();

//cout << "\nSentenca: ";
//for(int indexx = 0; indexx < s->getNumPalavrasStemmStop(); indexx++)
	//cout << (s->getSentenca())[indexx] << " ";
//cout << endl << endl;

			s = s->getProx();
		}

	//	cout << "\nSize_VEC_WORDS: " << vec_words->size() << endl;
	//	cout << "VEC_WORDS:\n";
	
		//for(int i = 0; i < vec_words->size(); i++)
		//	cout << vec_words->at(i) << endl;

		size_A = vec_words->size();
		A = new char* [sizeof(char) * vec_words->size()];

		if(!A){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(EXIT_FAILURE);
		}

		for(int aaa = 0; aaa < size_A; aaa++){

			if(!(A[aaa] = new char[sizeof(char) * MAX_NUM_LETRAS])){

				cout << endl << "ERROR! Memory Overflow" << endl << endl;
				exit(EXIT_FAILURE);
			}
		}

		//cout << "\nSIZE_A: " << size_A << endl;

		for(int aaa = 0; aaa < size_A; aaa++){

			strcpy(A[aaa], vec_words->at(aaa));
			//cout << "A[" << aaa << "]: " << A[aaa] << endl;
		}

		if(!flag_first){

			Group_T aux_group;

			aux_group.id = 0;			
			aux_group.index_topics[0] = cur_topic;
			aux_group.index_topics[1] = -1;
			aux_group.index_docs[0] = cur_doc;
			aux_group.index_docs[1] = -1;
			aux_group.topics = A;
			aux_group.size = size_A;

			this->vec_groups_t->push_back(aux_group);
			flag_first = true;
		}

		else {

			float bigger = -1.0, cosine = -1.0;
			int index_topics = -1;
			bool allowed_0, allowed_1;

			for(int i_groups = 0; i_groups < this->vec_groups_t->size(); i_groups++){

				allowed_0 = true;

				for(int i_docs = 0; this->vec_groups_t->at(i_groups).index_docs[i_docs] != -1; i_docs++){

//cout << "\nVec_groups[i_docs]: " << this->vec_groups_t->at(i_groups).index_docs[i_docs];
//cout << "\nCur_doc: " << cur_doc << endl << endl;

					if(this->vec_groups_t->at(i_groups).index_docs[i_docs] == cur_doc)
						allowed_0 = false;
				}

				allowed_1 = true;

				for(int i_topics = 0; this->vec_groups_t->at(i_groups).index_topics[i_topics] != -1; i_topics++){

//cout << "\nVec_groups[i_topics]: " << this->vec_groups_t->at(i_groups).index_topics[i_topics];
//cout << "\nCur_topic-1: " << (cur_topic-1) << endl << endl;

					if(this->vec_groups_t->at(i_groups).index_topics[i_topics] == cur_topic-1 ||
						this->vec_groups_t->at(i_groups).index_topics[i_topics] == cur_topic+1)
						allowed_1 = false;
				}

				if((!allowed_0 && allowed_1) || (allowed_0 && !allowed_1) || (allowed_0 && allowed_1)){

					cosine = Util().cosineSimilarity(this->vec_groups_t->at(i_groups).topics, A, this->vec_groups_t->at(i_groups).size, size_A); 

					//cout << "\nCosine: " << cosine << endl;
					//getchar();
					if(cosine >= treshold && cosine > bigger){

						index_topics = i_groups;
						bigger = cosine;
					}
				}

				else {

					//cout << "\nNot Allowed!\n\n";
					//getchar();
				}
			}

			if(bigger != -1.0){			
			
				concatenateVecs(index_topics, this->vec_groups_t->at(index_topics).size, A, size_A);

				int aux_index;				
				for(aux_index = 0; this->vec_groups_t->at(index_topics).index_topics[aux_index] != -1; aux_index++);

				this->vec_groups_t->at(index_topics).index_topics[aux_index] = cur_topic;
				this->vec_groups_t->at(index_topics).index_topics[aux_index+1] = -1;

				for(aux_index = 0; this->vec_groups_t->at(index_topics).index_docs[aux_index] != -1; aux_index++);

				this->vec_groups_t->at(index_topics).index_docs[aux_index] = cur_doc;
				this->vec_groups_t->at(index_topics).index_docs[aux_index+1] = -1;

				this->vec_groups_t->at(index_topics).size += size_A;
			}

			else {

				Group_T aux_group;

				aux_group.id = ++id;			
				aux_group.index_topics[0] = cur_topic;
				aux_group.index_topics[1] = -1;
				aux_group.index_docs[0] = cur_doc;
				aux_group.index_docs[1] = -1;
				aux_group.topics = A;
				aux_group.size = size_A;

				this->vec_groups_t->push_back(aux_group);
			}
		}

		int size_vec_words = vec_words->size();

		for(int i_vec_words = 0; i_vec_words < size_vec_words; i_vec_words++)
			vec_words->pop_back();

		size_A = 0;
		A = NULL;

		if(s != NULL)
			cur_topic = s->getTopic();
	}

	bool flag_found;

//cout << "\n\nBefore!\n";
//s = this->g->getIni();

//while(s != NULL){

//cout << "Topic: " << s->getTopic() << " " << s->getOrigemTexto() << " " << s->getPosicaoOriginal() << endl;
//s = s->getProx();
//}
	s = this->g->getIni();

	while(s != NULL){

		flag_found = false;

		for(int i = 0; i < this->vec_groups_t->size() && !flag_found; i++){

			for(int j = 0; this->vec_groups_t->at(i).index_topics[j] != -1 && !flag_found; j++){

				if(this->vec_groups_t->at(i).index_topics[j] == s->getTopic()){
				
					flag_found = true;
					s->setTopic(this->vec_groups_t->at(i).id);
				}
			}
		}

		s = s->getProx();
	}

//cout << "\n\nAfter!\n";
/*
s = this->g->getIni();

int i_file, i_aux_file;
char aux_file[50];
FILE *arq_after;
arq_after = fopen("agrupamento.txt", "w");
 
while(s != NULL){

//cout << "Topic: " << s->getTopic() << " ";

	i_file = 0;
	while((s->getOriginalFile())[i_file++] != 'D');

	i_aux_file = 0;
	while((s->getOriginalFile())[i_file] != '_'){

		//cout << (s->getOriginalFile())[i_file];
		aux_file[i_aux_file++] = (s->getOriginalFile())[i_file];
		i_file++;
	}

	aux_file[i_aux_file] = '\0';

//cout << " " << s->getPosicaoOriginal() << endl;


fprintf(arq_after, "%d %s %d\n", s->getTopic()+1, aux_file, s->getPosicaoOriginal()+1);
s = s->getProx();
}

fclose(arq_after);
*/
//cout << endl << endl << "begin!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;

//	for(int i = 0; i < this->vec_groups_t->size(); i++){

//		cout << "ID: " << this->vec_groups_t->at(i).id << endl;

//		cout << "Index Topics: ";
//		for(int j = 0; this->vec_groups_t->at(i).index_topics[j] != -1; j++)
//			cout << this->vec_groups_t->at(i).index_topics[j] << ", ";

//		cout << "Topics: ";
//		for(int k = 0; k < this->vec_groups_t->at(i).size; k++)
//			cout << this->vec_groups_t->at(i).topics[k] << " ";
	

//		cout << endl << endl;
//	}

//	cout << "end!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
//getchar();

	int size = this->vec_groups_t->size();
	setGlobalTopics(size);

	for(int i = 0; i < size; i++)
		this->vec_groups_t->pop_back();

	size = vec_words->size();

	for(int i_vec_words = 0; i_vec_words < size; i_vec_words++)
		vec_words->pop_back();

	delete this->vec_groups_t;
	delete vec_words;
}

void Topic::setGlobalTopics(int global_topics){

	this->global_topics = global_topics;
}

int Topic::getGlobalTopics(){

	return this->global_topics;
}

/* Cluster topics depending on the cosine similarity between groups */
void Topic::clusteringTopics_OLD(){

	Sentence *s = this->g->getIni();
	char aux[10000];
	int from, to;
	float value, treshold = calculateTresholdTopics((char*)"average");

cout << "\nTRESHOLD: " << treshold << endl << endl;

	for(int i = 0; i < this->vec_change_topics->size(); i++)
		cout << "\nvec_topics_before: " << vec_change_topics->at(i);

	for(int i = this->vec_change_topics->size()-1; i >= 0; i--){

		int j;

cout << "\nvec: " << vec_change_topics->at(i);

		for(j = 0; (this->vec_change_topics->at(i))[j] != '_'; j++)
			aux[j] = (this->vec_change_topics->at(i))[j];

		aux[j] = '\0';
		to = atoi(aux);

		int k, i_aux = 0;

		for(k = j+1; (this->vec_change_topics->at(i))[k] != '_'; k++)
			aux[i_aux++] = (this->vec_change_topics->at(i))[k];

		aux[i_aux] = '\0';
		from = atoi(aux);

		i_aux = 0;

		for(int m = k+1; (this->vec_change_topics->at(i))[m] != '\0'; m++)
			aux[i_aux++] = (this->vec_change_topics->at(i))[m];

		aux[i_aux] = '\0';
		value = atof(aux);

		if(value >= treshold && from != to){

		cout << "\n(((((((TENTANTTIIIVAAA)))))) FROM: " << from << " -- TO: " << to << endl << endl;
		//getchar();
			bool found = false;
			Sentence *s = this->g->getIni(), *s_aux = NULL;

			while(s != NULL && !found){

				if(s->getTopic() == to){

					s_aux = s;
					found = true;
				}

				else
					s = s->getProx();
			}

			s = this->g->getIni();
			bool stop = false;

			while(s != NULL && !stop){

				if(s->getTopic() == from){

					if(s->getOrigemTexto() != s_aux->getOrigemTexto() ||
							(s->getOrigemTexto() == s_aux->getOrigemTexto() && abs(s->getTopic() - s_aux->getTopic()) != 1)){
					
cout << "\n(((CONSEGUIU))) S->FROM: " << s->getTopic() << " -- S->TO: " << to << endl;
//getchar();
	
						s->setTopic(to);
					}

					else {cout << "\nNÃO CONSEGUIUUUU!!!\n";//getchar();
						stop = true;}
				}

				s = s->getProx();
			}

			/* Mudar em todo o vec_change_topics */
			for(int ii = this->vec_change_topics->size()-1; ii >= 0; ii--){

				for(j = 0; (this->vec_change_topics->at(ii))[j] != '_'; j++);

				char aux_to[10000];

				if(sprintf(aux_to, "%d", to) < 0){

					cout << endl << "ERROR! Conversion Failed" << endl << endl;
					exit(EXIT_FAILURE);
				}

				k, i_aux = 0;

				for(k = j+1; (this->vec_change_topics->at(ii))[k] != '_'; k++)
					aux[i_aux++] = (this->vec_change_topics->at(ii))[k];

				aux[i_aux] = '\0';
				
				if(atoi(aux) == from){

					i_aux = 0;

					for(int m = k+1; (this->vec_change_topics->at(ii))[m] != '\0'; m++)
						aux[i_aux++] = (this->vec_change_topics->at(ii))[m];

					aux[i_aux] = '\0';

					for(j = 0; (this->vec_change_topics->at(ii))[j] != '_'; j++);

					(this->vec_change_topics->at(ii))[j+1] = '\0';
					strcat((this->vec_change_topics->at(ii)), aux_to);
					strcat((this->vec_change_topics->at(ii)), "_"); 
					strcat((this->vec_change_topics->at(ii)), aux);
					strcat((this->vec_change_topics->at(ii)), "\0");

cout << "\nNEW_VEC ------> " << (this->vec_change_topics->at(ii)) << endl << endl;
				}
			}
//////////////////////////////////////////////////////////////
				s = this->g->getIni();

				while(s != NULL){

					cout << "Topic: " << s->getTopic() << endl;
					s = s->getProx();
				}
/////////////////////////////////////////////////////////////
		}
	}
cout << endl << endl;
	s = this->g->getIni();

	while(s != NULL){

		cout << "Topic: " << s->getTopic()+1 << endl;
		s = s->getProx();
	}
}


void Topic::removeFromVector(int cont, vector<char*> *vec){

	for(int i = 0; i < cont && vec->size() >= 0; i++)
		vec->pop_back();
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

	int bigger = 0, lesser = 10000, sum = 0, num = 0;
	bool flag_lesser = false;

	for(int i = 0; i < this->vec_keys->size(); i++){

		char *aux = this->vec_keys->at(i);
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
//getchar();
		if(num < 10){

			if(num < lesser){

				lesser = num;
				flag_lesser = true;

//cout << "\nLESSER: " << lesser << endl;
				//getchar();

				if(lesser == 1)
					return lesser;
			}
		}

		//if(!strcmp(type, "average"))
			//sum += num;

	//	if(!strcmp(type, "mm2")){

		//	if(num > bigger)
			//	bigger = num;

			//if(num < lesser)
				//lesser = num;
	//	}
	}

	//if(!strcmp(type, "average"))
	//	return sum / this->vec_keys->size();

	//if(!strcmp(type, "mm2"))
	//	return (bigger + lesser) / 2;

	/* Ad-hoc number */
	if(!flag_lesser)
		return 10;

	return lesser;
}

void Topic::roundWords(int number){

	int num_aux;
	bool flag, stop;
	char *aux;

//cout << "\n---------------BEFORE!-------------\n";
//for(int i = 0; i < this->vec_keys->size(); i++)
	//cout << "\nVEC: " << this->vec_keys->at(i);
//cout << endl;
//getchar();
	for(int i = 0; i < this->vec_keys->size(); i++){

		num_aux = 1;
		flag = false;
		stop = false;
		aux = this->vec_keys->at(i);

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

//cout << "\n---------------AFTER!-------------\n";
//for(int i = 0; i < this->vec_keys->size(); i++)
//	cout << "\nVEC: " << this->vec_keys->at(i);
//cout << endl;
}

void Topic::findKeywords(){

	char **words;
	int frequency[100000];

	if(!(words = new char* [sizeof(char) * 100000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);	
	}

	for(int i = 0; i < 100000; i++){

		if(!(words[i] = new char[sizeof(char) * MAX_NUM_LETRAS])){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	Sentence *s = this->g->getIni();
	int cur_topic = s->getTopic(), index = 0;

	strcpy(words[0], "end_of_matrix\0");
	frequency[0] = -1;

	while(s != NULL){
		
//cout << endl << endl;
//cout << cur_topic << endl;

//for(int i = 0; i < s->getNumPalavrasStemmStop(); i++)
//	cout << (s->getSentenca())[i] << " ";

		if(cur_topic != s->getTopic()){

//cout << "TOPIC CHANGE: " << cur_topic << "->" << s->getTopic() << endl;
			addKeyword(cur_topic, words, frequency);

			cur_topic = s->getTopic();
			strcpy(words[0], "end_of_matrix\0");
			frequency[0] = -1;
			index = 0;
		}
	
//cout << "\nNum_palavras: " << s->getNumPalavrasStemmStop() << endl;
	
		for(int i = 0; i < s->getNumPalavrasStemmStop(); i++){

//cout << "\nWord: " << aux[i] << "\n";

			int has_word = hasWord(words, s->getWord(i));

			if(has_word == -1){

//cout << "Nao tinha (" << words[index] << ")\n";
				strcpy(words[index], s->getWord(i));
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

		s = s->getProx();
	}

	/* Add last topic */
	addKeyword(cur_topic, words, frequency);

//cout << "\n(FINAL) AVERAGE: " << averageKeys((char*)"average") << endl;
//getchar();
	roundWords(averageKeys((char*)"average"));

	for(int i = 0; i < 100000; i++)
		delete words[i];

	delete words;

	clusteringKeywords();
	findNewKeywords();
}

void Topic::getTopics(){

	Sentence *s = this->g->getIni();
	int index = 0;
	bool flag;

	this->vec_topics[index] = s->getTopic();
	this->vec_topics[++index] = -1;

	while(s != NULL){

		flag = false;

		for(int i = 0; this->vec_topics[i] != -1 && !flag; i++){

			if(s->getTopic() == this->vec_topics[i])
				flag = true;
		}

		if(!flag){

			this->vec_topics[index] = s->getTopic();
			this->vec_topics[++index] = -1;
		}

		s = s->getProx();
	}
}

int Topic::getNumTopics(){

	Sentence *s = this->g->getIni();
	int topics[10000], size = 0;
	bool flag;

	topics[0] = -1;

	while(s != NULL){

		flag = false;

		for(int i = 0; topics[i] != -1 && !flag; i++){

			if(s->getTopic() == topics[i])
				flag = true;
		}

		if(!flag){

			topics[size] = s->getTopic();
			topics[++size] = -1;
		}

		s = s->getProx();
	}

	return size;
}

void Topic::findNewKeywords(){

	char **words;
	int frequency[100000];

	if(!(words = new char* [sizeof(char) * 100000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);	
	}

	for(int i = 0; i < 100000; i++){

		if(!(words[i] = new char[sizeof(char) * MAX_NUM_LETRAS])){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(EXIT_FAILURE);
		}
	}
	
	Sentence *s = this->g->getIni();
	int cur_topic, num_topics = getNumTopics(), index = 0;
	bool flag;

	if(!(this->vec_topics = new int[sizeof(int) * num_topics])){

		cout << endl << "ERROR! Memory Overflow!" << endl << endl;
		exit(EXIT_FAILURE);
	}

//cout << "\nNUM_TOPICS: " << num_topics << endl;

	getTopics();

	removeFromVector(this->vec_keys->size(), this->vec_keys);

	strcpy(words[0], "end_of_matrix\0");
	frequency[0] = -1;

	for(int i = 0; this->vec_topics[i] != -1; i++){

		cur_topic = this->vec_topics[i];
		s = this->g->getIni();

//cout << "\nCur_topic: " << cur_topic << endl << endl;
	
		while(s != NULL){

			if(s->getTopic() == cur_topic){
	
				for(int i = 0; i < s->getNumPalavrasStemmStop(); i++){

					int has_word = hasWord(words, s->getWord(i));

					if(has_word == -1){

						strcpy(words[index], s->getWord(i));
						frequency[index] = 1;

						strcpy(words[index+1], "end_of_matrix\0");
						frequency[index+1] = -1;

						index++;
					}

					else 
						frequency[has_word]++;
				}
			}

			s = s->getProx();
		}

		addKeyword(cur_topic, words, frequency);

		strcpy(words[0], "end_of_matrix\0");
		frequency[0] = -1;
		index = 0;
	}

//cout << "\n(FINAL) AVERAGE: " << averageKeys((char*)"average") << endl;
//getchar();
	roundWords(averageKeys((char*)"average"));

	for(int i = 0; i < 100000; i++)
		delete words[i];

	delete words;
}

void Topic::addCSTChange(int from_s, int from_doc, int to_s, int to_doc, int n_rels, float v_rels){

	Treshold_CST t_cst;
	bool flag = false;

	t_cst.from_s = from_s;
	t_cst.from_doc = from_doc;
	t_cst.to_s = to_s;
	t_cst.to_doc = to_doc;
	t_cst.n_rels = n_rels;
	t_cst.v_rels = v_rels;

	for(int index = 0; index < this->vec_treshold_cst->size() && !flag; index++){

		if((this->vec_treshold_cst->at(index).from_s == t_cst.from_s &&
				this->vec_treshold_cst->at(index).from_doc == t_cst.from_doc &&
					this->vec_treshold_cst->at(index).to_s == t_cst.to_s &&
						this->vec_treshold_cst->at(index).to_doc == t_cst.to_doc) ||
			(this->vec_treshold_cst->at(index).from_s == t_cst.to_s &&
				this->vec_treshold_cst->at(index).from_doc == t_cst.to_doc &&
					this->vec_treshold_cst->at(index).to_s == t_cst.from_s &&
						this->vec_treshold_cst->at(index).to_doc == t_cst.from_doc))
			flag = true;
	}

	if(!flag)
		this->vec_treshold_cst->push_back(t_cst);
}
 
void Topic::findCSTRelations(char type_1[5], char type_2[50]){

	Sentence *s = this->g->getIni();
	int sum_i = 0;
	float sum_f = 0.0;

	this->vec_treshold_cst = new vector<Treshold_CST>();

	if(!this->vec_treshold_cst){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	Edge *e = NULL;
	Sentence *s_aux = NULL;

	while(s != NULL){

		for(int i = 0; i < s->getSizeVecArestaCortada(); i++){

			e = s->getAresta(i);
			s_aux = e->getS();
			sum_i = 0;
			sum_f = 0.0;

//if(e->getTotalPesosCST() > 0.0)
	//cout << "D" << (s->getOrigemTexto()+1) << "_S" << (s->getPosicaoOriginal()+1) << " (" << s->getTopic() << ") == D" << (s_aux->getOrigemTexto()+1) << "_S" << (s_aux->getPosicaoOriginal()+1) << " (" << s_aux->getTopic() << ") (Weight: " << e->getTotalPesosCST() << ")" << endl;
//getchar();

			if(e->getTotalPesosCST() > 0.0 && s_aux->getTopic() != s->getTopic()){

				sum_f += e->getTotalPesosCST();
				sum_i += e->getSizeVecCST();

		//cout << "Sum_f: "<< sum_f << endl;
		//cout << "Sum_i: "<< sum_i << endl;

				addCSTChange(s->getPosicaoOriginal(), s->getOrigemTexto(), 
								s_aux->getPosicaoOriginal(), s_aux->getOrigemTexto(), 
									sum_i, sum_f);
			}
		}

		s = s->getProx();
	}
	
cout << endl << endl;

	float treshold_unit = 0.0, treshold_value = 0.0, bigger_v = 0.0, lesser_v = 10000.0;
	int bigger_n = 0, lesser_n = 10000;

	for(int i = 0; i < this->vec_treshold_cst->size(); i++){

		//cout << "From_S: " << this->vec_treshold_cst->at(i).from_s << endl;
		//cout << "From_Doc: " << this->vec_treshold_cst->at(i).from_doc << endl;
		//cout << "To_S: " << this->vec_treshold_cst->at(i).to_s << endl;
		//cout << "To_Doc: " << this->vec_treshold_cst->at(i).to_doc << endl;
		//cout << "N_rels: " << this->vec_treshold_cst->at(i).n_rels << endl;
		//cout << "V_rels: " << this->vec_treshold_cst->at(i).v_rels << endl << endl;

		treshold_unit += (float) this->vec_treshold_cst->at(i).n_rels;
		treshold_value += this->vec_treshold_cst->at(i).v_rels;

		if(this->vec_treshold_cst->at(i).n_rels > bigger_n)
			bigger_n = this->vec_treshold_cst->at(i).n_rels;

		if(this->vec_treshold_cst->at(i).v_rels > bigger_v)
			bigger_v = this->vec_treshold_cst->at(i).v_rels;

		if(this->vec_treshold_cst->at(i).n_rels < lesser_n)
			lesser_n = this->vec_treshold_cst->at(i).n_rels;

		if(this->vec_treshold_cst->at(i).v_rels < lesser_v)
			lesser_v = this->vec_treshold_cst->at(i).v_rels;
	}

	if(!strcmp(type_2, "average")){

		if(this->vec_treshold_cst->size() > 0){

			treshold_unit /= ((float) this->vec_treshold_cst->size());
			treshold_value /= ((float) this->vec_treshold_cst->size());
		}

		else {

			treshold_unit = 0.0;
			treshold_value = 0.0;
		}
	}

	else {

		treshold_unit = ((float)(bigger_n + lesser_n)) / 2.0;
		treshold_unit = (bigger_v + lesser_v) / 2.0;
	}

	//cout << "\nTreshold_Unit: " << treshold_unit << endl;
	//cout << "Treshold_Value: " << treshold_value << endl;

	int size = this->vec_treshold_cst->size();

	for(int i = 0; i < size; i++)
		this->vec_treshold_cst->pop_back();

	delete this->vec_treshold_cst;


	clusteringCST(type_1, treshold_unit, treshold_value);
}

void Topic::clusteringCST(char type_1[5], float treshold_unit, float treshold_value){

	Edge *e;
	Sentence *s = this->g->getIni(), *s_aux, *s_pointer;

	int index_topic = s->getTopic(), sum_i, aux_id = 0, aux_sd_i = 0;
	float sum_f;

	this->vec_groups_cst = new vector<Group_CST>();

	if(!this->vec_groups_cst){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	Group_CST aux_cst;
	aux_cst.id = aux_id++;

	while(s != NULL && s->getTopic() == index_topic){

		aux_cst.sentence[aux_sd_i] = s->getPosicaoOriginal();
		aux_cst.document[aux_sd_i++] = s->getOrigemTexto();
	
		s = s->getProx();
	}

	if(s != NULL)
		index_topic = s->getTopic();

	aux_cst.sentence[aux_sd_i] = -1;
	aux_cst.document[aux_sd_i] = -1;

	this->vec_groups_cst->push_back(aux_cst);

	bool flag_group;
	int bigger_sum_i, bigger_id;
	float bigger_sum_f;

	while(s != NULL){
//cout << "S: " << s->getPosicaoOriginal() << " - D: " << s->getOrigemTexto() << endl;
//getchar();
/*
		cout << "S: " << s->getPosicaoOriginal() << " - D: " << s->getOrigemTexto() << endl;
		flag_group = false;

		for(int i = 0; i < s->getSizeVecArestaCortada(); i++){
				
			e = s->getAresta(i);

			if(e->getSizeVecCST() > 0){
			
				flag_group = true;	
				s_aux = e->getS();
				cout << "S_aux: " << s_aux->getPosicaoOriginal() << " - D_aux: " << s_aux->getOrigemTexto() << endl;
			}
		}

		if(flag_group){

		}

		else {

			Sentence *aux2 = this->g->getIni();
			Group_CST aux_cst;

			aux_cst.id = aux_id++;
			aux_sd_i = 0;

			while(aux2 != NULL){

				if(aux2->getTopic() == index_topic){

					aux_cst.sentence[aux_sd_i] = aux2->getPosicaoOriginal();
					aux_cst.document[aux_sd_i++] = aux2->getOrigemTexto();
				}

				aux2 = aux2->getProx();
			}

			aux_cst.sentence[aux_sd_i] = -1;
			aux_cst.document[aux_sd_i] = -1;

			this->vec_groups_cst->push_back(aux_cst);
		}
	
		s = s->getProx();
	}
*/
		sum_i = 0;
		sum_f = 0.0;
		bigger_sum_i = 0;
		bigger_sum_f = 0.0;
		bigger_id = -1;
		flag_group = false;

		s_pointer = s;
/*
cout << "s_pointer_S: " << s_pointer->getPosicaoOriginal() << "\n";
cout << "s_pointer_D: " << s_pointer->getOrigemTexto() << "\n";
cout << "s_pointer_T: " << s_pointer->getTopic() << "\n";
cout << "index_topic: " << index_topic << "\n";

for(int jjj = 0; jjj < this->vec_groups_cst->size(); jjj++){
	for(int kkk = 0; this->vec_groups_cst->at(jjj).sentence[kkk] != -1; kkk++){
		cout << this->vec_groups_cst->at(jjj).sentence[kkk] << " ";
	}
}

cout << endl;
for(int jjj = 0; jjj < this->vec_groups_cst->size(); jjj++){
	for(int kkk = 0; this->vec_groups_cst->at(jjj).document[kkk] != -1; kkk++){
		cout << this->vec_groups_cst->at(jjj).document[kkk] << " ";
	}
}
*/
		for(int j = 0; j < this->vec_groups_cst->size(); j++){

			s = s_pointer;
			sum_i = 0;
			sum_f = 0.0;

			while(s != NULL && s->getTopic() == index_topic){

//cout << "S_inside: " << s->getPosicaoOriginal() << " - D_inside: " << s->getOrigemTexto() << endl; 
//cout << "SIZE: " << s->getSizeVecArestaCortada() << endl;
				for(int i = 0; i < s->getSizeVecArestaCortada(); i++){

					e = s->getAresta(i);

					if(e->getTotalPesosCST() > 0.0){

						s_aux = e->getS();

//cout << "S_aux: " << s_aux->getPosicaoOriginal() << " - D_aux: " << s_aux->getOrigemTexto() << endl;
//getchar();
						for(int k = 0; this->vec_groups_cst->at(j).sentence[k] != -1; k++){

							int aux_s = this->vec_groups_cst->at(j).sentence[k];
							int aux_d = this->vec_groups_cst->at(j).document[k];

							if(aux_s == s_aux->getPosicaoOriginal() && aux_d == s_aux->getOrigemTexto()){

								if((aux_d != s->getOrigemTexto() || (abs(aux_s - s->getPosicaoOriginal()) > 1)) &&
									e->getTotalPesosCST() > 0.0){
	//cout << "\naux_d: " << aux_d << "\n";
	//cout << "aux_s: " << aux_s << "\n";
	//cout << "s_s: " << s->getPosicaoOriginal() << "\n";
	//cout << "s_d: " << s->getOrigemTexto() << "\n";
									sum_f += e->getTotalPesosCST();
									sum_i += e->getSizeVecCST();
									flag_group = true;

	//cout << "Sum_f: " << sum_f << endl;
	//cout << "Sum_i: " << sum_i << endl;

//cout << "\nSOMOU!!!\n";
//getchar();
								}
							}
						}
					}
				}

				s = s->getProx();
			}

			if(!strcmp(type_1, "unit")){

				if(sum_i > bigger_sum_i){

					bigger_sum_i = sum_i;
					bigger_id = this->vec_groups_cst->at(j).id;
//cout << "\n1!!!\n";
//getchar();
				}

				else if(sum_i == bigger_sum_i && bigger_sum_i > 0){

					if(sum_f > bigger_sum_f){

						bigger_sum_f = sum_f;
						bigger_id = this->vec_groups_cst->at(j).id;
//cout << "\n2!!!\n";
//getchar();
					}
				}
			}

			else {

				if(sum_f > bigger_sum_f){

					bigger_sum_f = sum_f;
					bigger_id = this->vec_groups_cst->at(j).id;
//cout << "\n3!!!\n";
//getchar();
				}

				else if(sum_f == bigger_sum_f && bigger_sum_f > 0.0){

					if(sum_i > bigger_sum_i){

						bigger_sum_i = sum_i;
						bigger_id = this->vec_groups_cst->at(j).id;
//cout << "\n4!!!\n";
//getchar();
					}
				}
			}
		}

		if(flag_group){

//cout << "\nINSIDE!!!\n";
//getchar();
			for(int l = 0; l < this->vec_groups_cst->size(); l++){

				if(this->vec_groups_cst->at(l).id == bigger_id){

					int m;
					for(m = 0; this->vec_groups_cst->at(l).sentence[m] != -1; m++);

					Sentence *aux3 = this->g->getIni();

					while(aux3 != NULL){

						if(aux3->getTopic() == index_topic){

								this->vec_groups_cst->at(l).sentence[m] = aux3->getPosicaoOriginal();
								this->vec_groups_cst->at(l).document[m++] = aux3->getOrigemTexto();
								this->vec_groups_cst->at(l).sentence[m] = -1;
								this->vec_groups_cst->at(l).document[m] = -1;
						}

						aux3 = aux3->getProx();
					}
				}
			}
		}

		else {
//cout << "\nOUTSIDE!!!!!\n";
//getchar();
			Sentence *aux2 = this->g->getIni();
			Group_CST aux_cst;

			aux_cst.id = aux_id++;
			aux_sd_i = 0;

			while(aux2 != NULL){

				if(aux2->getTopic() == index_topic){

					aux_cst.sentence[aux_sd_i] = aux2->getPosicaoOriginal();
					aux_cst.document[aux_sd_i++] = aux2->getOrigemTexto();
				}

				aux2 = aux2->getProx();
			}

			aux_cst.sentence[aux_sd_i] = -1;
			aux_cst.document[aux_sd_i] = -1;

			this->vec_groups_cst->push_back(aux_cst);
		}

/*
for(int jjj = 0; jjj < this->vec_groups_cst->size(); jjj++){
	for(int kkk = 0; this->vec_groups_cst->at(jjj).sentence[kkk] != -1; kkk++){
		cout << this->vec_groups_cst->at(jjj).id << " ";
	}
}
cout << endl;
for(int jjj = 0; jjj < this->vec_groups_cst->size(); jjj++){
	for(int kkk = 0; this->vec_groups_cst->at(jjj).sentence[kkk] != -1; kkk++){
		cout << this->vec_groups_cst->at(jjj).sentence[kkk] << " ";
	}
}

cout << endl;
for(int jjj = 0; jjj < this->vec_groups_cst->size(); jjj++){
	for(int kkk = 0; this->vec_groups_cst->at(jjj).document[kkk] != -1; kkk++){
		cout << this->vec_groups_cst->at(jjj).document[kkk] << " ";
	}
}
cout << endl << endl;
//getchar();
*/
		if(s != NULL)
			index_topic = s->getTopic();

	}

/*
for(int iiii = 0; iiii < this->vec_groups_cst->size(); iiii++){

	cout << "\nID: " << this->vec_groups_cst->at(iiii).id << endl;

	cout << "Vec_Sent: ";
	for(int jjjj = 0; this->vec_groups_cst->at(iiii).sentence[jjjj] != -1; jjjj++)
		cout << this->vec_groups_cst->at(iiii).sentence[jjjj] << " ";

	cout << "\nVec_Doc: ";
	for(int jjjj = 0; this->vec_groups_cst->at(iiii).document[jjjj] != -1; jjjj++)
		cout << this->vec_groups_cst->at(iiii).document[jjjj] << " ";

	cout << endl << endl;
}
*/

//cout << "\nBefore!!!\n";
	//s = this->g->getIni();
//while(s != NULL){

//	cout << s->getTopic() << " ";
//	s=s->getProx();
//}
	s = this->g->getIni();
	bool flag;

	while(s != NULL){

		flag = false;

		for(int i = 0; this->vec_groups_cst->size() && !flag; i++){

			for(int j = 0; this->vec_groups_cst->at(i).sentence[j] != -1 && !flag; j++){

				if(s->getPosicaoOriginal() == this->vec_groups_cst->at(i).sentence[j] &&
					s->getOrigemTexto() == this->vec_groups_cst->at(i).document[j]){

					s->setTopic(this->vec_groups_cst->at(i).id);
					flag = true;
				}
			}
		}

		s = s->getProx();
	}

//cout << "\nAfter!!!\n";

//	s = this->g->getIni();
//while(s != NULL){

//	cout << s->getTopic() << " ";
//	s=s->getProx();
//}
	int size = this->vec_groups_cst->size();
	setGlobalTopics(size);

	for(int i = 0; i < size; i++)
		this->vec_groups_cst->pop_back();

	delete this->vec_groups_cst;
}

/*
void Topic::findCSTRelations_OLD(char type[5]){

	Sentence *s = this->g->getIni();
	int bigger_i = 0, sum_i = 0, cur_topic = -1, prev_topic = -1;
	bool flag;
	float bigger_f = 0.0, sum_f = 0.0;

	Edge *e = NULL;
	Sentence *s_aux = NULL;

	while(s != NULL){

		flag = false;

		for(int i = 0; i < s->getSizeVecArestaTotal(); i++){

			e = s->getAresta(i);
			s_aux = e->getS();

			if(s_aux->getTopic() != s->getTopic()){

				cur_topic = s_aux->getTopic();

				if(!flag){

					prev_topic = cur_topic = s_aux->getTopic();
					flag = true;
				}

				if(prev_topic == cur_topic){

					if(!strcmp(type, "value")){

						for(int j = 0; j < e->getSizeVecCST(); j++)
							sum_f += e->getCST(j);
					}

					else
						sum_i += e->getSizeVecCST();
				}

				else {

//cout << "\nTexto: " << s->getOrigemTexto() << " -- Pos: " << s->getPosicaoOriginal() << endl;
//cout << "\nSENT: ";
//for(int ii = 0; ii < s->getNumPalavras(); ii++)
//	cout << (s->getSentencaOriginal())[ii] << " ";

//cout << "\nAUX: texto-> " << s_aux->getOrigemTexto() << " -- pos-> " << s_aux->getPosicaoOriginal() << endl;
//for(int ii = 0; ii < s_aux->getNumPalavras(); ii++)
//	cout << (s_aux->getSentencaOriginal())[ii] << " ";

					if(!strcmp(type, "value")){

						cout << "\nTopic_Change: " << s->getTopic() << "_" << prev_topic << "_" << sum_f << endl;

						addCSTChange(s->getTopic(), prev_topic, sum_f);
						sum_f = 0.0;

						for(int j = 0; j < e->getSizeVecCST(); j++)
							sum_f += e->getCST(j);
					}

					else {

						cout << "\nTopic_Change: " << s->getTopic() << "_" << prev_topic << "_" << sum_i << endl;

						addCSTChange(s->getTopic(), prev_topic, sum_i);
						sum_i = e->getSizeVecCST();

					}

					prev_topic = cur_topic;
				}
			}
		}

		if(!s_aux->getProx()){

			if(!strcmp(type, "value")){

				cout << "\nTopic_Change: " << s->getTopic() << "_" << prev_topic << "_" << sum_f << endl;

				addCSTChange(s->getTopic(), prev_topic, sum_f);
				sum_f = 0.0;

				for(int j = 0; j < e->getSizeVecCST(); j++)
					sum_f += e->getCST(j);
			}

			else {

				cout << "\nTopic_Change: " << s->getTopic() << "_" << prev_topic << "_" << sum_i << endl;

				addCSTChange(s->getTopic(), prev_topic, sum_i);
				sum_i = e->getSizeVecCST();
			}
		}

		s = s->getProx();
	}

	sumCSTResults();

	////////////////// ARRUMAR PAR o sumCSTResults também trabalhar com floats!!!!!!!!!!!!!!!
	if(!strcmp(type, "value"))
		clusteringCSTValue();

	else
		clusteringCSTUnit();
}
*/

void Topic::sumCSTResults(){

	vector<char*> *vec_to_from;

	if(!(vec_to_from = new vector<char*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	int to_1, from_1, value_1;
	int to_2, from_2, value_2;

	for(int i = 0; i < this->vec_cst_aux->size()-1; i++){

		getToFromValue(this->vec_cst_aux->at(i), &to_1, &from_1, &value_1);

		for(int j = i+1; j < this->vec_cst_aux->size(); j++){

			getToFromValue(this->vec_cst_aux->at(j), &to_2, &from_2, &value_2);

			if(to_1 == to_2 && from_1 == from_2)
				value_1 += value_2;
		}

		if(!hasVerified(vec_to_from, to_1, from_1)){

			addVec(vec_to_from, to_1, from_1, -1);
			addVec(this->vec_cst, to_1, from_1, value_1);
		}
	}

	int size = vec_to_from->size();

	for(int i = 0; i < size; i++)
		vec_to_from->pop_back();

	delete vec_to_from;
}

void Topic::addVec(vector<char*> *vec, int to, int from, int value){

	char *aux;

	if(!(aux = new char [sizeof(char) * 10000])){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);	
	}

	if(value >= 0){

		if(sprintf(aux, "%d_%d_%d", to, from, value) < 0){

			cout << endl << "ERROR! Conversion Failed" << endl << endl;
			exit(EXIT_FAILURE);
		}
	}

	else {

		if(sprintf(aux, "%d_%d", to, from) < 0){

			cout << endl << "ERROR! Conversion Failed" << endl << endl;
			exit(EXIT_FAILURE);
		}
	}

	vec->push_back(aux);
}

bool Topic::hasVerified(vector<char*> *vec, int to, int from){

	char aux[10000];

	if(sprintf(aux, "%d_%d", to, from) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < vec->size(); i++){

		if(!strcmp(vec->at(i), aux))
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

	Sentence *s = this->g->getIni();
	char aux[10000];
	int from, to;
	int value, treshold = calculateTresholdCSTUnit((char*)"average");

//cout << "\nTRESHOLD: " << treshold << endl << endl;

//	for(int i = 0; i < this->vec_cst->size(); i++)
//		cout << "\nvec_cst_before: " << this->vec_cst->at(i);

	for(int i = this->vec_cst->size()-1; i >= 0; i--){

		int j;

//cout << "\nvec: " << this->vec_cst->at(i);

		for(j = 0; (this->vec_cst->at(i))[j] != '_'; j++)
			aux[j] = (this->vec_cst->at(i))[j];

		aux[j] = '\0';
		to = atoi(aux);

		int k, i_aux = 0;

		for(k = j+1; (this->vec_cst->at(i))[k] != '_'; k++)
			aux[i_aux++] = (this->vec_cst->at(i))[k];

		aux[i_aux] = '\0';
		from = atoi(aux);

		i_aux = 0;

		for(int m = k+1; (this->vec_cst->at(i))[m] != '\0'; m++)
			aux[i_aux++] = (this->vec_cst->at(i))[m];

		aux[i_aux] = '\0';

		value = atoi(aux);

////////// VERIFICAR SE O ALGARISMO 0 (ZERO) PODE ENTRAR NA JOGADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(value >= treshold && from != to){

	//	cout << "\nFROM: " << from << " -- TO: " << to << endl << endl;

			Sentence *s = this->g->getIni();

			while(s != NULL){

				if(s->getTopic() == from){

					cout << "\nS->FROM: " << s->getTopic() << " -- S->TO: " << to << endl;
					s->setTopic(to);
				}

				s = s->getProx();
			}

			/* Mudar em todo o vec_change_topics */
			for(int ii = this->vec_cst->size()-1; ii >= 0; ii--){

				for(j = 0; (this->vec_cst->at(ii))[j] != '_'; j++);

				char aux_to[10000];

				if(sprintf(aux_to, "%d", to) < 0){

					cout << endl << "ERROR! Conversion Failed" << endl << endl;
					exit(EXIT_FAILURE);
				}

				k, i_aux = 0;

				for(k = j+1; (this->vec_cst->at(ii))[k] != '_'; k++)
					aux[i_aux++] = (this->vec_cst->at(ii))[k];

				aux[i_aux] = '\0';
				
				if(atoi(aux) == from){

					i_aux = 0;

					for(int m = k+1; (this->vec_cst->at(ii))[m] != '\0'; m++)
						aux[i_aux++] = (this->vec_cst->at(ii))[m];

					aux[i_aux] = '\0';

					for(j = 0; (this->vec_cst->at(ii))[j] != '_'; j++);

					(this->vec_cst->at(ii))[j+1] = '\0';
					strcat((this->vec_cst->at(ii)), aux_to);
					strcat((this->vec_cst->at(ii)), "_"); 
					strcat((this->vec_cst->at(ii)), aux);
					strcat((this->vec_cst->at(ii)), "\0");

//cout << "\nNEW_VEC ------> " << (this->vec_cst->at(ii)) << endl << endl;
				}
			}
//////////////////////////////////////////////////////////////
				//s = this->g->getIni();

				//while(s != NULL){

					//cout << "Topic: " << s->getTopic() << endl;
					//s = s->getProx();
			//	}
/////////////////////////////////////////////////////////////
		}
	}

//cout << endl;

//	s = this->g->getIni();

//	while(s != NULL){

	//	cout << "Topic: " << s->getTopic()+1 << endl;
	//	s = s->getProx();
	//}
}

void Topic::clusteringCSTValue(){

	Sentence *s = this->g->getIni();
	char aux[10000];
	int from, to;
	float value, treshold = calculateTresholdCSTValue((char*)"average");

//cout << "\nTRESHOLD: " << treshold << endl << endl;

	//for(int i = 0; i < this->vec_cst->size(); i++)
		//cout << "\nvec_cst_before: " << vec_cst->at(i);

	for(int i = this->vec_cst->size()-1; i >= 0; i--){

		int j;

//cout << "\nvec: " << vec_cst->at(i);

		for(j = 0; (this->vec_cst->at(i))[j] != '_'; j++)
			aux[j] = (this->vec_cst->at(i))[j];

		aux[j] = '\0';
		to = atoi(aux);

		int k, i_aux = 0;

		for(k = j+1; (this->vec_cst->at(i))[k] != '_'; k++)
			aux[i_aux++] = (this->vec_cst->at(i))[k];

		aux[i_aux] = '\0';
		from = atoi(aux);

		i_aux = 0;

		for(int m = k+1; (this->vec_cst->at(i))[m] != '\0'; m++)
			aux[i_aux++] = (this->vec_cst->at(i))[m];

		aux[i_aux] = '\0';

		value = atof(aux);

////////// VERIFICAR SE O ALGARISMO 0 (ZERO) PODE ENTRAR NA JOGADA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(value >= treshold && from != to){

	//	cout << "\nFROM: " << from << " -- TO: " << to << endl << endl;

			Sentence *s = this->g->getIni();

			while(s != NULL){

				if(s->getTopic() == from){

					//cout << "\nS->FROM: " << s->getTopic() << " -- S->TO: " << to << endl;
					s->setTopic(to);
				}

				s = s->getProx();
			}

			/* Mudar em todo o vec_change_topics */
			for(int ii = this->vec_cst->size()-1; ii >= 0; ii--){

				for(j = 0; (this->vec_cst->at(ii))[j] != '_'; j++);

				char aux_to[10000];

				if(sprintf(aux_to, "%d", to) < 0){

					cout << endl << "ERROR! Conversion Failed" << endl << endl;
					exit(EXIT_FAILURE);
				}

				k, i_aux = 0;

				for(k = j+1; (this->vec_cst->at(ii))[k] != '_'; k++)
					aux[i_aux++] = (this->vec_cst->at(ii))[k];

				aux[i_aux] = '\0';
				
				if(atoi(aux) == from){

					i_aux = 0;

					for(int m = k+1; (this->vec_cst->at(ii))[m] != '\0'; m++)
						aux[i_aux++] = (this->vec_cst->at(ii))[m];

					aux[i_aux] = '\0';

					for(j = 0; (this->vec_cst->at(ii))[j] != '_'; j++);

					(this->vec_cst->at(ii))[j+1] = '\0';
					strcat((this->vec_cst->at(ii)), aux_to);
					strcat((this->vec_cst->at(ii)), "_"); 
					strcat((this->vec_cst->at(ii)), aux);
					strcat((this->vec_cst->at(ii)), "\0");

//cout << "\nNEW_VEC ------> " << (this->vec_cst->at(ii)) << endl << endl;
				}
			}
//////////////////////////////////////////////////////////////
	/*			s = this->g->getIni();

				while(s != NULL){

					cout << "Topic: " << s->getTopic() << endl;
					s = s->getProx();
				}
	*/
/////////////////////////////////////////////////////////////
		}
	}
/*
	s = this->g->getIni();

	while(s != NULL){

		cout << "Topic: " << s->getTopic() << endl;
		s = s->getProx();
	}
*/
}

/*
void Topic::addCSTChange(int to, int from, int value){

	char *cst_change = (char*) malloc(sizeof(char) * (MAX_NUM_LETRAS + 1000));

	if(sprintf(cst_change, "%d_%d_%d", to, from, value) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(EXIT_FAILURE);
	}

	this->vec_cst_aux->push_back(cst_change);
}

void Topic::addCSTChange(int to, int from, float value){

	char *cst_change = (char*) malloc(sizeof(char) * (MAX_NUM_LETRAS + 1000));

	if(sprintf(cst_change, "%d_%d_%f", to, from, value) < 0){

		cout << endl << "ERROR! Conversion Failed" << endl << endl;
		exit(EXIT_FAILURE);
	}

	this->vec_cst_aux->push_back(cst_change);
}
*/
vector<char*>* Topic::getVecCST(){

	return this->vec_cst;
}

int Topic::calculateTresholdCSTUnit(char *type){

	int sum = 0, value = 0;
	int bigger = 0, lesser = 1;

	for(int i = 0; i < this->vec_cst->size(); i++){

		int j, k, int_aux = 0;
		char aux[10000];

		for(j = 0; (this->vec_cst->at(i))[j] != '_'; j++);
		for(k = j+1; (this->vec_cst->at(i))[k] != '_'; k++);
		
		for(int m = k+1; (this->vec_cst->at(i))[m] != '\0'; m++)
			aux[int_aux++] = (this->vec_cst->at(i))[m];

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

//cout << "\nSUM: " << sum << endl;
//cout << "\nSIZE: " << this->vec_cst->size() << endl;
//cout << "\nSUM/VEC_CST-SIZE: " << sum / this->vec_cst->size() << endl;
//getchar();


	if(!strcmp(type, "average"))
		return sum / this->vec_cst->size();	

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2;

	return 0;
}

float Topic::calculateTresholdCSTValue(char *type){

	float sum = 0.0, value = 0.0;
	float bigger = 0.0, lesser = 1.0;

	for(int i = 0; i < this->vec_cst->size(); i++){

		int j, k, int_aux = 0;
		char aux[10000];

		for(j = 0; (this->vec_cst->at(i))[j] != '_'; j++);
		for(k = j+1; (this->vec_cst->at(i))[k] != '_'; k++);
		
		for(int m = k+1; (this->vec_cst->at(i))[m] != '\0'; m++)
			aux[int_aux++] = (this->vec_cst->at(i))[m];

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
		return sum / this->vec_cst->size();	

	if(!strcmp(type, "mm2"))
		return (bigger + lesser) / 2.0;

	return 0.0;
}
