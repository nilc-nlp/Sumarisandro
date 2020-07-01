#!/usr/bin/env python
#-*- encoding:utf-8 -*-

import re

map_microaspects = {
    'A0': 'WHO_AGENT',
    'A1': 'WHO_AFFECTED',
    'AM-LOC': 'WHERE',
    'AM-TMP': 'WHEN',
    'AM-MNR': 'HOW',
    'AM-CAU': 'WHY',
}

PERSON_TAGS = ['H', 'HH', 'Hattr', 'Hbio', 'Hfam', 'Hideo', 'Hmyth', 'Hnat',
               'Hprof', 'Hsick', 'Htit', 'hum *', 'hum * heur', 'hum * newlex']

ORGANIZATION_TAGS = ['admin *', 'org *', 'inst *', 'media *', 'party *',
                     'suborg *', 'groupind *', 'groupofficial *', 'grouporg *']


def get_CoNLL_tags_from_instance(CoNLL_file):
    """
    Get all CoNLL tags and its positions (position of a word in a sentence):

        Sentence:
        ---------
        O primo Favio comprou um frango para comer de noite.

        input:
        ------
            O        0  -                 (A0*         *
            primo    1  -                 *            *
            Favio    2  -                 *)           *
            comprou  3  comprar           *            *
            um       4  -                 (A1*         *
            frango   5  -                 *            *
            para     6  -                 *)           *
            comer    7  comer             *            *
            de       8  -                 *            (AM-TMP*
            noite    9  -                 *            *)
            .        10 -                 *            *

        output:
        -------
            [("(A0", 0),     (")", 2),
             ("(A1", 4),     (")", 6),
             ("(AM-TMP", 8), (")", 9)]

    """
    # Initialize variables
    tags, pos = [], 0
    # Iterate all lines in the CoNLL_file
    for i, line in enumerate(CoNLL_file.readlines()[:-1]):
        line = line.strip()
        # Find all semantic labels (A0, A1, AM-TMP, etc)
        cm = re.compile(r'(\([\w-]+|\))')
        match = [(it.start(), it.end()) for it in re.finditer(cm, line)]
        # Save the semantic label and its position in a list
        for m in match:
            tags.append((line[m[0]:m[1]], i))
    # Close file
    CoNLL_file.close()
    # Return tags
    return tags


def get_CoNLL_tags_from_file(CoNLL_file):
    """
    Get all CoNLL tags and its positions (position of a word in a sentence):

        Sentence:
        ---------
        O primo Favio comprou um frango para comer de noite.

        input:
        ------
            O        0  -                 (A0*         *
            primo    1  -                 *            *
            Favio    2  -                 *)           *
            comprou  3  comprar           *            *
            um       4  -                 (A1*         *
            frango   5  -                 *            *
            para     6  -                 *)           *
            comer    7  comer             *            *
            de       8  -                 *            (AM-TMP*
            noite    9  -                 *            *)
            .        10 -                 *            *

        output:
        -------
            [("(A0", 0),     (")", 2),
             ("(A1", 4),     (")", 6),
             ("(AM-TMP", 8), (")", 9)]

    """
    # Initialize variables
    tags, all_tags, pos = [], [], 0
    # Iterate all lines in the CoNLL_file
    for l in CoNLL_file.readlines():
        l = l.strip()
        if l != '':
            # Find all semantic labels (A0, A1, AM-TMP, etc)
            cm = re.compile(r'(\([\w-]+|\))')
            mtch = [(it.start(), it.end()) for it in re.finditer(cm, l)]
            # Save the semantic label and its position in a list
            for m in mtch:
                tags.append((l[m[0]:m[1]], pos))
            pos += 1
        else:
            # Save all tags per sentence
            all_tags.append(tags)
            tags, pos = [], 0
    # Close file
    CoNLL_file.close()

    return all_tags


