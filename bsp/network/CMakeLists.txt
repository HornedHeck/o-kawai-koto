set(
    NETWORK_FLAVOR
    "at"
    CACHE STRING "Network module used"
)
set_property(CACHE NETWORK_FLAVOR PROPERTY STRINGS "at")


file(GLOB NETWORK_SRC ${CMAKE_CURRENT_LIST_DIR}/${NETWORK_FLAVOR}/src/*.c)

set(NETWORK_INC ${CMAKE_CURRENT_LIST_DIR}/include)

set(NETWORK_PRIVATE_INC ${CMAKE_CURRENT_LIST_DIR}/${NETWORK_FLAVOR}/include)

add_library(NETWORK STATIC)

target_sources(NETWORK PUBLIC ${NETWORK_SRC})

target_include_directories(NETWORK PUBLIC ${NETWORK_INC} ${NETWORK_PRIVATE_INC})

include(${NETWORK_FLAVOR}/dependencies.cmake OPTIONAL)

target_compile_options(NETWORK PRIVATE ${COMPILE_OPTIONS})
target_compile_definitions(NETWORK PRIVATE ${COMPILE_DEFINITIONS})
target_link_options(NETWORK PRIVATE ${LINK_OPTIONS})

target_enable_lint(NETWORK)
