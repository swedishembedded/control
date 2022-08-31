# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

*** Settings ***
Library  OperatingSystem
Library  ${CURDIR}/DocChecker.py
Resource  ${CURDIR}/module.robot

*** Variables ***
${ROOT_DIR}  ${CURDIR}/../

*** Test Cases ***

Module structure is correct
	Module structure check misc

*** Keywords ***
