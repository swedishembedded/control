find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable")

if(NOT CLANG_TIDY_EXE)
  message(FATAL_ERROR "clang-tidy not found")
endif()

function(clang_tidy_check TARGET)
  # Only check header files from the same current source dir
  get_filename_component(SOURCE_DIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
  set(DO_CLANG_TIDY
      "${CLANG_TIDY_EXE}" "-extra-arg=-ferror-limit=0"
      "-p=${CMAKE_CURRENT_BUILD_DIR}/" "--header-filter=${SOURCE_DIR_NAME}/.*")

  set_target_properties(${TARGET} PROPERTIES C_CLANG_TIDY "${DO_CLANG_TIDY}")
  set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endfunction()
