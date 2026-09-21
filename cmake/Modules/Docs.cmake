option(BUILD_DOCS "Enable the 'docs' target (requires Doxygen)" OFF)

if(BUILD_DOCS)
    find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_EXCLUDE_PATTERNS */build*/* */vcpkg*/* */_deps/*)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)

    if(TARGET Doxygen::dot)
        set(DOXYGEN_HAVE_DOT YES)
        set(DOXYGEN_CALL_GRAPH YES)
    endif()

    doxygen_add_docs(docs
        ${CMAKE_SOURCE_DIR}/README.md
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/OpenGL
        ${CMAKE_SOURCE_DIR}/examples
        COMMENT "Generating API documentation")
endif()