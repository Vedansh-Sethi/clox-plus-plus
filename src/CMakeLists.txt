# Define the list of source files
# As you add Scanner.cpp, Parser.cpp, etc., add them here.
set(SOURCES
    main.cpp
    token/token.cpp
    token/token.hpp
    token/token_type.hpp
    scanner/scanner.cpp
    scanner/scanner.hpp
    expr.hpp
    error.hpp
)

# Define the executable
add_executable(clox++ ${SOURCES})

# Include the current directory for header lookups
target_include_directories(clox++ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)