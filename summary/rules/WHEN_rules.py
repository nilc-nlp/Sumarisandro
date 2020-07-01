#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from __future__ import unicode_literals

import re

numbers = [
    'um(a)?', 'dois', 'duas', 'três', 'quatro', 'cinco', 'seis', 'sete',
    'oito', 'nove', 'dez', 'once', 'doze', 'treze', 'quatorze', 'quinze',
    'dezesseis', 'dezessete', 'dezoito', 'dezenove'
]

cardinals = [
    'vinte', 'trinta', 'quarenta', 'cinquenta', 'sessenta', 'setenta',
    'oitenta', 'noventa', 'cem', 'cento'
]

days = [
    'segunda', 'terça', 'quarta', 'quinta', 'sexta', 'sábado', 'domingo'
]

months = [
    'janeiro', 'fevereiro', 'março', 'abril', 'maio', 'junho', 'julho',
    'agosto', 'setembro', 'outubro', 'novembro', 'dezembro'
]

time = [
    'microssegundo', 'segundo', 'minuto', 'hora', 'dia', 'semana', 'mês',
    'ano', 'década', 'milênio', 'semestre', 'bimestre', 'trimestre',
    'época', 'tempo', 'set', 'etapa',
]

adverbs = [
    'hoje', 'amanhã', 'ontem', 'anteontem', 'tarde', 'madrugada', 'noite',
    'meia-noite', 'manhã',
]

demostratives = [
    'em(\s(o|a)(s)?)?', 'de', 'na(s)?', 'no(s)?', 'este(s)?', 'esta(s)?', 'esse(s)?',
    'essa(s)?', 'nesse(s)?', 'nessa(s)?', 'neste(s)?', 'nesta(s)?',
    'desse(s)?', 'dessa(s)?', 'deste(s)?', 'desta(s)?', 'aquele(s)?',
    'aquela(s)?', 'naquele(s)?', 'naquela(s)?', 'daquele(s)?', 'daquela(s)?',
    'há', 'depois de', 'no dia', 'aos',
]

mode_exp = [
    'antecipadamente', 'finalmente', 'inicialmente', 'oportunamente',
    'novamente', 'primeiramente', 'primitivamente', 'primordialmente',
    'definitivamente', 'prontamente', 'repentinamente', 'simultaneamente',
    'sincronicamente', 'subitamente', 'normalmente',
]

referencial_exp = [
    'anteriormente', 'antigamente', 'atualmente', 'correntemente',
    'futuramente', 'imediatamente', 'modernamente', 'posteriormente',
    'postumamente', 'precedentemente', 'presentemente', 'recentemente',
    'subseqüentemente', 'tardiamente', 'ulteriormente', 'ultimamente',
]

duration_exp = [
    'brevemente', 'eternamente', 'momentaneamente', 'provisoriamente',
    'temporariamente', 'instantaneamente',
]

frequency_exp = [
    'diariamente', 'semanalmente', 'mensalmente', 'anualmente', 'raramente',
    'constantemente', 'bimestralmente', 'trimestralmente', 'permanetemente',
    'periodicamente', 'quinzenalmente', 'semestralmente',
    '(quo|co)tidianamente', 'eventualmente', 'habitualmente', 'regularmente',
    'reiteradamente', 'rotineiramente', 'usualmente', 'ocasionalmente',
    'freq(u|ü)entemente', 'amiúde', 'todos os dias', 'com freq(u|ü)ência',
    '(várias|muitas|algumas|às|por)\svezes',
]

N = '|'.join(numbers)
C = '|'.join(cardinals)
D = '|'.join(days)
T = '|'.join(time)
M = '|'.join(months)
AT = '|'.join(adverbs)
PP = '|'.join(demostratives)
FE = '|'.join(frequency_exp)
S = 'á|é|í|ó|ú|ã|õ|â|ê|ô|à|ü'

def filter_temporal_expressions(ETs):
    """
    Delete all temporal expressions (TE) that are included within other
    expressions. For example:

        a = "ABSOLUTO | em 28 de fevereiro de 2002"
        b = "ENUNCIADO | em 28 de fevereiro"

        ... TE "b" is within "a" , then "b" will be deleted.

    """
    i = 0
    while i != len(ETs):
        j = i + 1
        while j != len(ETs):
            a = ETs[i][0]
            b = ETs[j][0]
            if a in b:
                del ETs[i]
                j -= 1
            elif b in a:
                del ETs[j]
                j -= 1
            j += 1
        i += 1
    return ETs