def map_CoNLL_tags(tags):
    """
    Identify the position of ')' simbol in the array "tags".
    This is necessary to determine the words between "(" and ")".

        Sentence:
        ---------
        O primo Favio comprou um frango para comer de noite.

        input:
        ------
            0: ("(A0", 0)
            1: (")", 2),
            2: ("(A1", 4)
            3: (")", 6)
            4: ("(AM-TMP", 8)
            5: (")", 9)

        output:
        -------
        [("(A0", 0, 1), (")", 2, -1), ("(A1", 4, 3), (")", 6, -1),
        ("(AM-TMP", 8, 5), (")", 9, -1)]

    Then ("(A0", 0, 1) -> (")", 2, -1) means that "O primo Favio" (0, 2)
    were tagged with the label "A0".

    """
    new = []
    for i in range(len(tags)):
        # Add position 'j' if the simbol is '(' (open)
        if tags[i][0].find('(') > -1:
            # Initialize breakers
            A, B = 1, 0
            for j in range(i + 1, len(tags)):
                # Identifier open simbol '('
                if tags[j][0].find('(') > -1:
                    A += 1
                # Identifier close simbol ')'
                if tags[j][0].find(')') > -1:
                    B += 1
                # Means that simbol ')' closes simbol '('
                if A - B == 0:
                    new.append((tags[i][0], tags[i][1], j))
                    break
        # Add position '-1' if the simbol is ')' (close)
        else:
            new.append((tags[i][0], tags[i][1], -1))

    return new


def CoNLL_to_aspects(tags, words, semantic_tags, fltr, locals_dct):
    """
    Convert CoNLL tags to "aspects".
    Filter all tags that are not "aspects".

        ---------------------------
        | CoNLL    | Aspects      |
        ---------------------------
        | A0       | WHO_AGENT    |
        | A1       | WHO_AFFECTED |
        | AM-LOC   | WHERE        |
        | AM-TMP   | WHEN         |
        | AM-MNR   | HOW          |
        | AM-CAU   | WHY          |
        ---------------------------

        input:
        ------
        [("(A0", 0, 1), (")", 2, -1), ("(A2", 4, 3), (")", 6, -1)]

        output:
        -------
        [("(WHO_AGENT", 0, 1, 1), (")", 2, -1, 1)]
        where "A0" represents aspect WHO_AGENT and "A2" is not an aspect

    """
    # Initialize list of tags. Last value represent by:
    # 0 (not aspect) or 1 (aspect)
    new = [tuple(list(item) + [0]) for item in tags]
    # Filter map_microspects by aspect (fltr)
    filters = {k: v for k, v in map_microaspects.items() if v in fltr}
    for i in range(len(tags)):
        if tags[i][0].find('(') > -1:
            try:
                # Get the aspect related to CoNLL tag
                aspect = filters[tags[i][0][1:]]
                # Uptade last value
                new[i] = ('(' + aspect, tags[i][1], tags[i][2], 1)
                new[tags[i][2]] = tuple(list(tags[tags[i][2]]) + [1])
            except KeyError:
                pass
    # Return all tags that are "aspects" (last value is 1)
    new = rule_WHO_AGENT(new, words, semantic_tags, locals_dct)
    return [it for it in new if it[3] == 1]


def rule_WHO_AGENT(tags, words, semantic_tags, locals_dct):
    for it, item in enumerate(tags):
        if item[0] in ['(WHO_AGENT', '(WHO_AFFECTED']:
            x, y = item[1], tags[item[2]][1] + 1
            if is_PALAVRAS_semantic_tag(semantic_tags[x:y], words[x:y], locals_dct)\
                    is False:
                lst1, lst2 = list(item), list(tags[item[2]])
                lst1[3], lst2[3] = 0, 0
                tags[it], tags[item[2]] = tuple(lst1), tuple(lst2)
    return tags


def is_PALAVRAS_semantic_tag(semantic_tags, words, locals_dct):
    for i, item in enumerate(semantic_tags):
        if item in PERSON_TAGS + ORGANIZATION_TAGS:
            try:
                locals_dct[words[i]]
            except:
                return True
    return False
