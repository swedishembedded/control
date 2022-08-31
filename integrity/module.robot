# SPDX-License-Identifier: Apache-2.0
# Copyright 2022 Martin Schröder <info@swedishembedded.com>
# Consulting: https://swedishembedded.com/go
# Training: https://swedishembedded.com/tag/training

*** Settings ***
Library  OperatingSystem
Library  ${CURDIR}/DocChecker.py

*** Variables ***
${ROOT_DIR}  ${CURDIR}/../

*** Keywords ***

Module structure check ${module}
	File Should Exist  ${ROOT_DIR}/include/control/${module}.h
	File Should Exist  ${ROOT_DIR}/tests/${module}/CMakeLists.txt
	Check Page Contains Text  ${ROOT_DIR}/tests/CMakeLists.txt  add_subdirectory(${module})
	@{FILES} =  List Files In Directory  ${ROOT_DIR}/src/${module}  *.c
	FOR  ${FILE}  IN   @{FILES}
		${PATH}  ${NAME}  Split Path  ${FILE}
		${SECTION}  ${EXT}  Split Extension  ${NAME}
		File Should Exist  ${ROOT_DIR}/doc/${module}/${SECTION}.adoc
		# Check that page is included in index
		Check Page Contains Text  ${ROOT_DIR}/doc/${module}/index.adoc  ${SECTION}.adoc
		# Check that test exists and that it is included in the build cmake file
		File Should Exist  ${ROOT_DIR}/tests/${module}/${SECTION}.cpp
		Check Page Contains Text  ${ROOT_DIR}/tests/${module}/CMakeLists.txt  ${SECTION}.cpp
		# Check that file is in zephyr build as well
		Check Page Contains Text  ${ROOT_DIR}/zephyr/CMakeLists.txt  ../src/${module}/${SECTION}.c
		# Check that main doc index contains module reference
		Check Page Contains Text  ${ROOT_DIR}/doc/index.adoc  ${module}/index.adoc
	END
