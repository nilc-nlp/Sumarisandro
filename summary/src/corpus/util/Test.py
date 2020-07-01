'''
Created on Sep 14, 2013

@author: feralvam
'''
from corpus.reader.pbrconll import PropbankBrConllCorpusReader

PATH_ROOT_TRAIN = "/Users/feralvam/Dropbox/ICMC/Mestrado/Densenvolvimento/APSBr_Sup/resources/corpus/train/"
PATH_ROOT_TEST = "/Users/feralvam/Dropbox/ICMC/Mestrado/Densenvolvimento/APSBr_Sup/resources/corpus/test/"
PATH_ROOT_OUTPUT = "/Users/feralvam/Dropbox/ICMC/Mestrado/Densenvolvimento/APSBr_Sup/resources/output/"
PATH_ROOT_MODELS = "/Users/feralvam/Dropbox/ICMC/Mestrado/Densenvolvimento/APSBr_Sup/resources/models/"

if __name__ == '__main__':
    # Testing constituent trees
#     column_types = ["id", "words", "lemma", "pos", "feat", "clause", "fclause","tree","srl"]
#     reader_const = PropbankBrConllCorpusReader(PATH_ROOT_TRAIN, 'PropBankBr_Test.conll', column_types, None , "S", False, True, "utf-8")
#     
#     for sent_ins in reader_const.srl_instances(None, None, False):
#         for ins in sent_ins:
#             print ins.roleset
            
    # Testing dependency graphs
    column_types_dep = ["id", "words", "lemma", "pos", "feat", "head", "deprel","fillpred","srl"]
    reader_dep = PropbankBrConllCorpusReader(PATH_ROOT_TEST, 'PBr2012Dep.conll', column_types_dep, None , "FCL", False , False, "utf-8")
    
    for sent_dg in reader_dep.dep_parsed_sents():
        print sent_dg.tree()

    print "Process finished."
    
            