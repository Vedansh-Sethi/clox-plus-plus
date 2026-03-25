set(SOURCES
    main.cpp
    token/token.cpp
    scanner/scanner.cpp
    error.cpp
    parser/parser.cpp
    interpreter/interpreter.cpp
    environment/environment.cpp
    function/native_function/nativeFunction.cpp
    function/function.cpp
    resolver/resolver.cpp
    class/class.cpp
    instance/instance.cpp
    function/getter/getter.cpp
    function/setter/setter.cpp
)


# Define the executable
add_executable(clox++ ${SOURCES})

# Include the current directory for header lookups
target_include_directories(clox++ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})