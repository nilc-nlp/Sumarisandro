#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from WHEN_rules import get_temporal_expression

import re


def filter_equal_expressions(A, B):
    i = 0
    while i != len(B):
        j = 0
        while j != len(A):
            comp = re.compile('<aspect SRL="WHERE">(.+?)</aspect>')
            a, b = A[j][0], re.search(comp, B[i][0]).group(1)
            if b in a or a in b:
                del A[j]
                j -= 1
            j += 1
        i += 1
    return A


def delete_annotated_tags(annotated):
    try:
        aspects = re.search(r'\](.+?)$', annotated).group(1).split('/')
        del aspects[aspects.index('WHERE')]
        sentence = re.search(r'^\[(.+?)\][\w/]+$', annotated).group(1)
        annotated = '[' + sentence + ']' + '/'.join(aspects)
    except:
        pass
    return annotated


def delete_temporal_expressions(s, SRL_labels, annotated):
    """
    Delete all temporal expressions that were identified like "WHERE".
    For example:

        Input:
        ------
            s = '<aspect SRL="WHERE">Em a sexta-feira</aspect>, <aspect
                 SRL="WHERE">em encontro com sindicalistas em São Paulo
                 </aspect>, Lula disse que venceria a eleição no primeiro
                 turno.'

            SRL_labels = [
                ('<aspect SRL="WHERE">Em a sexta-feira</aspect>', 0, 45),
                ('<aspect SRL="WHERE">em encontro ... Paulo</aspect>', 47, 118)
            ]

        Output:
        -------
            s = 'Em a sexta-feira, <aspect SRL="WHERE"> em encontro com
                 sindicalistas em São Paulo</aspect>, Lula disse que
                 venceria a eleição em o primeiro turno.'

    """
    tag1, tag2 = '<aspect SRL="WHERE">', '</aspect>'
    a, b, tam = 0, 0, len(tag1) + len(tag2)
    for it in SRL_labels:
        expression = re.search(tag1 + '(.+?)' + tag2, it[0]).group(1)
        # If the expression is a temporal expression, then delete it
        if get_temporal_expression(expression.lower()):
            s = s[:it[1] - a] + expression + s[it[2] - b:]
            a += tam
            b += tam
    # If doesn't exits 'WHERE' tag then delete '/WHERE/' sentence label
    if not re.search('<aspect SRL="WHERE">(.+?)</aspect>', s):
        annotated = delete_annotated_tags(annotated)
    # Return the new sentence
    return s, annotated


def add_location_expressions(s, SRL_labels, annotated):
    """
    Identify another location expressions by rules and tagged them.

        Input:
        ------
            s = 'O Airbus de a Tam acidentado em São Paulo tinha um defeito '
                '<aspect SRL="WHERE">em o reversor de a turbina direita'
                '</aspect>.'

            SRL_labels = [
                ('<aspect SRL="WHERE">em o rever...direita</aspect>', 59, 122)]
            ]

            annotated = '[O Airbus de a Tam acidentado em São Paulo tinha um defeito
                         em o reversor de a turbina direita]WHERE/'

        Output:
        -------
            s = 'O Airbus de a Tam acidentado <aspect RULE="WHERE">em São
                 Paulo</aspect> tinha um defeito <aspect SRL="WHERE">em o
                 reversor de a turbina direita</aspect>.'

            annotated = '[O Airbus de a Tam acidentado em São Paulo tinha um defeito
                          em o reversor de a turbina direita]WHERE/

    """
    V = u'\w|á|é|í|ó|ú|ã|õ|â|ê|ô|à|ü'  # All portugese letters
    c = u'(^|\s)((e|E)m)(\s(a|o)(s)?)?\s([A-Z](' + V + ')+(\s|\.|\,|-|$))+'
    # Identify another location expressions
    expressions = [(s[i.start():i.end()].strip(), i.start(), i.end()) for i in re.finditer(c, s)]
    expressions = filter_equal_expressions(expressions, SRL_labels)
    # Tag new location expressions in the sentence
    tag1, tag2 = ' <aspect SRL="WHERE">', '</aspect> '
    a, b, tam = 0, 0, len(tag1) + len(tag2)
    for it in expressions:
        s = s[:it[1] + a] + tag1 + it[0] + tag2 + s[it[2] + b:]
        a += tam
        b += tam
    # If exists some 'WHERE' tag then annotated the sentence with '/WHERE'
    if re.search('<aspect SRL="WHERE">(.+?)</aspect>', s) and \
            'WHERE' not in annotated:
        annotated += 'WHERE/'
    # Return the new sentence
    return s, annotated


def reforming_WHERE_sentence(sentence, annotated):
    """
    Reforming the sentence previosly annotated by SRL (Semantic Role Labeling)
    with "WHERE" tag:

        1.- Delete all temporal expressions that were identified like "WHERE"
        2.- Identify another location expressions by rules and tagged them.

    """
    # Get all "SRL" labels
    c = re.compile('<aspect SRL="WHERE">(.+?)</aspect>')
    SRL_labels = [(sentence[i.start():i.end()], i.start(), i.end()) for i in re.finditer(c, sentence)]
    # Delete all temporal expressions
    sentence, annotated = delete_temporal_expressions(sentence, SRL_labels, annotated)
    # Apply WHERE rule to get all location expressions
    sentence, annotated = add_location_expressions(sentence, SRL_labels, annotated)
    # Return the new sentence
    return sentence, annotated
