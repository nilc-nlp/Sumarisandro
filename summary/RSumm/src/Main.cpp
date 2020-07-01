#include "../include/Library.h"
#include "../include/Graph.h"
#include "../include/Salton.h"
#include "../include/CST.h"
#include "../include/Splitter.h"

int main(int argc, char *argv[]){

	char str_dir[5000];

	int aux_arg = -1;

	if(!strcmp(argv[argc-1], "-ponderada") || !strcmp(argv[argc-1], "-unitaria"))
		aux_arg = 1;

	else
		aux_arg = 0;

	if(!aux_arg){
							// -1 RSumm NORMAL
							// -2 RSummNews
		for(int i = 2; i < argc-1; i++){

			char aux_tiling[3000];

			strcpy(aux_tiling, "java -jar ../Tiling.jar ");
			strcat(aux_tiling, argv[i]);
			strcat(aux_tiling, " ");
			strcat(aux_tiling, argv[i]);
			strcat(aux_tiling, "\0");

			if(system(aux_tiling)){

				cout << endl << "ERROR! TextTiling" << endl << endl;
				exit(EXIT_FAILURE);
		
			}

			strcpy(str_dir, argv[i]);
			strcat(str_dir, "\0");

			Splitter().initSplitter(str_dir);
		}
	}

	if(aux_arg){

		for(int i = 2; i < argc-4; i++){

			strcpy(str_dir, argv[i]);
			strcat(str_dir, "\0");
	
			Splitter().initSplitter(str_dir);
		}
	}

	FILE *arq, *arq_summary, *arq_stoplist;
	Graph *g = new Graph();
	vector<char*> *cst_rels;
	vector<char*> *rels_sents = new vector<char*>();
	vector<char*> *stopwords = new vector<char*>();
	char *str_aux = NULL, *aux_cst = NULL;

	if(!stopwords || !g || !rels_sents){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	/* Leitura das stopwords de língua portuguesa e armazenamento em um vetor de caracteres. */



					// ../Stoplists RSumm NORMAL
					// Stoplists	RSummNews
	if(arq_stoplist = fopen("../Stoplists/stoplist_portugues.txt", "r")){

		while(!feof(arq_stoplist)){
			
			str_aux = NULL;

			if(str_aux = (char*) malloc(sizeof(char) * MAX_NUM_LETRAS)){

				if(fscanf(arq_stoplist, "%s", str_aux) >= 0)
					stopwords->push_back(str_aux);
			}

			else {

				cout << endl << "ERROR! Memory Overflow" << endl << endl;
				exit(EXIT_FAILURE);
			}
		}

		fclose(arq_stoplist);
	}

	else {

		cout << endl << "ERROR! Couldn't open stoplist file" << endl << endl;
		exit(EXIT_FAILURE);
	}

	int count = 0;
	int dezena = 0;
	bool flag = false;

/* Abaixo há o descobrimento de quantos arquivos existem no diretório SENTER. */

	FILE *arq_qt;
	int count_aux;
	vector<char*> *name_arq = new vector<char*>();

	if(!name_arq){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	if(!aux_arg){
							// -1 RSumm NORMAL
							// -2 RSummNews
		for(int i = 2; i < argc-1; i++)
			name_arq->push_back(argv[i]);
	}

	if(aux_arg){

		for(int i = 2; i < argc-4; i++)
			name_arq->push_back(argv[i]);
	}

	int n_int_aux = 0, posicao;
	int aux_2;

	if(!aux_arg)	// -3 RSumm NORMAL; -4 RSummNews
		aux_2 = argc - 3;

	if(aux_arg)
		aux_2 = argc - 6;

	/* Leitura das sentenças e construção do grafo (um grafo para o todo o cenário multidocumento). */
	/* Número de arquivos (menos o executável e a taxa de compressão) = argc - 2.*/
	while(n_int_aux < aux_2 && !flag){

		posicao = 0;

		char nome_arq[2000];
		
		strcpy(nome_arq, name_arq->at(count));
		strcat(nome_arq, ".seg\0");

//cout << "CLUSTER PAUSE! (Press Enter to continue after changing the correct files)\n";
//getchar();

		if(arq = fopen(nome_arq, "r")){
			
			g->carregar(arq, count++, stopwords, posicao, &dezena, nome_arq);
			fclose(arq);
		}
		
		else {

			flag = true;
			cout << endl << "ERROR! Couldn't open segmented text file" << endl << endl;
		}

		n_int_aux++;
	}

	Salton *salton = new Salton(g);

	if(!salton){

		cout << endl << "ERROR! Memory Overflow" << endl << endl;
		exit(EXIT_FAILURE);
	}

	float taxa_compressao;

	if(!aux_arg)					// -1 RSumm NORMAL; -2 RSummNews
		taxa_compressao = atof(argv[argc-1]);

	else
		taxa_compressao = atof(argv[argc-4]);

	Sentence *s_aux = g->getIni();
	int total_palavras = 0, maior = 0;
	int aux_arq = 0;

	/* Contagem do total de palavras do cluster. */
	for(int i = 0; i < g->getNumNos(); i++){

		if(s_aux->getOrigemTexto() > aux_arq){

			if(total_palavras > maior)
				maior = total_palavras;

			total_palavras = 0;
			aux_arq++;
		}

		total_palavras += s_aux->getNumPalavras();

		s_aux = s_aux->getProx();
	}

	if(total_palavras > maior)
		maior = total_palavras;

	Sentence *s2 = g->getIni();
	int arq_aux = 0;
	
	/* Arredondamento do número máximo de palavras que o sumário pode ter (se >= 0.5, arredonda para cima, senão para baixo). */
	float aux_total = (100.0f - (taxa_compressao * 100.0f)) * ((float) maior) / 100.0f;
	int total = (int) aux_total;

	float f = aux_total - total;

	if(f >= 0.5f)
		aux_total += (1.00f - f);

	total = (int) aux_total;

	salton->ponderarGrafo();

	char usar_cst = 'n';
	char desempate[13] = "padrao";

	if(aux_arg && !strcmp(argv[argc-3], "-cst"))
		usar_cst = 'y';

	CST *cst;

	if(usar_cst == 'y' || usar_cst == 'Y'){

		cst_rels = new vector<char*>();

		if(!cst_rels){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(EXIT_FAILURE);
		}

		FILE *arq_cst;

		if(arq_cst = fopen("../CST_relations/CST_Rels.txt", "r")){

			while(!feof(arq_cst)){
			
				if(aux_cst = (char*) malloc(sizeof(char) * MAX_NUM_LETRAS)){

					if(fscanf(arq_cst, "%s", aux_cst) >= 0)
						cst_rels->push_back(aux_cst);
				}

				else {

					cout << endl << "ERROR! Memory Overflow" << endl << endl;
					exit(EXIT_FAILURE);
				}

				aux_cst = NULL;
			}

			free(aux_cst);
			fclose(arq_cst);
		}

		else {

			cout << endl << "ERROR! Couldn't open CST relations file" << endl << endl;
			exit(EXIT_FAILURE);
		}

		cst = new CST(cst_rels);

		if(!cst){

			cout << endl << "ERROR! Memory Overflow" << endl << endl;
			exit(EXIT_FAILURE);
		}

		char dir_rels_cst[MAX_NUM_LETRAS];

		strcpy(dir_rels_cst, argv[argc-2]);
		strcat(dir_rels_cst, "\0");

		strcpy(desempate, argv[argc-1]);
		strcat(desempate, "\0");

		if(arq_cst = fopen(dir_rels_cst, "r"))
			cst->leituraRelacoes(arq_cst, g);			

		else {

			cout << endl << "ERROR! Couldn't open CST relations file of the texts" << endl << endl;
			exit(EXIT_FAILURE);
		}

		fclose(arq_cst);
	}

	float aux = salton->maiorCos(NULL, NULL);
	int aux_taxa_compressao = 0, num_sentencas_sumario = 0;
	char numero_cluster[5];
	bool stop = false;

	int intersect = 0;
	int num_sent_ext;

	float precision;
	float recall;
	float f_measure;
	
	/* Caminhos propostos por Salton (Denso, Profundo e Segmentado). */

	if(!strcmp(argv[1], "-denso")){

		vector<Sentence*> *vec_denso = salton->caminhoDenso(total, desempate);

		char nome_sumario_denso[1000];

		strcpy(nome_sumario_denso, "summary");	
		
		if(usar_cst == 'n' || usar_cst == 'N')
			strcat(nome_sumario_denso, ".denso\0");

		else if(usar_cst == 'y' || usar_cst == 'Y'){

			if(!strcmp(desempate, "-unitaria"))
				strcat(nome_sumario_denso, ".denso_uni\0");

			else if(!strcmp(desempate, "-ponderada"))
				strcat(nome_sumario_denso, ".denso_pond\0");

			else
				cout << endl << "ERROR! CST decision!" << endl << endl;
		}
	
		/* Caminho Denso */
		if(arq_summary = fopen(nome_sumario_denso, "w+")){

			/* Número de sentenças de um sumário */
//--------------------------------------------------------------
			FILE *arq_num_sent = fopen("summary.num", "w");
			fprintf(arq_num_sent, "%d", (int) vec_denso->size());
			fclose(arq_num_sent);
//--------------------------------------------------------------

			FILE *arq_links, *arq_crono;

			if(!(arq_links = fopen("links.txt", "r"))){

				cout << endl << "ERROR! Links Archive" << endl << endl;
				exit(EXIT_FAILURE);
			}

			if(!(arq_crono = fopen("crono.txt", "w"))){

				cout << endl << "ERROR! Crono Archive" << endl << endl;
				exit(EXIT_FAILURE);
			}

			for(int i = 0; i < vec_denso->size() && !stop; i++){

				Sentence *s = vec_denso->at(i);
				char **auxiliar = s->getSentencaOriginal();

				if(aux_taxa_compressao + s->getNumPalavras() > total){

					if(abs(aux_taxa_compressao + s->getNumPalavras() - total) <= abs(total - aux_taxa_compressao)){

								// position; document; number of words
						fprintf(arq_crono, "%d;%d;%d\n", s->getPosicaoOriginal(), s->getOrigemTexto(), s->getNumPalavras());

						fprintf(arq_summary, "<a href=");
						fprintf(arq_summary, "\"");

						int origem_texto = s->getOrigemTexto();

						rewind(arq_links);

						int aux_origem = 0;
						char link[10000];

						while(aux_origem <= origem_texto){

							fscanf(arq_links, "%s", link);
							aux_origem++;
						}
					
						fprintf(arq_summary, "%s", link);
						fprintf(arq_summary, "\"");
						fprintf(arq_summary, " title=");
						fprintf(arq_summary, "\"");
						fprintf(arq_summary, "Ir para o texto-fonte");
						fprintf(arq_summary, "\"");
						fprintf(arq_summary, " target=");
						fprintf(arq_summary, "\"");
						fprintf(arq_summary, "_blank");
						fprintf(arq_summary, "\"");
						fprintf(arq_summary, ">");

						for(int j = 0; j < s->getNumPalavras(); j++){

							fprintf(arq_summary, "%s ", auxiliar[j]);
							aux_taxa_compressao++;
						}

						fprintf(arq_summary, "</a> ");

						num_sentencas_sumario++;
					}

					else
						stop = true;
				}

				else {

							// position; document; number of words
					fprintf(arq_crono, "%d;%d;%d\n", s->getPosicaoOriginal(), s->getOrigemTexto(), s->getNumPalavras());

					fprintf(arq_summary, "<a href=");
					fprintf(arq_summary, "\"");

					int origem_texto = s->getOrigemTexto();

					rewind(arq_links);

					int aux_origem = 0;
					char link[10000];

					while(aux_origem <= origem_texto){

						fscanf(arq_links, "%s", link);
						aux_origem++;
					}

					fprintf(arq_summary, "%s", link);
					fprintf(arq_summary, "\"");
					fprintf(arq_summary, " title=");
					fprintf(arq_summary, "\"");
					fprintf(arq_summary, "Ir para o texto-fonte");
					fprintf(arq_summary, "\"");
					fprintf(arq_summary, " target=");
					fprintf(arq_summary, "\"");
					fprintf(arq_summary, "_blank");
					fprintf(arq_summary, "\"");
					fprintf(arq_summary, ">");

					for(int j = 0; j < s->getNumPalavras(); j++){

						fprintf(arq_summary, "%s ", auxiliar[j]);
						aux_taxa_compressao++;
					}

					fprintf(arq_summary, "</a> ");

					num_sentencas_sumario++;	
				}
			}

fprintf(arq_summary, "aaaaaaaaaaaaaaaa");
			fclose(arq_summary);
		}

		else
			cout << endl << "Couldn't open final summary file (bushy)" << endl << endl;
	}

	else if(!strcmp(argv[1], "-profundo")){

		aux_taxa_compressao = 0;
		num_sentencas_sumario = 0;
		stop = false;

		vector<Sentence*> *vec_prof = salton->caminhoProfundo(total, desempate);

		char nome_sumario_prof[1000];

		strcpy(nome_sumario_prof, "summary");

		if(usar_cst == 'n' || usar_cst == 'N')
			strcat(nome_sumario_prof, ".prof\0");

		else if(usar_cst == 'y' || usar_cst == 'Y'){

			if(!strcmp(desempate, "-unitaria"))
				strcat(nome_sumario_prof, ".prof_uni\0");

			else if(!strcmp(desempate, "-ponderada"))
				strcat(nome_sumario_prof, ".prof_pond\0");

			else
				cout << endl << "ERROR! CST decision!" << endl << endl;
		}

		/* Caminho Profundo */
		if(arq_summary = fopen(nome_sumario_prof, "w+")){

			for(int i = 0; i < vec_prof->size() && !stop; i++){

				Sentence *s = vec_prof->at(i);
				char **auxiliar = s->getSentencaOriginal();

				if(aux_taxa_compressao + s->getNumPalavras() > total){

					if(abs(aux_taxa_compressao + s->getNumPalavras() - total) <= abs(total - aux_taxa_compressao)){

						for(int j = 0; j < s->getNumPalavras(); j++){

							fprintf(arq_summary, "%s ", auxiliar[j]);
							aux_taxa_compressao++;
						}

						num_sentencas_sumario++;
					}

					else
						stop = true;
				}

				else {
					for(int j = 0; j < s->getNumPalavras(); j++){

						fprintf(arq_summary, "%s ", auxiliar[j]);
						aux_taxa_compressao++;
					}

					num_sentencas_sumario++;
				}
			}

			fclose(arq_summary);
		}

		else
			cout << endl << "Couldn't open final summary file (depth)" << endl << endl;
	}

	else if(!strcmp(argv[1], "-segmentado")){

		vector<Sentence*> *vec_segmentado = salton->caminhoSegmentado(total, desempate, (char*)"all");

		Sentence *s = g->getIni();

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SAVE CLUSTERING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
FILE *arq_after;
arq_after = fopen("agrupamento.txt", "w+");
int i_file, i_aux_file = 0;
char aux_file[30];

while(s != NULL){

//cout << "Topic: " << s->getTopic() << " ";

	i_file = 0;
	while((s->getOriginalFile())[i_file++] != 'D');
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
}
	else
		cout << endl << "ERROR! Wrong Path!" << endl << endl;

//cout << endl << endl;

	int size = stopwords->size();
	
	for(int i = 0; i < size; i++)
		stopwords->pop_back();

	if(usar_cst == 'y'){

		size = cst_rels->size();

		for(int i = 0; i < size; i++)
			cst_rels->pop_back();

		delete cst_rels;
		delete cst;
	}

	size = name_arq->size();

	for(int i = 0; i < size; i++)
		name_arq->pop_back();

	delete name_arq;
	delete stopwords;
	delete g;
	delete salton;

	return EXIT_SUCCESS;
}
