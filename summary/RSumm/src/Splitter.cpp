/* This program segments a text into sentences, producing a output file with one sentence per line.

	Input: a plain text file
	Output: a plain text file, with the original name + '.seg'
*/

#include "../include/Splitter.h"
#include "../include/language_res.h"

char *tmp_word;

Splitter::Splitter(){

}

//Main function
//input: a file with a text to be segmented
void Splitter::initSplitter(char *argv){
	
	initialize_variables();

	segment(argv);

	finalize_variables();

	if (FLAG_FILE) {
	
		FILE *f_flag=fopen("_ready_.tmp","w");
		fclose(f_flag);
	}
}

//This function initializes global variables
void Splitter::initialize_variables()
{
	tmp_word=(char*) malloc(SIZE_WORD*sizeof(char));
	if (tmp_word==NULL) {
		printf("\tERROR: out of memory!\n");
		finalize_variables();
		exit(1);
	}
}

//This function finalizes global variables
void Splitter::finalize_variables()
{
	free(tmp_word);
}


//This function converts a string to lowercase
char* Splitter::lowercase(char *word)
{
	int i;
	char ch[2];
	strcpy(tmp_word,"\0");
	ch[1]='\0';
	for (i=0;i<strlen(word);++i) {
		ch[0]=tolower(word[i]);
		strcat(tmp_word,ch);
	}
	return(tmp_word);
}


//This function verifies if the character is a valid letter
//- interface with language_res repository
int Splitter::is_letter(char c)
{
	if (letter(c))
		return(TRUE);
	else return(FALSE);
}


//This function verifies if the word is an abbreviation
//- interface with language_res repository
int Splitter::is_abbrev(char *word)
{
	if (abbrev(lowercase(word)))
		return(TRUE);
	else return(FALSE);
}


