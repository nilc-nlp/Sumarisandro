#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from utils.CONLL import (get_CoNLL_tags_from_instance, CoNLL_to_aspects,
        map_CoNLL_tags)

import re
import os
import codecs
import copy

PATH_OUTPUT_CONLL = os.path.dirname(os.path.realpath(__file__)) + \
    '/../src/bin/resources/output/output.conll'


def annotate_instance(sentence, words, semantic_tags, labels, locals_dct):
    """
    Annotate an instance (sentence) with microaspects

        input:
        ------
            sentence = A prisão aconteceu em Tobão da Serra na Grande
                       São Paulo.

            words = [
                'A', 'prisão', 'aconteceu', 'em', 'Taboão da Serra', ',',
                'em', 'a', 'Grande São Paulo', '.'
            ]

            semantic_tags = [
                '* artd', 'inst act tool per', 'fmc vN mv', '--', 'civ *',
                '--', 'sam-', '-sam artd', 'org * heur', '--'
            ]

            labels = [WHO_AGENT, WHO_AFFECTED, WHEN, WHERE, HOW, WHY]

            locals_dct: locals (REPENTINO) dictionary

        output:
        -------
            SRL_sentence = A prisão aconteceu <aspect SRL="WHERE">em Taboão
                           da Serra</aspect>, <aspect SRL="WHERE">em a Grande
                           São Paulo</aspect>.

            annotated_sentence = [A prisão aconteceu em Taboão da Serra, na
                                 Grande São Paulo.]WHERE/'

    """
    # Open CoNLL file
    f = codecs.open(PATH_OUTPUT_CONLL, 'r', encoding='utf-8')
    # Get all CoNLL tags (semantic labels)
    tags = get_CoNLL_tags_from_instance(f)
    # Initialize variables
    n_words = copy.deepcopy(words)
    # Convert CoNLL tags to SRL tags
    tags = CoNLL_to_aspects(
        map_CoNLL_tags(tags),
        words,
        semantic_tags,
        labels,
        locals_dct
    )
    # Add tags "<aspect>...</aspect>" inside the sentence
    for tag in tags[::-1]:
        word = n_words[tag[1]]
        if tag[0].find('(') > -1:
            w = '<aspect SRL="' + tag[0][1:] + '">' + word
            n_words[tag[1]] = w
        if tag[0].find(')') > -1:
            w = word + '</aspect>'
            n_words[tag[1]] = w
    # Get all sentences annotated with SRL
    SRL_sentence = clean_sentence(n_words)
    # Get annotated tags
    AS = list(set([it[0].replace('(', '') for it in tags if it[0] != ')']))
    # Get all sentences annotated with microaspects
    annotated_sentence = '[' + sentence + ']' + '/'.join(AS) + '/'
    # Return all sentences annotated with SRL-microaspects
    return SRL_sentence, annotated_sentence


def clean_sentence(words):
    """
    Process to join all words in a sentence and, then, clean it.

    """
    # Join all words in a sentence
    sentence = ' '.join(words)
    # Clean the sentence
    sentence = sentence.replace(' , ', ', ')
    sentence = sentence.replace(' . ', '.').replace(' .', '.')
    sentence = sentence.replace('( ', '(').replace(' )', ')')
    # Return clean sentence
    return sentence


def run_PALAVRAS_flat(input):
    """
    Get all sentence atributes of the PALAVRAS parser (flat format).
    Generate a list of 6-tuple:

        6-tuple:
        --------
            (word, lemma, semantic, POS, morphosyntax, syntax)

        input:
        ------
            O 	[o] <*> <artd> DET M S @>N  #1->2
            menino 	[menino] <Hbio> N M S @SUBJ>  #2->3
            jogou 	[jogar] <fmc> <vH> <mv> V PS 3S IND VFIN @FS-STA  #3->0
            futebol 	[futebol] <sport> N M S @<ACC  #4->3
            $. #5->0
            </s>
        output:
        -------
            [('O', 'o',  '*+artd', 'DET', 'M+S', '@>N'),
            ('menino', 'menino',  'Hbio', 'N', 'M+S', '@SUBJ'),
            ('jogou', 'jogar',  'fmc+vH+mv', 'V', 'PS+3S+IND+VFIN', '@FS-STA'),
            ('futebol', 'futebol',  '*+artd', 'N', 'M+S', '@<ACC')]

    """
    attributes = []
    lines = input.decode('utf-8').split('\n')
    for line in lines:
        line = re.sub(r' +', ' ', line)
        line = re.sub('(\t|poss\s)', '', line)
        # Ex: ['O', '[o]', '<*>', '<artd>', 'DET', 'M', 'S', '@>N', '#1->2']
        array = line.split(' ')
        # A list must to have more than 2 elements. Ex: ['$.', '#5->0']
        if len(array) < 3:
            continue
        # Get "word" attribute
        word = array[0]
        # Get "lemma" attribute
        lemma = array[1][1:-1]
        # Get "POS", "semantic", "morphosyntax" and "syntax" attributes
        POS, semantic, morphosyntax, syntax, flag = "", "", "", "", 1
        for i in range(2, len(array)):
            try:
                semantic += re.match('<(.+?)>', array[i]).group(1) + '+'
            except:
                if "@" not in array[i] and "#" not in array[i]:
                    if flag:
                        POS, flag = array[i], 0
                    else:
                        morphosyntax += array[i] + '+'
                elif '@' in array[i]:
                    syntax = array[i]
        # Append all attributes in a list
        attributes.append(
            (word, lemma, semantic[:-1], POS, morphosyntax[:-1], syntax)
        )
    # Return a list of sentence attributes
    return attributes
