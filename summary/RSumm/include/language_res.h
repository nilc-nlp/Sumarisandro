#include "../include/Library.h"

#define TRUE 1
#define FALSE 0

int letter(char c){

	if (((c>='a') && (c<='z')) ||
           ((c>='A') && (c<='Z')) ||
           ((c>='0') && (c<='9')) ||
           (c=='-') || (c=='_') ||
           (c=='á') || (c=='é') || (c=='í') || (c=='ó') || (c=='ú') ||
           (c=='â') || (c=='ê') || (c=='î') || (c=='ô') || (c=='û') ||
           (c=='ã') || (c=='õ') ||
           (c=='à') || (c=='è') || (c=='ì') || (c=='ò') || (c=='ù') ||
           (c=='ä') || (c=='ë') || (c=='ï') || (c=='ö') || (c=='ü') ||
           (c=='ç') || (c=='ñ') ||
           (c=='Á') || (c=='É') || (c=='Í') || (c=='Ó') || (c=='Ú') ||
           (c=='Â') || (c=='Ê') || (c=='Î') || (c=='Ô') || (c=='Û') ||
           (c=='Ã') || (c=='Õ') ||
           (c=='À') || (c=='È') || (c=='Ì') || (c=='Ò') || (c=='Ù') ||
           (c=='Ä') || (c=='Ë') || (c=='Ï') || (c=='Ö') || (c=='Ü') ||
           (c=='Ç') || (c=='Ñ') ||
           (c=='ª') || (c=='°'))
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
	(!strcmp(word,"calç")) ||
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
	(!strcmp(word,"cód")) ||
	(!strcmp(word,"com")) ||
	(!strcmp(word,"comte")) ||
	(!strcmp(word,"côn")) ||
	(!strcmp(word,"conj")) ||
	(!strcmp(word,"cons")) ||
	(!strcmp(word,"cont")) ||
	(!strcmp(word,"créd")) ||
	(!strcmp(word,"cx")) ||
	(!strcmp(word,"d")) ||
	(!strcmp(word,"dª")) ||
	(!strcmp(word,"db")) ||
	(!strcmp(word,"dd")) ||
	(!strcmp(word,"ddd")) ||
	(!strcmp(word,"déb")) ||
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
	(!strcmp(word,"drª")) ||
	(!strcmp(word,"dras")) ||
	(!strcmp(word,"drªs")) ||
	(!strcmp(word,"drs")) ||
	(!strcmp(word,"dupl")) ||
	(!strcmp(word,"dz")) ||
	(!strcmp(word,"ed")) ||
	(!strcmp(word,"educ")) ||
	(!strcmp(word,"eletr")) ||
	(!strcmp(word,"eletrôn")) ||
	(!strcmp(word,"ema")) ||
	(!strcmp(word,"emª")) ||
	(!strcmp(word,"emas")) ||
	(!strcmp(word,"emªs")) ||
	(!strcmp(word,"emb")) ||
	(!strcmp(word,"end")) ||
	(!strcmp(word,"eng")) ||
	(!strcmp(word,"esq")) ||
	(!strcmp(word,"estr")) ||
	(!strcmp(word,"etc")) ||
	(!strcmp(word,"ex")) ||
	(!strcmp(word,"exa")) ||
	(!strcmp(word,"exª")) ||
	(!strcmp(word,"exas")) ||
	(!strcmp(word,"exªs")) ||
	(!strcmp(word,"excl")) ||
	(!strcmp(word,"exmo")) ||
	(!strcmp(word,"f")) ||
	(!strcmp(word,"fat")) ||
	(!strcmp(word,"fev")) ||
	(!strcmp(word,"fig")) ||
	(!strcmp(word,"fl")) ||
	(!strcmp(word,"fls")) ||
	(!strcmp(word,"fº")) ||
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
	(!strcmp(word,"ilmª")) ||
	(!strcmp(word,"ilmas")) ||
	(!strcmp(word,"ilmªs")) ||
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
	(!strcmp(word,"magª")) ||
	(!strcmp(word,"magas")) ||
	(!strcmp(word,"magªs")) ||
	(!strcmp(word,"maj")) ||
	(!strcmp(word,"mal")) ||
	(!strcmp(word,"mar")) ||
	(!strcmp(word,"máx")) ||
	(!strcmp(word,"me")) ||
	(!strcmp(word,"méd")) ||
	(!strcmp(word,"min")) ||
	(!strcmp(word,"mín")) ||
	(!strcmp(word,"ml")) ||
	(!strcmp(word,"mm")) ||
	(!strcmp(word,"mn")) ||
	(!strcmp(word,"mons")) ||
	(!strcmp(word,"mús")) ||
	(!strcmp(word,"n")) ||
	(!strcmp(word,"nac")) ||
	(!strcmp(word,"neg")) ||
	(!strcmp(word,"nº")) ||
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
	(!strcmp(word,"pág")) ||
	(!strcmp(word,"págs")) ||
	(!strcmp(word,"pç")) ||
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
	(!strcmp(word,"profª")) ||
	(!strcmp(word,"profas")) ||
	(!strcmp(word,"profªs")) ||
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
	(!strcmp(word,"revmº")) ||
	(!strcmp(word,"revmos")) ||
	(!strcmp(word,"revmºs")) ||
	(!strcmp(word,"s")) ||
	(!strcmp(word,"sa")) ||
	(!strcmp(word,"sª")) ||
	(!strcmp(word,"sac")) ||
	(!strcmp(word,"sarg")) ||
	(!strcmp(word,"sas")) ||
	(!strcmp(word,"sªs")) ||
	(!strcmp(word,"séc")) ||
	(!strcmp(word,"sécs")) ||
	(!strcmp(word,"seg")) ||
	(!strcmp(word,"segs")) ||
	(!strcmp(word,"set")) ||
	(!strcmp(word,"sr")) ||
	(!strcmp(word,"sra")) ||
	(!strcmp(word,"srª")) ||
	(!strcmp(word,"sras")) ||
	(!strcmp(word,"srªs")) ||
	(!strcmp(word,"srs")) ||
	(!strcmp(word,"srta")) ||
	(!strcmp(word,"srtª")) ||
	(!strcmp(word,"srtas")) ||
	(!strcmp(word,"srtªs")) ||
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
	(!strcmp(word,"vª")) ||
	(!strcmp(word,"vig")) ||
	(!strcmp(word,"vol")) ||
	(!strcmp(word,"vols")) ||
	(!strcmp(word,"vv")))
		return(TRUE);
	else return(FALSE);
}