def get_temporal_expression(s):
    """
    Get all temporal expressions (TEs) present in the sentence using "regular
    expressions" (regex). TEs are divided in 5 categories according to Batista
    et al. (2008): "ABSOLUTO", "DURACAO", "ENUNCIADO", "HORA" and "INTERVALO".

    """
    s = s.lower()
    # "ABSOLUTO" category
    # aa = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(de|em|até|desde|para)\s\d{4}', s)]
    # bb = [(s[i.start():i.end()].strip(), i.start(), i.end()) for i in re.finditer('(em|até|desde)?(\s\d{1,2}\s(de))?\s(' + M + ')\s(de)\s\d{4}', s)]
    # "DURACAO" category
    # c_exp = '((((' + C + ')\se\s)?(' + N + '))|(\d+))'
    # cc = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('((cerca|mais|menos)\s)?((de|há|durante|em|por)\s)?' + c_exp + '\s(' + T + ')(s|es)?(\se\s' + c_exp + '\s(' + T + ')(s|es))?', s)]
    # "ENUNCIADO" category
    dd = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('((' + D + ')(s)?(-feira(s)?)|sábado(s)?|domingo(s)?)', s)]
    ee = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(na\s(' + AT + ')\s)?(' + PP + ')\s(' + D + ')(s)?(-feira(s)?)?(\,?\s?\(?\d+\)?)?', s)]
    # ff = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('((até(\sa)?|(a\spartir\s)?de)\s)?(' + AT + ')', s)]
    # TODO "\w" da seguinte exressão não identifica "próximo"
    gg = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(' + PP + ')\s((\w|' + S + ')+\s)?((' + T + ')|(' + AT + '))(s|es)?', s)]
    # hh = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(no\sdia|até|em|(a\spartir\s)?de|desde|('+ PP +'))(\s\d{1,2}\s(de))?\s(' + M + ')(\s(de)\s\d{4})?', s) if s[i.start() - 4:i.start()] not in ['rio ']]
    # "HORA" category
    # ii = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(desde\sas|por\svolta\sdas|até|até\sàs|às|aos)\s((\d{1,2}(h|\shoras)(\d{1,2}(m(in)?)?)?)|(\d{1,2}(m(in)?|\sminutos)))', s)]
    # jj = [(s[i.start():i.end()].strip(), i.start(), i.end()) for i in re.finditer('\s\d{1,2}:\d{1,2}(\s(AM|PM))?', s)]
    # "INTERVALO" category
    # kk = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(de|entre|desde)\s(' + M + ')\s(a|e|até)\s(' + M + ')(\sde\s\d{4})?', s)]
    # ll = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(de|entre|entre\sos\sdias|desde)\s(\d{1,2})\s(a|e|até)\s(\d{1,2})(\sde\s(' + M + '))', s)]
    # mm = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(entre)\s(as)\s(\d{1,2}(h(\d{1,2}(m)?)?)?)\s(e)\s(as)\s(\d{1,2}(h(\d{1,2}(m)?)?)?)', s)]
    # nn = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(entre\s)?\d{4}(-|\s(a|e)\s)\d{4}', s)]
    # "REFERENCIAL" category
    # oo = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('\d{1,2}(/|\.)\d{1,2}(/|\.)(\d{4}|\d{1,2})', s)]
    # pp = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('\d{4}-\d{1,2}-\d{1,2}', s)]
    # "FREQUENCIA" category
    # qq = [(s[i.start():i.end()], i.start(), i.end()) for i in re.finditer('(' + FE + ')', s)]
    # All temporal expressions (TEs)
    # TEs = list(set(aa + bb + cc + dd + ee + ff + gg + hh + ii + jj + kk + ll + mm + nn + oo + pp + qq))
    TEs = list(set(dd + ee + gg))
    # Return filter TEs
    return filter_temporal_expressions(TEs)


def insert_temporal_expressions(s):
    tag1, tag2 = ' <aspect SRL="WHEN">', '</aspect> '
    a, b, tam = 0, 0, len(tag1) + len(tag2)
    expressions = sorted(get_temporal_expression(s), key=lambda tup: tup[1])
    for it in expressions:
        s = s[:it[1] + a] + tag1 + it[0] + tag2 + s[it[2] + b:]
        a += tam
        b += tam
    # Return the new sentence
    return s
