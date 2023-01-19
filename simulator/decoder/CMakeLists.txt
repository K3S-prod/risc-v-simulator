set(DECODER_BINARY_DIR ${RVSIM_BINARY_DIR}/simulator/decoder)
set(DECODER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/simulator/decoder)
set(ISA "${CMAKE_SOURCE_DIR}/isa/isa.yaml")

set(TEMPLATES_GENERATOR "${CMAKE_SOURCE_DIR}/isa/isa_parser.rb")
set(TEMPLATES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/templates")
set(GENERATED_DIR "${DECODER_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(TEMPLATES_NAMES
    "decoder.inl"
    "dispatch_labels_enum.inl"
    "signatures_definition.inl"
)

list(TRANSFORM TEMPLATES_NAMES PREPEND "${TEMPLATES_DIR}/" OUTPUT_VARIABLE TEMPLATES)
list(TRANSFORM TEMPLATES APPEND ".erb")
list(TRANSFORM TEMPLATES_NAMES PREPEND "${GENERATED_DIR}/" OUTPUT_VARIABLE TEMPLATES_INSTANTIATED)

add_custom_command(
    OUTPUT ${TEMPLATES_INSTANTIATED}
    DEPENDS ${TEMPLATES_GENERATOR} ${ISA} ${TEMPLATES} 
    COMMAND ruby ${TEMPLATES_GENERATOR} ${ISA} ${TEMPLATES} ${TEMPLATES_INSTANTIATED}
)

add_custom_target(decoder_gen DEPENDS ${TEMPLATES_INSTANTIATED})
add_library(decoder decoder.cpp)
add_dependencies(decoder decoder_gen)
