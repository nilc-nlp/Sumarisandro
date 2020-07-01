#include "../include/Library.h"

#define TRUE 1
#define FALSE 0

int letter(char c){

	if (((c>='a') && (c<='z')) ||
           ((c>='A') && (c<='Z')) ||
           ((c>='0') && (c<='9')) ||
           (c=='-') || (c=='_') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') || (c=='�') || (c=='�') || (c=='�') ||
           (c=='�') || (c=='�') ||
           (c=='�') || (c=='�'))
		return(TRUE);
	else return(FALSE);
}


//This function verifies if a word is an abbreviation
int abbrev(char *word) {
	if ((!strcmp(word,"a")) ||
	(!strcmp(word,"aa")) ||
	(!strcmp(word,"abr")) ||
	(!strcmp(word,"abrev")) ||
	(!strcmp(word,"adv")) ||
	(!strcmp(word,"ago")) ||
	(!strcmp(word,"al")) ||
	(!strcmp(word,"alm")) ||
	(!strcmp(word,"alq")) ||
	(!strcmp(word,"ant")) ||
	(!strcmp(word,"ap")) ||
	(!strcmp(word,"aprox")) ||
	(!strcmp(word,"apto")) ||
	(!strcmp(word,"arc")) ||
	(!strcmp(word,"arceb")) ||
	(!strcmp(word,"art")) ||
	(!strcmp(word,"ass")) ||
	(!strcmp(word,"at")) ||
	(!strcmp(word,"av")) ||
	(!strcmp(word,"bpo")) ||
	(!strcmp(word,"brig")) ||
	(!strcmp(word,"c")) ||
	(!strcmp(word,"cal�")) ||
	(!strcmp(word,"cap")) ||
	(!strcmp(word,"caps")) ||
	(!strcmp(word,"car")) ||
	(!strcmp(word,"card")) ||
	(!strcmp(word,"cb")) ||
	(!strcmp(word,"cel")) ||
	(!strcmp(word,"cf")) ||
	(!strcmp(word,"cia")) ||
	(!strcmp(word,"cient")) ||
	(!strcmp(word,"cm")) ||
	(!strcmp(word,"c�d")) ||
	(!strcmp(word,"com")) ||
	(!strcmp(word,"comte")) ||
	(!strcmp(word,"c�n")) ||
	(!strcmp(word,"conj")) ||
	(!strcmp(word,"cons")) ||
	(!strcmp(word,"cont")) ||
	(!strcmp(word,"cr�d")) ||
	(!strcmp(word,"cx")) ||
	(!strcmp(word,"d")) ||
	(!strcmp(word,"d�")) ||
	(!strcmp(word,"db")) ||
	(!strcmp(word,"dd")) ||
	(!strcmp(word,"ddd")) ||
	(!strcmp(word,"d�b")) ||
	(!strcmp(word,"dec")) ||
	(!strcmp(word,"def")) ||
	(!strcmp(word,"dep")) ||
	(!strcmp(word,"deps")) ||
	(!strcmp(word,"depto")) ||
	(!strcmp(word,"deptos")) ||
	(!strcmp(word,"desc")) ||
	(!strcmp(word,"desemb")) ||
	(!strcmp(word,"dez")) ||
	(!strcmp(word,"dipl")) ||
	(!strcmp(word,"dir")) ||
	(!strcmp(word,"doc")) ||
	(!strcmp(word,"docs")) ||
	(!strcmp(word,"dr")) ||
	(!strcmp(word,"dra")) ||
	(!strcmp(word,"dr�")) ||
	(!strcmp(word,"dras")) ||
	(!strcmp(word,"dr�s")) ||
	(!strcmp(word,"drs")) ||
	(!strcmp(word,"dupl")) ||
	(!strcmp(word,"dz")) ||
	(!strcmp(word,"ed")) ||
	(!strcmp(word,"educ")) ||
	(!strcmp(word,"eletr")) ||
	(!strcmp(word,"eletr�n")) ||
	(!strcmp(word,"ema")) ||
	(!strcmp(word,"em�")) ||
	(!strcmp(word,"emas")) ||
	(!strcmp(word,"em�s")) ||
	(!strcmp(word,"emb")) ||
	(!strcmp(word,"end")) ||
	(!strcmp(word,"eng")) ||
	(!strcmp(word,"esq")) ||
	(!strcmp(word,"estr")) ||
	(!strcmp(word,"etc")) ||
	(!strcmp(word,"ex")) ||
	(!strcmp(word,"exa")) ||
	(!strcmp(word,"ex�")) ||
	(!strcmp(word,"exas")) ||
	(!strcmp(word,"ex�s")) ||
	(!strcmp(word,"excl")) ||
	(!strcmp(word,"exmo")) ||
	(!strcmp(word,"f")) ||
	(!strcmp(word,"fat")) ||
	(!strcmp(word,"fev")) ||
	(!strcmp(word,"fig")) ||
	(!strcmp(word,"fl")) ||
	(!strcmp(word,"fls")) ||
	(!strcmp(word,"f�")) ||
	(!strcmp(word,"fob")) ||
	(!strcmp(word,"fr")) ||
	(!strcmp(word,"funai")) ||
	(!strcmp(word,"g")) ||
	(!strcmp(word,"gal")) ||
	(!strcmp(word,"ge")) ||
	(!strcmp(word,"gen")) ||
	(!strcmp(word,"grav")) ||
	(!strcmp(word,"h")) ||
	(!strcmp(word,"ha")) ||
	(!strcmp(word,"hab")) ||
	(!strcmp(word,"he")) ||
	(!strcmp(word,"hz")) ||
	(!strcmp(word,"ib")) ||
	(!strcmp(word,"ibid")) ||
	(!strcmp(word,"ilma")) ||
	(!strcmp(word,"ilm�")) ||
	(!strcmp(word,"ilmas")) ||
	(!strcmp(word,"ilm�s")) ||
	(!strcmp(word,"ilmo")) ||
	(!strcmp(word,"ilmos")) ||
	(!strcmp(word,"inc")) ||
	(!strcmp(word,"ind")) ||
	(!strcmp(word,"inst")) ||
	(!strcmp(word,"int")) ||
	(!strcmp(word,"jan")) ||
	(!strcmp(word,"jd")) ||
	(!strcmp(word,"jr")) ||
	(!strcmp(word,"jul")) ||
	(!strcmp(word,"jun")) ||
	(!strcmp(word,"k")) ||
	(!strcmp(word,"kg")) ||
	(!strcmp(word,"khz")) ||
	(!strcmp(word,"km")) ||
	(!strcmp(word,"l")) ||
	(!strcmp(word,"ling")) ||
	(!strcmp(word,"lit")) ||
	(!strcmp(word,"ltda")) ||
	(!strcmp(word,"m")) ||
	(!strcmp(word,"maga")) ||
	(!strcmp(word,"mag�")) ||
	(!strcmp(word,"magas")) ||
	(!strcmp(word,"mag�s")) ||
	(!strcmp(word,"maj")) ||
	(!strcmp(word,"mal")) ||
	(!strcmp(word,"mar")) ||
	(!strcmp(word,"m�x")) ||
	(!strcmp(word,"me")) ||
	(!strcmp(word,"m�d")) ||
	(!strcmp(word,"min")) ||
	(!strcmp(word,"m�n")) ||
	(!strcmp(word,"ml")) ||
	(!strcmp(word,"mm")) ||
	(!strcmp(word,"mn")) ||
	(!strcmp(word,"mons")) ||
	(!strcmp(word,"m�s")) ||
	(!strcmp(word,"n")) ||
	(!strcmp(word,"nac")) ||
	(!strcmp(word,"neg")) ||
	(!strcmp(word,"n�")) ||
	(!strcmp(word,"no")) ||
	(!strcmp(word,"nom")) ||
	(!strcmp(word,"norm")) ||
	(!strcmp(word,"nov")) ||
	(!strcmp(word,"obs")) ||
	(!strcmp(word,"of")) ||
	(!strcmp(word,"op")) ||
	(!strcmp(word,"org")) ||
	(!strcmp(word,"orig")) ||
	(!strcmp(word,"out")) ||
	(!strcmp(word,"p")) ||
	(!strcmp(word,"p�g")) ||
	(!strcmp(word,"p�gs")) ||
	(!strcmp(word,"p�")) ||
	(!strcmp(word,"pe")) ||
	(!strcmp(word,"pg")) ||
	(!strcmp(word,"pl")) ||
	(!strcmp(word,"pol")) ||
	(!strcmp(word,"pp")) ||
	(!strcmp(word,"pq")) ||
	(!strcmp(word,"pr")) ||
	(!strcmp(word,"pref")) ||
	(!strcmp(word,"pres")) ||
	(!strcmp(word,"prof")) ||
	(!strcmp(word,"profa")) ||
	(!strcmp(word,"prof�")) ||
	(!strcmp(word,"profas")) ||
	(!strcmp(word,"prof�s")) ||
	(!strcmp(word,"profs")) ||
	(!strcmp(word,"psic")) ||
	(!strcmp(word,"ql")) ||
	(!strcmp(word,"r")) ||
	(!strcmp(word,"ra")) ||
	(!strcmp(word,"rel")) ||
	(!strcmp(word,"remte")) ||
	(!strcmp(word,"rev")) ||
	(!strcmp(word,"revdo")) ||
	(!strcmp(word,"revma")) ||
	(!strcmp(word,"revmo")) ||
	(!strcmp(word,"revm�")) ||
	(!strcmp(word,"revmos")) ||
	(!strcmp(word,"revm�s")) ||
	(!strcmp(word,"s")) ||
	(!strcmp(word,"sa")) ||
	(!strcmp(word,"s�")) ||
	(!strcmp(word,"sac")) ||
	(!strcmp(word,"sarg")) ||
	(!strcmp(word,"sas")) ||
	(!strcmp(word,"s�s")) ||
	(!strcmp(word,"s�c")) ||
	(!strcmp(word,"s�cs")) ||
	(!strcmp(word,"seg")) ||
	(!strcmp(word,"segs")) ||
	(!strcmp(word,"set")) ||
	(!strcmp(word,"sr")) ||
	(!strcmp(word,"sra")) ||
	(!strcmp(word,"sr�")) ||
	(!strcmp(word,"sras")) ||
	(!strcmp(word,"sr�s")) ||
	(!strcmp(word,"srs")) ||
	(!strcmp(word,"srta")) ||
	(!strcmp(word,"srt�")) ||
	(!strcmp(word,"srtas")) ||
	(!strcmp(word,"srt�s")) ||
	(!strcmp(word,"ss")) ||
	(!strcmp(word,"sta")) ||
	(!strcmp(word,"sto")) ||
	(!strcmp(word,"suj")) ||
	(!strcmp(word,"t")) ||
	(!strcmp(word,"tel")) ||
	(!strcmp(word,"tels")) ||
	(!strcmp(word,"temp")) ||
	(!strcmp(word,"ten")) ||
	(!strcmp(word,"trav")) ||
	(!strcmp(word,"tv")) ||
	(!strcmp(word,"univ")) ||
	(!strcmp(word,"v")) ||
	(!strcmp(word,"v�")) ||
	(!strcmp(word,"vig")) ||
	(!strcmp(word,"vol")) ||
	(!strcmp(word,"vols")) ||
	(!strcmp(word,"vv")))
		return(TRUE);
	else return(FALSE);
}
