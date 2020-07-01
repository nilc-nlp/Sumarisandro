#ifndef SEGMENT_H
#define SEGMENT_H

#include "../include/Library.h"
#include "../include/Graph.h"
#include "../include/Sentence.h"

class Segment {

	private:
			vector<Sentence*> *vec;
			char *signature;
			int topic;

	public:
			Segment();
			~Segment();

			void addSentence(Sentence *s);
			Sentence* getSentence(int index);
			int getSize();

			void discoverSignature();

			void setSignature(char *signature);
			void setTopic(int topic);

			char* getSignature();
			int getTopic();
};

#endif
