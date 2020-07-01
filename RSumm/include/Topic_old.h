#ifndef TOPIC_H
#define TOPIC_H

#include "../include/Library.h"
#include "../include/Graph.h"
#include "../include/Segment.h"
#include "../include/Sentence.h"
#include "../include/Util.h"

class Topic {

	private:
			Graph *g;
			vector<Segment*> *vec_seg;
			vector<char*> *vec_keys, *vec_change_topics;

	public:
			Topic(Graph *g);
			~Topic();

			vector<char*>* getVecKeys();
			void addSegment(Segment *segment);
			void addKeyword(int topic, char **words, int frequency[100000]);
			void findKeywords();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			int fillVector(vector<char*> *vec_words, vector<int*> *vec_freq, Sentence *s);
			float cosineSimilarity(vector<char*> *vec_words, vector<int*>* vec_freq1, vector<int*> *vec_freq2);
			int hasWord(char **words, char *aux);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			void findSimilarTopics();
			void clusteringKeywords();
			void clusteringTopics();
			void changeTopic(int prev_topic, int cur_topic);
			float similarity(char *keywords_1, char *keywords_2);
			void removeFromVector(int cont, vector<char*> *vec_words);
};

#endif
