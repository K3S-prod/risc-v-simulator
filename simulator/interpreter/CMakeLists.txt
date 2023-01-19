set(INTERPRETER_BINARY_DIR ${RVSIM_BINARY_DIR}/simulator/interpreter)
set(INTERPRETER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/simulator/interpreter)
set(ISA "${CMAKE_SOURCE_DIR}/isa/isa.yaml")

set(TEMPLATES_GENERATOR "${CMAKE_SOURCE_DIR}/isa/isa_parser.rb")
set(TEMPLATES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/templates")
set(GENERATED_DIR "${INTERPRETER_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(TEMPLATES_NAMES
    "dispatch_table.inl"
)

list(TRANSFORM TEMPLATES_NAMES PREPEND "${TEMPLATES_DIR}/" OUTPUT_VARIABLE TEMPLATES)
list(TRANSFORM TEMPLATES APPEND ".erb")
list(TRANSFORM TEMPLATES_NAMES PREPEND "${GENERATED_DIR}/" OUTPUT_VARIABLE TEMPLATES_INSTANTIATED)

add_custom_command(
    OUTPUT ${TEMPLATES_INSTANTIATED}
    DEPENDS ${TEMPLATES_GENERATOR} ${ISA} ${TEMPLATES} 
    COMMAND ruby ${TEMPLATES_GENERATOR} ${ISA} ${TEMPLATES} ${TEMPLATES_INSTANTIATED}
)

add_custom_target(interpreter_gen DEPENDS ${TEMPLATES_INSTANTIATED})

add_library(interpreter interpreter.cpp)
target_link_libraries(interpreter PUBLIC decoder)
add_dependencies(interpreter interpreter_gen)

