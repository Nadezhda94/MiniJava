cmake_minimum_required(VERSION 3.5)
project(Compilers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Compilers_SOURCE_DIR}/code )

# Create target for the parser
ADD_CUSTOM_TARGET(FlexBison ALL echo "Creating flex/bison cpp")

# Create custom command for flex/lex
ADD_CUSTOM_COMMAND(
        TARGET FlexBison
        PRE_BUILD
        COMMAND flex
        ARGS -o ${Compilers_SOURCE_DIR}/code/lex.yy.cpp
        ${Compilers_SOURCE_DIR}/code/simplejava.l)

# Create custom command for bison/yacc
ADD_CUSTOM_COMMAND(
        TARGET FlexBison
        PRE_BUILD
        COMMAND bison
        ARGS -o ${Compilers_SOURCE_DIR}/code/simplejava.tab.cpp
        -d
        --report all
        --report-file ${Compilers_SOURCE_DIR}/code/"report.txt"
        ${Compilers_SOURCE_DIR}/code/simplejava.y
        DEPENDS ${Compilers_SOURCE_DIR}/code/lex.yy.cpp)


set(SOURCE_FILES
    code/simplejava.tab.cpp
    code/lex.yy.cpp
    code/ASTVisitors/Printer.cpp
    code/ASTVisitors/Translator.cpp
    code/ASTVisitors/SymbolTableBuilder.cpp
    code/ASTVisitors/TypeChecker.cpp
    code/Structs/IRTree.cpp
    code/Structs/BasicBlocks.cpp
    code/Structs/Frame.cpp
    code/Structs/Symbol.cpp
    code/Structs/SymbolsTable.cpp
    code/Structs/Temp.cpp
    code/Structs/TraceShedule.cpp
    code/Structs/Ast.cpp
    code/IRVisitors/Canonizer.cpp
    code/IRVisitors/Printer.cpp
    code/IRVisitors/Optimizer.cpp
        code/Structs/TempMap.cpp
        code/Structs/Temp.cpp
        code/Structs/Codegen.cpp
        code/Structs/Assembler.cpp
    code/main.cpp code/Structs/FlowGraph.cpp code/Structs/InterferenceGraph.cpp code/Structs/InterferenceGraph.h code/IRVisitors/RegAlloc.cpp code/IRVisitors/RegAlloc.h code/IRVisitors/CodeGenerator.cpp)

SET_SOURCE_FILES_PROPERTIES(${Compilers_SOURCE_DIR}/code/simplejava.tab.cpp GENERATED)
SET_SOURCE_FILES_PROPERTIES(${Compilers_SOURCE_DIR}/code/lex.yy.cpp GENERATED)

add_executable(Compilers ${SOURCE_FILES})
add_dependencies(Compilers FlexBison)

ADD_CUSTOM_COMMAND(
        TARGET Compilers
        POST_BUILD
        COMMAND rm
        ${Compilers_SOURCE_DIR}/code/lex.yy.cpp ${Compilers_SOURCE_DIR}/code/simplejava.tab.cpp ${Compilers_SOURCE_DIR}/code/simplejava.tab.hpp)

