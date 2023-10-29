add_subdirectory(opus) 

target_compile_options(opus PRIVATE ${COMPILE_OPTIONS})
target_link_options(opus PRIVATE ${LINK_OPTIONS})