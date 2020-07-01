
'''
Created on May 7, 2012

@author: feralvam
'''

import re,subprocess
import unicodedata as ud
from corpus.reader.pbrconll import PropbankBrConllCorpusReader
from nltk.corpus.reader import XMLCorpusReader 
from nltk.cluster.util import cosine_distance
import os

PATH_ROOT = "/Users/feralvam/Documents/Dropbox/Projeto Mestrado/Densenvolvimento/APSBr/resources/"


def validate_senteces():
    reader = open(PATH_ROOT + "PBrDep_Sents.txt", "r")
    
    column_types_pbr = ["id", "words", "lemma", "pos", "feat", "head", "deprel"]
    reader_pbr = PropbankBrConllCorpusReader(PATH_ROOT, "PBrDep.conll", column_types_pbr, None , "FCL", False , False, "utf-8")
        
    lines = [line for line in reader if line!="\n"] 
    for line,sent in zip(lines,reader_pbr.sents()):
        w_sent = ud.normalize('NFC',unicode(line.strip("(*)").split()[0])).lower()
        w_pbr = ud.normalize('NFC',unicode(sent[0])).lower()
        if not w_sent in w_pbr:
            print "Required:"
            print sent
            print "Found:"
            print line
            break
    return 

def extract_sentences():
    
    reader = open(PATH_ROOT + "Bosque_CF_8.0.ad.txt", "r")
   
    writer = open(PATH_ROOT + "PropbankBr_Sentences2.txt","w")
    
    print "Initiating process"
    num_sent = 0
    for line in reader:
        # Check if it is a sentence
        aux = re.search("CF[0-9]+\-[0-9]+",line) 
        if (aux):
            num_sent +=1
            sep = aux.group(0)
            writer.write(line.split(sep)[-1].strip() + "\n\n")
        
    writer.close()
    
    print "Process finished: {:} sentences".format(num_sent)
    
    return 





if __name__ == '__main__':
    pass
    
    
            
