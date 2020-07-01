#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from __future__ import unicode_literals

import re

cause = [
    'por isso', 'com isso', 'porque', 'devido (a(o|os)?|à(s)?)',
    'por causa d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'por força d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'por falta d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'em função d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'em virtude d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'em razão d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'em decorrência d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'em conseq(u|ü)ência d(e|a(s)?|o(s)?|um|uns|umas|aquilo|isso|esse|essa)',
    'pois', 'visto que', 'já que', 'uma vez que',
    '(causad|provocad|prejudicad)(o|a)',
]

CS = '|'.join(cause)


def get_cause_expression(s):
    """
    Get all cause expressions (CEs) present in the sentence using "regular
    expressions" (regex).

    """
    a = [(s[i.start():i.end()].strip(), i.start(), i.end()) for i in re.finditer('(\s|^)(graças\s(a(o|os)?|à(s)?))(\s)', s) if s[i.start() - 4:i.start()] not in ['dar ']]
    b = [(s[i.start():i.end()].strip(), i.start(), i.end()) for i in re.finditer('(\s|^)(((por)\s\w+(ar|er|ir))|(' + CS + '))(\s|\.|,)', s)]
    return a + b

def insert_cause_expressions(s):
    tag1, tag2 = ' <aspect SRL="WHY">', '</aspect> '
    a, b, tam = 0, 0, len(tag1) + len(tag2)
    expressions = sorted(get_cause_expression(s), key=lambda tup: tup[1])
    for it in expressions:
        s = s[:it[1] + a] + tag1 + it[0] + tag2 + s[it[2] + b:]
        a += tam
        b += tam
    # Return the new sentence
    return s
