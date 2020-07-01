#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from __future__ import unicode_literals

import re

lexical_score = ['set(s)?', 'gol(s)?', 'jogo(s)?']

LS = '|'.join(lexical_score)


def get_score_expression(s):
    """
    Get all score expressions (CEs) present in the sentence using "regular
    expressions" (regex).

    """
    a = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('\d+\s((' + LS + ')\s)?(a)\s\d+', s)]
    b = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(\d+h)?\d+(m(in)?)\d+((s(eg)?))?(\d+)?', s)]
    return a + b


def insert_score_expressions(s):
    tag1, tag2 = ' <aspect SRL="SCORE">', '</aspect> '
    a, b, tam = 0, 0, len(tag1) + len(tag2)
    expressions = sorted(get_score_expression(s), key=lambda tup: tup[1])
    for it in expressions:
        s = s[:it[1] + a] + tag1 + it[0] + tag2 + s[it[2] + b:]
        a += tam
        b += tam
    # Return the new sentence
    return s
