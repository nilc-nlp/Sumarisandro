#!/usr/bin/env python
#-*- encoding:utf-8 -*-

import sys
import codecs
import nltk
import re
sys.path.append('src')

from nltk.tokenize import RegexpTokenizer

from bin.srl.main import load_models
from run_ASumm import get_summary


if __name__ == "__main__":
    f = codecs.open(sys.argv[1], 'r', 'UTF-8')
    # Get all sentences of the file (tokenize)
    sent_tokenizer=nltk.data.load('tokenizers/punkt/portuguese.pickle')
    output = re.sub('(\[|\]|\:)', '', f.read().strip())
    RSumm_sentences = sent_tokenizer.tokenize(output)
    # Close file
    f.close()
    # Get a dictionary for locals (REPENTINO)
    f = codecs.open('resources/repentino_locais.txt', 'r', 'UTF-8')
    repentino_locals = {line.strip(): True for line in f.readlines()}
    # Loading models ...
    AI_model, AC_model = load_models()
    # Get document with the maximum size
    max_size = int(sys.argv[2])
    # Get summary
    summary = get_summary(RSumm_sentences, AI_model, AC_model, repentino_locals, max_size)
    # Print all annotated sentences
    #for it in RSumm_sentences:
    for it in summary:
        resp = '<p>' + it + '</p>' 
        print resp.encode('utf-8')
