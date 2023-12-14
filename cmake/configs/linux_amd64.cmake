set(COMPILE_OPTIONS
    -fdata-sections
    -ffunction-sections
    -Wall
    -Wno-unused-command-line-argument
    $<$<CONFIG:Debug>:-Og>)

set(COMPILE_DEFINITIONS FIXED_POINT)

set(LINK_OPTIONS
    # -specs=nano.specs -lc -lm -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    # -Wl,--gc-sections
)