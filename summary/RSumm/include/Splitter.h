#ifndef SPLITTER_H
#define SPLITTER_H

/*	The functions in this file are language dependent
		- customized for Brazilian Portuguese
*/

#include "../include/Library.h"

#define SIZE_WORD 500		//maximum number of characters in a word
#define SIZE_BUFFER 1500	//maximum number of characters in the buffer used in function 'segment'
#define FLAG_FILE FALSE		//determines whether a flag file should be generated after SENTER runs

class Splitter {

	private:

	public:
		Splitter();

		void initSplitter(char *argv);
		void initialize_variables();
		void finalize_variables();
		char* lowercase(char *word);
		int is_letter(char c);
		int is_abbrev(char *word);
		void segment(char filename[]);

};

#endif
