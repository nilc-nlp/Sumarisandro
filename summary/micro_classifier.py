#!/usr/bin/env python
#-*- encoding:utf-8 -*-

import re
import sys
sys.path.append('src')

from utils.functions import annotate_instance
from utils.run_palavras import run_palavras_sentence_by_web_service, add_semantic_tags

from rules.WHEN_rules import (get_temporal_expression,
                              insert_temporal_expressions)
from rules.WHERE_rules import reforming_WHERE_sentence
from rules.WHY_rules import (get_cause_expression,
                             insert_cause_expressions)
from rules.SCORE_rules import get_score_expression

from corpus.util.CoNLLFormatter import main
from bin.srl.main import classify

microaspects = ['WHO_AGENT', 'WHO_AFFECTED', 'WHEN', 'WHERE', 'HOW', 'WHY']


class MicroaspectClassifier():
    def __init__(self, sentences, AI_model, AC_model, locals_dct, labels=microaspects):
        self.labels = labels
        self.argident_sys, self.argclass_sys = AI_model, AC_model
        self.SRL_sentences, self.annotated = self.get_sentences(
            sentences, locals_dct
        )

    def get_sentences(self, sentences, locals_dct, rules=True):
        SRL_sentences, annotated_sentences = [], []
        # Load 'identification' and 'classification' models
        for sentence in sentences:
            sentence = re.sub('[\"|\[|\]]', '', sentence).strip()
            # Run PALAVRAS TigerXML (tree format)
            # Get a list of words of the sentence
            words, semantic_tags = add_semantic_tags(
                run_palavras_sentence_by_web_service(sentence)
            )
            SRL_sentence, annotated_sentence = sentence, '[' + sentence + ']'
            # If exists a list of words (or a syntactic tree has nonterminals
            # and has verbs)
            if words:
                try:
                    # Transform TigerXML to CoNLL format
                    main()
                    # Semantic role labelling (SRL)
                    classify(self.argident_sys, self.argclass_sys)
                    # Get sentence annotated with SRL-microaspects
                    SRL_sentence, annotated_sentence = annotate_instance(
                        sentence,
                        words[0],
                        semantic_tags[0],
                        self.labels,
                        locals_dct,
                    )
                except:
                    pass
            # If exists rules condition
            if rules:
                SRL_sentence, annotated_sentence = self.apply_rules_to_sentence(
                    SRL_sentence,
                    annotated_sentence,
                )
            # Append sentences
            SRL_sentences.append(SRL_sentence)
            annotated_sentences.append(annotated_sentence)
        # Return all sentences anotated with microaspects
        return SRL_sentences, annotated_sentences

    def apply_rules_to_sentence(self, SRL_sentence, annotated_sentence):
        # Apply WHEN rules
        if not re.search('<aspect SRL="WHEN">(.+?)</aspect>', SRL_sentence):
            if get_temporal_expression(SRL_sentence):
                SRL_sentence = insert_temporal_expressions(SRL_sentence)
                annotated_sentence += 'WHEN/'
        # Apply WHERE rules
        SRL_sentence, annotated_sentence = reforming_WHERE_sentence(
            SRL_sentence,
            annotated_sentence
        )
        # Apply WHY rules
        if not re.search('<aspect SRL="WHY">(.+?)</aspect>', SRL_sentence):
            if get_cause_expression(SRL_sentence):
                SRL_sentence = insert_cause_expressions(SRL_sentence)
                annotated_sentence += 'WHY/'
        # Apply SCORE rules
        if get_score_expression(annotated_sentence):
            annotated_sentence += 'SCORE/'
        # Delete last '/' 
        if annotated_sentence[-1] == '/':
            annotated_sentence = annotated_sentence[:-1]
        return SRL_sentence, annotated_sentence
