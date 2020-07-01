'''
Created on 19 Jul 2012

@author: feralvam
'''

import bin.srl.util as util

import cPickle
import os

PATH_TEST = os.path.dirname(__file__) + "/../../bin/resources/corpus/test/"
PATH_OUTPUT = os.path.dirname(__file__) + "/../../bin/resources/output/"
PATH_MODELS = os.path.dirname(__file__) + "/../../bin/resources/models/"

def load_models():
    #print '"Argument Identification" model loaded ...'
    argindet_file = open(PATH_MODELS + "AI_LR-All.bin", "rb")
    argident_sys = cPickle.load(argindet_file)
    #print '"Argument Classification" model loaded ...'
    argclass_file = open(PATH_MODELS + "AC_LR-All.bin", "rb")
    argclass_sys = cPickle.load(argclass_file)
    return argident_sys, argclass_sys


def classify(argident_sys, argclass_sys):
    # print "Testing Argument Identification + Argument Classification"
    # print "Testing Argument Identification"
    argident_sys.set_params(
        path_test=PATH_TEST,
        fileid_test="inputConst.conll",
        fileid_test_dep="inputDep.conll"
    )
    argcands_ident = argident_sys.predict_mix()
    # print "Testing Argument Classification"
    tagged_args = argclass_sys.predict_mix(argcands_ident, True)
    # print "Argument Classification"
    writer_test = open(PATH_OUTPUT + "output.conll", "w")
    util.pprint_output(tagged_args, writer_test)
    # print "Test finished. File created."
