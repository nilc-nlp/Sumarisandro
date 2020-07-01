#include "../include/Segment.h"

////////////////////////////////////////////////////
/// LEMBRAR DE DESALOCAR 'SEGMENT' NO TOPIC.CPP/////
////////////////////////////////////////////////////

Segment::Segment(){

	if(!(this -> vec = new vector<Sentence*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(1);
	}
}

Segment::~Segment(){

	int size = vec -> size();

	for(int i = 0; i < size; i++)
		vec -> pop_back();

	delete vec;	
}

void Segment::addSentence(Sentence *s){
cout << "\n\n<INSIDE: " << s << ">\n\n";
if(s == NULL)
cout << "<S NULL>";
else
cout << "<S NOT NULL>";
cout << "\n\n1 " << s << ">\n\n";
cout << "\n\n2 " << s << ">\n\n";
cout << "\n\n3 " << s << ">\n\n";
cout << "\n\n4 (last) " << s << ">\n\n";
cout << "\n\nVEC: " << this -> vec << endl << "SIZE: " << vec -> size() << endl;
cout << "\n\nCAP: " << vec -> capacity() << endl << endl;
cout << "\n\nMAX: " << vec -> max_size() << endl << endl;

	this -> vec -> push_back(s);

cout << "\n\nINSIDEEEE!!!(((2)))\n\n";
}

Sentence* Segment::getSentence(int index){

	return this -> vec -> at(index);
}

int Segment::getSize(){

	return this -> vec -> size();
}

void Segment::discoverSignature(){

}

void Segment::setSignature(char *signature){

	strcpy(this -> signature, signature);
}

void Segment::setTopic(int topic){

	this -> topic = topic;
}

char* Segment::getSignature(){

	return this -> signature;
}

int Segment::getTopic(){

	return this -> topic;
}


