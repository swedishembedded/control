*** Settings ***
Library  OperatingSystem
Library  Process

*** Variables ***
${M_CONTROL_DIR}  ${CURDIR}/../octave/control
${M_SYSID_DIR}  ${CURDIR}/../octave/systemid
${C_INCLUDE_DIR}  ${CURDIR}/../include/control
${SRC}  ${CURDIR}/../src/

*** Test Cases ***

All files have spdx license
	All *.m files in ${M_CONTROL_DIR}
	All *.m files in ${M_SYSID_DIR}
	All *.h files in ${C_INCLUDE_DIR}
	All *.c files in ${SRC}/ai
	All *.c files in ${SRC}/controller
	All *.c files in ${SRC}/filter
	All *.c files in ${SRC}/linalg
	All *.c files in ${SRC}/misc
	All *.c files in ${SRC}/optimization
	All *.c files in ${SRC}/sysid

*** Keywords ***

All ${wildcard} files in ${dir}
	@{files} =  List Files In Directory  ${dir}  ${wildcard}  absolute=True
	FOR  ${FILE}  IN  @{FILES}
		File ${FILE} has spdx identifier
	END

File ${FILE} has spdx identifier
	${r} =  Run Process  grep  SPDX  ${FILE}
	Should Contain  ${r.stdout}  SPDX-License-Identifier:  msg=SPDX identifier not found in ${FILE}
