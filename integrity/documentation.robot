# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

*** Settings ***
Library  OperatingSystem

Library  ${CURDIR}/DocChecker.py

*** Variables ***
${ROOT_DIR}  ${CURDIR}/../

*** Test Cases ***

Linear algebra functions have been documented
	@{FILES} =  List Files In Directory  ${ROOT_DIR}/src/linalg  *.c
	FOR  ${FILE}  IN   @{FILES}
		${PATH}  ${NAME}  Split Path  ${FILE}
		${SECTION}  ${EXT}  Split Extension  ${NAME}
		File Should Exist  ${ROOT_DIR}/doc/linalg/${SECTION}.adoc
		File Should Exist  ${ROOT_DIR}/tests/linalg/${SECTION}.cpp
	END

*** Keywords ***