//This function segments the text into sentences, producing an output file with a sentence per line.
//input: file with the text to be segmented
//output: one file with one sentence per line
void Splitter::segment(char filename[])
{
	FILE *fin, *fout;
	char c, ch[2], buffer[SIZE_BUFFER];
	int b, i, j, k, inside_quotes, inside_par, test_dot, test_segment_found, test_end_file, cont_sent;
	//fin: arquivo com texto-fonte; fout: arquivo em que as sentenças delimitadas serão armazenadas
	//b: contador para o buffer; i: contador para leitura do buffer e processamento; j, k: contadores auxiliares
	//inside_quotes e inside_par: indicadores da presença de aspas e sinais de parênteses, colchetes e chaves
	//test_dot: teste usado para verificar se o ponto é de fato um ponto delimitador de sentença
	//test_segment_found: teste usado para verificar se uma sentença foi delimitada
	//test_end_file: teste usado para verificar se o fim do arquivo foi atingidio

	//A variável cont_sent foi alterada, por Ademar T. Akabane e Rafael Ribaldo, pela necessidade de saber a quantidade de sentenças fragmentadas no texto
	//cont_sent: conta o número de sentenças do texto

	ch[1]='\0';
	cont_sent = 0;

	//opening files

	if ((fin=fopen(filename,"r"))==NULL) {
		printf("\tERROR: can not open file %s\n",filename);
		finalize_variables();
		exit(1);
	}

	char word[10000];

	strcpy(word,filename);
	strcat(word,".seg\0");

	if ((fout=fopen(word,"w"))==NULL) {
		printf("\tERROR: can not create file %s\n",word);
		finalize_variables();
		exit(1);
	}

	//reading the source text
	strcpy(buffer,"\0");
	inside_quotes=FALSE;
	inside_par=0;
	i=0;
	c=(char) fgetc(fin);

	while ((!(feof(fin))) || (i<strlen(buffer))) {

		//guaranteeing that the buffer is not full in this point
		if ((strlen(buffer)>0) && (i==strlen(buffer))) {
			for (i=0;i<strlen(buffer);i++)
				fprintf(fout,"%c",buffer[i]);
			strcpy(buffer,"\0");
		}

		//creating buffer
		test_end_file=TRUE;
		b=strlen(buffer);
		while ((!(feof(fin))) && (b<SIZE_BUFFER-1)) {
			buffer[b]=c;
			b++;
			c=(char) fgetc(fin);
			if (test_end_file)
				test_end_file=FALSE;
		}

		if (!test_end_file)
			buffer[b]='\0';

		//analyzing the buffer, looking for sentence boundaries
		test_segment_found=FALSE;
		i=0;
		while ((i<strlen(buffer)) && (!test_segment_found)) {

			//treating quotes: segments should not exist inside quotes
			if (buffer[i]=='"')
				inside_quotes=!inside_quotes;

			//treating special symbols for quotes
			else if (buffer[i]==-109)
				inside_quotes=TRUE;
			else if (buffer[i]==-108)
				inside_quotes=FALSE;

			//treating open and close parenthetical signals: segments should not exist inside them
			else if ((buffer[i]=='(') || (buffer[i]=='[') || (buffer[i]=='{'))
				inside_par++;
			else if (((buffer[i]==')') || (buffer[i]==']') || (buffer[i]=='}')) && (inside_par>0))
				inside_par--;

			//new line: new sentence found
			else if (buffer[i]=='\n') {
				j=0;
				while ((j<=i) && ((buffer[j]==' ') || (buffer[j]=='\n') || (buffer[j]=='\t')))
					j++;
				for (;j<=i;j++)
					fprintf(fout,"%c",buffer[j]);
				inside_quotes=FALSE;
				inside_par=0;
				fprintf(fout,"\n");
				test_segment_found=TRUE;

				//rearranging the buffer
				k=0;
				for (j=i+1;j<=strlen(buffer);j++) {
					buffer[k]=buffer[j];
					k++;
				}
				i=-1;
			}

			//exclamation and interrogation points: new sentence found
			else if (((buffer[i]=='!') || (buffer[i]=='?')) && (!inside_quotes) && (!inside_par)) {
				j=0;
				while ((j<=i) && ((buffer[j]==' ') || (buffer[j]=='\n') || (buffer[j]=='\t')))
					j++;
				for (;j<=i;j++)
					fprintf(fout,"%c",buffer[j]);
				fprintf(fout,"\n");
				test_segment_found=TRUE;
				cont_sent++;

				//rearranging the buffer
				k=0;
				for (j=i+1;j<=strlen(buffer);j++) {
					buffer[k]=buffer[j];
					k++;
				}
				i=-1;
			}

			//dot: treating all possible cases
			else if ((buffer[i]=='.') && (!inside_quotes) && (!inside_par)) {
				test_dot=TRUE;

				//number
				if ((i-1>=0) && (((buffer[i-1]>='0') && (buffer[i-1]<='9')) || (buffer[i-1]==' ')) &&
				   (i+1<strlen(buffer)) && (buffer[i+1]>='0') && (buffer[i+1]<='9'))
			   		test_dot=FALSE;

				//abbreviation
				if (test_dot) {
					strcpy(word,"\0");
					j=i-1;
					while ((j>=0) && (is_letter(buffer[j])))
						j--;
					j++;
					while (j<i) {
						ch[0]=buffer[j];
						strcat(word,ch);
						j++;
					}
					if (is_abbrev(word))
						test_dot=FALSE;
				}

				//reticences
				if (test_dot) {
					if (((i-1>=0) && (buffer[i-1]=='.')) ||
					   ((i+1<strlen(buffer)) && (buffer[i+1]=='.')))
				   		test_dot=FALSE;
				}

				//web address and e-mail
				if (test_dot) {
					if ((i+1<strlen(buffer)) && (is_letter(buffer[i+1]))) {
						strcpy(word,"\0");
						j=i-1;
						while ((j>=0) && (buffer[j]!=' '))
							j--;
						j++;
						while (j<i) {
							ch[0]=buffer[j];
							strcat(word,ch);


							j++;
						}
						if ((strstr(word,"http")!=NULL) || (strstr(word,"ftp")!=NULL) || (strstr(word,"www")!=NULL) || (strstr(word,"@")!=NULL))
							test_dot=FALSE;
					}
				}

				//finally, verifying whether the dot represents a sentence boundary
				if (test_dot) {
					j=0;
					while ((j<=i) && ((buffer[j]==' ') || (buffer[j]=='\n') || (buffer[j]=='\t')))
						j++;
					for (;j<=i;j++)
						fprintf(fout,"%c",buffer[j]);
					fprintf(fout,"\n");
					test_segment_found=TRUE;
					cont_sent++;

					//rearranging the buffer
					k=0;
					for (j=i+1;j<=strlen(buffer);j++) {
						buffer[k]=buffer[j];
						k++;
					}
					i=-1;
				}
			}

			//capital letter after quotes/reticences: begining of a new sentence found
			else if ((buffer[i]>='A') && (buffer[i]<='Z') && (!inside_quotes) && (!inside_par)) {
				j=i-1;
				while ((j>=0) && ((buffer[j]==' ') || (buffer[j]=='\t')))
					j--;
				if ((j>=0) && ((buffer[j]=='"') || ((j-1>=0) && (buffer[j]=='.') && (buffer[j-1]=='.')))) {
					j=0;
					while ((j<i) && ((buffer[j]==' ') || (buffer[j]=='\n') || (buffer[j]=='\t')))
						j++;
					for (;j<i;j++)
						fprintf(fout,"%c",buffer[j]);
					fprintf(fout,"\n");
					test_segment_found=TRUE;
					cont_sent++;

					//rearranging the buffer
					k=0;
					for (j=i;j<=strlen(buffer);j++) {
						buffer[k]=buffer[j];
						k++;
					}
					i=-1;
				}
			}

			i++;
		}
	}

	//print the rest of the buffer (if there is something else)
	if (strlen(buffer)>0) {
		j=0;
		while ((j<strlen(buffer)) && ((buffer[j]==' ') || (buffer[j]=='\n') || (buffer[j]=='\t')))
			j++;
		for (;j<strlen(buffer);j++)
			fprintf(fout,"%c",buffer[j]);
		fprintf(fout,"\n");
		cont_sent++;
	}

	//fprintf(fout, "^%d", cont_sent);

	//closing files
	fclose(fin);
	fclose(fout);

	return;
}
