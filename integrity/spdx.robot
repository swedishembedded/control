*** Settings ***
Library  OperatingSystem
Library  Process
Library  String

*** Variables ***
${M_CONTROL_DIR}  ${CURDIR}/../octave/control
${M_SYSID_DIR}  ${CURDIR}/../octave/systemid
${C_INCLUDE_DIR}  ${CURDIR}/../include/control
${SRC}  ${CURDIR}/../src/

*** Test Cases ***

All files have spdx license
	All *.m files
	All *.h files
	All *.c files
	All *.cpp files
	All *.yaml files
	All *.robot files
	All *.adoc files

*** Keywords ***

All ${wildcard} files
	@{files} =  Git List Files ${wildcard}
	FOR  ${FILE}  IN  @{FILES}
		${length}=    Get Length    ${FILE}
		Run Keyword If    ${length}   File ${FILE} has spdx identifier
	END

Git List Files ${wildcard}
	${r} =  Run Process  git  ls-files  ${wildcard}
	@{list}=  Split String  ${r.stdout}  \n
	[return]  @{list}

File ${FILE} has spdx identifier
	${r} =  Run Process  grep  SPDX  ${FILE}
	Should Contain  ${r.stdout}  SPDX-License-Identifier:  msg=SPDX identifier not found in ${FILE}
