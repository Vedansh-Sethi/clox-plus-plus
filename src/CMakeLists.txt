set(SOURCES
    main.cpp
    token/token.cpp
    scanner/scanner.cpp
    utils/ast_printer/ast_printer.cpp
    error.cpp
    parser/parser.cpp
    interpreter/interpreter.cpp
    environment/environment.cpp
)


# Define the executable
add_executable(clox++ ${SOURCES})

# Include the current directory for header lookups
target_include_directories(clox++ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})