
function(generate_resources)
    set(OPTIONS)
    set(ONE_VALUE_ARGS OUTPUT PREFIX)
    set(MULTI_VALUE_ARGS FILES)

    cmake_parse_arguments(RESOURCE "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})

    get_filename_component(RESOURCE_OUTPUT_DIR ${RESOURCE_OUTPUT} DIRECTORY)

    string(APPEND FILE_CONTENT "<RCC>")
    string(APPEND FILE_CONTENT "<qresource prefix=\"/${RESOURCE_PREFIX}\">")

    foreach(RESOURCE_FILE ${RESOURCE_FILES})
        file(RELATIVE_PATH RESOURCE_FILE_PATH ${RESOURCE_OUTPUT_DIR} ${RESOURCE_FILE})
        string(APPEND FILE_CONTENT "<file>${RESOURCE_FILE_PATH}</file>")
    endforeach()

    string(APPEND FILE_CONTENT "</qresource>")
    string(APPEND FILE_CONTENT "</RCC>")

    file(WRITE ${RESOURCE_OUTPUT} ${FILE_CONTENT})
endfunction()
