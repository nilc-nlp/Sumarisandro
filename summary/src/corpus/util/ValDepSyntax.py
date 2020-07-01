# -*- coding: utf-8 -*-
'''
Created on Apr 30, 2012

@author: feralvam
'''

from corpus.reader.pbrconll import PropbankBrConllCorpusReader

PATH_ROOT = "/Users/feralvam/Documents/Dropbox/Projeto Mestrado/Densenvolvimento/APSBr/resources/"

def  cmp_sent(dep1, dep2):
    words1 = [node["word"] for node in dep1.nodelist if node["rel"]!="TOP"]
    words2 = [node["word"] for node in dep2.nodelist if node["rel"]!="TOP"]
    return words1==words2

def cmp_depgraph(dep1, dep2):
    errors = []
    for node1, node2 in zip(dep1.nodelist, dep2.nodelist):
        if node1["rel"] =="TOP": continue
        if not (node1["head"] == node2["head"]):
        #if not ((node1["head"] == node2["head"]) and (node1["rel"] == node2["rel"])):
            if "ARG" in node1["rel"]: continue
            errors.append( (node1["address"], (node1["head"], node1["rel"]), (node2["head"], node2["rel"]) ) )
    return errors

def to_str(dep):
    s = ""
    for node in dep.nodelist:
        if node["rel"]!= "TOP":
            s+= "{:} ".format(node["word"])
    return s


if __name__ == '__main__':
    
    column_types_conllx = ["id", "words", "lemma", "ignore", "pos", "feat", "head", "deprel"]    
    column_types_pbr = ["id", "words", "lemma", "pos", "feat", "head", "deprel"]

    reader_conllx = PropbankBrConllCorpusReader(PATH_ROOT, "CETENFolha_CoNLL-X.txt", column_types_conllx, None , "FCL", False , False, "utf-8")
    reader_pbr = PropbankBrConllCorpusReader(PATH_ROOT, "PBrDep_Full.conll", column_types_pbr, None , "FCL", False , False, "utf-8")
    
    writer_conllx = open(PATH_ROOT + "JustInCoNLL-X.txt", "w")
    writer_pbr = open(PATH_ROOT + "JustInPropbankBr.txt", "w")
    writer_error = open(PATH_ROOT + "ErrorsInTransformation.txt", "w")

    print "Getting sentences..."
    pbr_sents = [pbr_s for pbr_s in reader_pbr.dep_parsed_sents()]
    conllx_sents = [conllx_s for conllx_s in reader_conllx.dep_parsed_sents()]

    print "Initiating process"
    ins_num = 0
    erro_num = 0
    for pbr_s in pbr_sents:
        ins_num+=1
        print "Processing instance {:}".format(ins_num)
        for conllx_s in conllx_sents:
            if cmp_sent(pbr_s, conllx_s):
                # It's the same sentence
                # Compare dependency graphs
                errors = cmp_depgraph(pbr_s, conllx_s) 
                if  errors != []:
                    erro_num += 1
                    writer_error.write("{:}. ".format(erro_num))
                    writer_error.write(to_str(pbr_s))
                    writer_error.write("\n")
                    writer_error.write(errors.__str__())
                    writer_error.write("\n\n")
                    
                conllx_sents.remove(conllx_s)
                break
        else:
            # Couldn't find the sentence in the CoNLL-X corpus
            writer_pbr.write(to_str(pbr_s))
            writer_pbr.write("\n\n")
    
    for conllx_s in conllx_sents:
        writer_conllx.write(to_str(conllx_s))
        writer_conllx.write("\n\n")
    
    writer_error.close()
    writer_pbr.close()
    writer_conllx.close()
    
    print "Process finished"
    print "{:} errors found".format(erro_num)
    
    
                
                
        
    
        
        
    
    