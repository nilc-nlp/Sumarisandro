#!/usr/bin/env python
#-*- encoding:utf-8 -*-

from __future__ import unicode_literals

from micro_classifier import MicroaspectClassifier
from macro_classifier import MacroaspectClassifier

from nltk.tokenize import RegexpTokenizer

import tempfile
import nltk
import os
import sys
import codecs
import re

COTIDIANO_RECALL = ['WHAT', 'WHERE', 'WHEN', 'WHO_AGENT', 'DECLARATION']

ESPORTES_RECALL = ['WHO_AGENT', 'WHAT', 'SCORE', 'CONSEQUENCE', 'SITUATION',
    'COMMENT', 'WHEN', 'WHERE']

MUNDO_RECALL = ['WHAT', 'WHERE', 'WHO_AFFECTED', 'WHEN', 'WHY', 'WHO_AGENT',
    'DECLARATION', 'HISTORY', 'PREDICTION', 'GOAL']

POLITICA_RECALL = ['WHO_AGENT','WHAT', 'WHO_AFFECTED', 'WHEN', 'WHY',
    'PREDICTION', 'DECLARATION', 'HISTORY']

WHATEVER_RECALL = ['WHAT', 'WHERE', 'WHO_AFFECTED', 'WHEN', 'WHY', 'WHO_AGENT',
    'CONSEQUENCE', 'DECLARATION', 'HISTORY', 'PREDICTION', 'GOAL']


def annotate(sentences, AI_model, AC_model, repentino_locals):
    # Initialize MicroaspectClassifier
    clf = MicroaspectClassifier(sentences, AI_model, AC_model, repentino_locals)
    micro_annotation = clf.annotated
    # Initialize MacroaspectClassifier
    clf = MacroaspectClassifier(sentences)
    macro_annotation = clf.annotated_sentences
    # Get all the annotated sentences
    annotated_sentences = []
    for i in range(len(micro_annotation)):
        sentence = micro_annotation[i].split(']')[0]
        A = micro_annotation[i].split(']')[1]
        B = macro_annotation[i].split(']')[1]
        aspects = A + '/' + B if B else A
        annotated_sentences.append(sentence + ']' + aspects)
    return annotated_sentences


def get_document_max_size(documents):
    """
    Function that returns the maximum number of words of the largest document.

    """
    tokenizer, sizes = RegexpTokenizer(r'\w+'), []
    for document in documents:
        f = codecs.open(documents, 'r', 'UTF-8')
        sizes.append(len(tokenizer.tokenize(fi.read())))
        f.close()
    return max(sizes)


def compression_ratio(summary, max_size, ratio=0.70):
    tokenizer, compressed_summary = RegexpTokenizer(r'\w+'), []
    # Define threshold
    threshold, total = round(max_size * (1.00 - ratio)), 0
    for sentence in summary:
        size = len(tokenizer.tokenize(sentence))
        total += size
        if total > threshold:
            if (size * (1 - ratio)) >= (total - threshold):
                compressed_summary.append(sentence)
            else:
                return compressed_summary
        else:
            compressed_summary.append(sentence)
    return compressed_summary


def get_sentence_weight(aspects, ctgy_aspects, pos, n_sentences, alpha=1.00):
    # Rsumm weight
    ranking = float((n_sentences - 1) - pos) / (n_sentences - 1)
    # Aspect weight
    recall = len(list(set(aspects) & set(ctgy_aspects)))
    recall = float(recall) / len(ctgy_aspects)
    # Final weight
    weight = (ranking * (1 - alpha)) + (recall * alpha)
    return weight


def get_summary(RSumm_sentences, AI_model, AC_model, repentino_locals, max_size):
    # Get annotated sentences
    annotated_sentences = annotate(
        RSumm_sentences,
        AI_model,
        AC_model,
        repentino_locals
    )
    data, ctgy_aspects = [], WHATEVER_RECALL
    # ----------------------------------------------------------
    #                  Sentence weighting
    # ----------------------------------------------------------
    for i, annotated_sentence in enumerate(annotated_sentences):
        # Get a weight (W) for each sentence 
        spl = annotated_sentence.split(']')
        sentence, aspects = spl[0][1:], spl[1].split('/')
        tam = len(annotated_sentences)
        W = get_sentence_weight(aspects, ctgy_aspects, i, tam)
        data.append((sentence, annotated_sentence, W))
    # ----------------------------------------------------------
    #       Get a compressed summary by "ASumm" system
    # ----------------------------------------------------------
    # Sort all sentences (data) by aspect recall
    sorted_data = sorted(data, key=lambda tup: tup[2], reverse=True)
    # Get all sentences (without aspects annotation)
    sentences = [S for S, A, R in sorted_data]
    A_sentences = [A for S, A, R in sorted_data]
    # Get compressed summary
    summary = compression_ratio(
        sentences,
        max_size
    )
    # Return summary
    return summary
    #return A_sentences
