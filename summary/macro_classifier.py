#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from rules.handcrafted_rules import apply_annotation_rules

import numpy


class MacroaspectClassifier():
    def __init__(self, sentences):
        self.sentences = sentences
        self.annotated_sentences = self.annotate_sentences()

    def annotate_sentences(self):
        try: 
            return apply_annotation_rules(self.sentences)
        except:
            return ['[' + item + ']' for item in self.sentences]
