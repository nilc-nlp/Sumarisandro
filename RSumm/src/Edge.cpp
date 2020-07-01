#include "../include/Edge.h"

Edge::Edge(){

	if(!(this -> cst = new vector<float>()) ||
		!(this -> cst_rel = new vector<char*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		return;
	}

	setAresta(NULL, 0, -1, NULL);
	setCortada(false);
}

Edge::Edge(Sentence *s, float peso, float cst, char *cst_rel){

	if(!(this -> cst = new vector<float>()) ||
		!(this -> cst_rel = new vector<char*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		return;
	}

	setAresta(s, peso, cst, cst_rel);
	setCortada(false);
}

Edge::Edge(Sentence *s, float peso){

	if(!(this -> cst = new vector<float>()) ||
		!(this -> cst_rel = new vector<char*>())){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		return;
	}

	setAresta(s, peso);
	setCortada(false);
}

Edge::~Edge(){

	int size = this -> cst -> size();

	for(int i = 0; i < size; i++)
		this -> cst -> pop_back();

	delete this -> cst;

	size = this -> cst_rel -> size();

	for(int i = 0; i < size; i++)
		this -> cst_rel -> pop_back();

	delete this -> cst_rel;
}

void Edge::setCortada(bool cortada){

	this -> cortada = cortada;
}

void Edge::setPeso(float peso){

	this -> peso = peso;
}

void Edge::setCST(float cst){

	this -> cst -> push_back(cst);
}

void Edge::setCSTRelation(char *cst_rel){

	//this -> cst_rel -> push_back(cst_rel);
}

void Edge::setAresta(Sentence *s, float peso, float cst, char *cst_rel){

	setS(s);
	setPeso(peso);
	setCST(cst);
	setCSTRelation(cst_rel);
}

void Edge::setAresta(Sentence *s, float peso){

	setS(s);
	setPeso(peso);
}

void Edge::setS(Sentence *s){

	this -> s = s;
}

bool Edge::getCortada(){

	return this -> cortada;
}

float Edge::getPeso(){

	return this -> peso;
}

char* Edge::getCSTRelation(int index){

	return this -> cst_rel -> at(index);
}

float Edge::getCST(int index){

	return this -> cst -> at(index);
}

float Edge::getTotalPesosCST(){

	float total = 0.0;

	for(int i = 0; i < this -> cst -> size(); i++){

		if(this -> cst -> at(i) > 0.0f)
			total += this -> cst -> at(i);
	}

return total;
}

int Edge::getSizeVecCST(){

	return this -> cst -> size();
}

vector<float>* Edge::getVecCST(){

	return this -> cst;
}

int Edge::getSizeVecCSTRel(){

	return this -> cst_rel -> size();
}

vector<char*>* Edge::getVecCSTRel(){

	return this -> cst_rel;
}

Sentence* Edge::getS(){

	return s;
}
