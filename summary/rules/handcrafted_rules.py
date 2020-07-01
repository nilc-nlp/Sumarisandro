#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from utils.functions import run_PALAVRAS_flat

import subprocess
import codecs
import re
import os
import numpy


illocutionary_verbs = [
    'dizer', 'afirmar', 'anunciar', 'informar', 'destacar', 'expressar',
    'referir', 'classificar', 'admitir', 'comentar', 'divulgar', 'opinar', 
    'falar',
]

EXTRA_TAGS = ['H', 'Hprof', 'hum', 'hum *', 'hum * heur', 'hum * newlex', 
              'admin *', 'org *', 'inst *', 'media *', 'party *', 'suborg *']

macroaspects = ['DECLARATION', 'PREDICTION', 'HISTORY', 'GOAL', 'COMPARISON']

PATH = os.path.dirname(os.path.realpath(__file__))

PATH_INPUT = PATH + '/input.txt'

PATH_OUTPUT = PATH + '/flat.txt'

PATH_CALL_PHP = PATH + '/../utils/call_palavras_flat.php'


def is_semantic_tag(tags, semantic_tags):
    for tag in tags.split('+'):
        if tag in semantic_tags:
            return True
    return False


def apply_DECLARATION_rules(attributes):
    for i, attr in enumerate(attributes):
        # Rule 1: if the verb belongs to the illocutionary verbs 
        if attr[3] == 'V' and attr[1] in illocutionary_verbs:
            return True
        # Rule 2: if the sentence has 'segundo'
        elif attr[3] == 'PRP' and attr[1] == 'segundo':
            # Rule 2.1: followed by a determinant
            if i + 1 < len(attributes) and attributes[i + 1][3] == 'DET':
                return True
            # Rule 2.2: followed by a PERSON/ORGANIZATION entity
            elif (i + 1 < len(attributes) and 
                    is_semantic_tag(attributes[i + 1][2], EXTRA_TAGS)):
                return True
        # Rule 3: if the sentence has 'de acordo com'
        elif (i + 1 < len(attributes) and attr[1] == 'de=acordo' and 
                attributes[i + 1][1] == 'com'):
            return True
    return False


def apply_PREDICTION_rules(attributes):
    for i, attr in enumerate(attributes):
        # Rule 1: If the verb tense is future
        if attr[3] == 'V' and 'FUT' in attr[4].split('+'):
            return True
        # Rule 2: If the lemma of the word is 'previsão'
        elif attr[1] == u'previsão':
            return True
    return False


def apply_HISTORY_rules(attributes):
    for i, attr in enumerate(attributes):
        # Rule 1: If the sentence has 'já' ...
        if i + 1 < len(attributes) and attr[1] == u'já':
            morphosyntax = attributes[i + 1][4].split('+')
            # Rule 1.1: ... followed by 'PS' or 'PS/MQP'
            if 'PS' in morphosyntax or 'PS/MQP' in morphosyntax:
                return True
            # Rule 1.2: ... followed by 'IMPF' or 'COND'
            elif 'IMPF' in morphosyntax or 'COND' in morphosyntax:
                return True
        # Rule 2: If the sentence has 'desde'
        elif attr[1] == 'desde':
            return True
        # Rule 3: If the sentence has 'da história'
        elif attr[1] == u'história' and attributes[i - 1][3] == 'DET':
            return True
    return False


def apply_GOAL_rules(attributes):
    for i, attr in enumerate(attributes):
        # Rule 1: If the sentence has 'objetivo'
        if attr[1] == 'objetivo':
            return True
    return False


def apply_COMPARISON_rules(attributes):
    for i, attr in enumerate(attributes):
        # Rule 1: If the sentence has 'em relação|comparação a'
        if re.search(u'em=(relação|comparação)(=a)?', attr[1]): 
            return True
        # Rule 2: If the sentence has 'tão' + ADJ + 'quanto'
        elif (i + 2 < len(attributes) and attr[1] == u'tão' and 
                attributes[i + 1][3] == 'ADJ' and
                attributes[i + 2][1] == 'quanto'):
            return True
        # Rule 3: If the sentence has the verb 'comprar'
        elif attr[1] == 'comparar' and attr[3] == 'V':
            return True
    return False
 

def apply_annotation_rules(sentences) :
    annotated_sentences = []
    # Create a "input.txt" file that contains the input sentence
    f = codecs.open(PATH_INPUT, 'w', 'utf-8')
    for item in sentences:
    	f.write(item + '\r\n\r\n')
    f.close()
    # Run the PALAVRAS by "command line"
    cmd = 'php ' + PATH_CALL_PHP + ' ' + PATH_INPUT + ' > ' + PATH_OUTPUT
    # Run Palavras
    os.system(cmd)
    # Get all the instances/sentences processed by PALAVRAS
    f = codecs.open(PATH_OUTPUT, 'r', 'utf-8')
    output = f.read()
    instances = output.encode('utf-8').strip().split('\n\n')
    # Verify is instance's length is equals to sentence's length
    for instance in instances:
        print instance
   	print '-----------------------------'
    print len(instances), len(sentences)
    if len(instances) != len(sentences):
        msg = 'PALAVRAS error! Instances were not processed correctly.'
        raise NameError(msg)
    # Iterate all instances
    for i, instance in enumerate(instances):
        # Get all attributes from each instance
        attributes, annotation = run_PALAVRAS_flat(instance), ""
        # Apply DECLARATION rules from each instance
        if apply_DECLARATION_rules(attributes) == True:
            annotation += 'DECLARATION/'
        # Apply PREDICTION rules from each instance
        if apply_PREDICTION_rules(attributes) == True:
            annotation += 'PREDICTION/'
        # Apply HISTORY rules from each instance
        if apply_HISTORY_rules(attributes) == True:
            annotation += 'HISTORY/'
        # Apply GOAL rules from each instance
        if apply_GOAL_rules(attributes) == True:
            annotation += 'GOAL/'
        # Apply COMPARISON rules from each instance
        if apply_COMPARISON_rules(attributes) == True:
            annotation += 'COMPARISON/'
        # Delete last '/' 
        if annotation and annotation[-1] == '/':
            annotation = annotation[:-1]
        # Annotate each sentence
        annotated_sentences.append('[' + sentences[i] + ']' + annotation)
    # Return all annotated sentences
    return annotated_sentences
