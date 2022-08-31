# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

# Check top level directories

*** Settings ***
Library  OperatingSystem
Library  ${CURDIR}/DocChecker.py
Resource  ${CURDIR}/module.robot

*** Variables ***
${ROOT_DIR}  ${CURDIR}/../

*** Test Cases ***

Modules have corresponding robot scripts
	@{MODULES} =  List Directories In Directory  ${ROOT_DIR}/src/
	FOR  ${module}  IN   @{MODULES}
		File Should Exist  ${ROOT_DIR}/integrity/${module}.robot
		Check Page Contains Text  ${ROOT_DIR}/integrity/${module}.robot  Module structure check ${module}
	END

*** Keywords ***
