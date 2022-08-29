#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

import subprocess
import os

dir_name = os.path.dirname(os.path.realpath(__file__))


def check_page_contains_bullet_point(page, text):
    proc = subprocess.run(
        ["grep", "-Ri", "\\*" + text + ".*\\*", page], capture_output=True
    )
    if proc.returncode != 0:
        raise BaseException('Page %s does not contain bullet point "%s"' % (page, text))


def check_page_contains_text(page, text):
    proc = subprocess.run(["grep", "-Ri", text, page], capture_output=True)
    if proc.returncode != 0:
        raise BaseException('Page %s does not contain text "%s"' % (page, text))


def check_page_contains_headline(page, headline):
    proc = subprocess.run(
        ["grep", "-Ri", "^[=]\\+ " + headline + "$", page], capture_output=True
    )
    if proc.returncode != 0:
        raise BaseException('Page %s does not contain headline "%s"' % (page, headline))


def check_page_spelling(page):
    proc = subprocess.run([dir_name + "/spell-check", page], capture_output=True)
    if proc.returncode != 0 or proc.stderr:
        raise BaseException(
            "Page %s has misspelled words: %s" % (page, str(proc.stderr))
        )


def check_page_includes_part(page, part):
    proc = subprocess.run(
        ["grep", "-Ri", "include::.*" + part, page], capture_output=True
    )
    if proc.returncode != 0:
        raise BaseException('Page %s does not include partial "%s"' % (page, part))
