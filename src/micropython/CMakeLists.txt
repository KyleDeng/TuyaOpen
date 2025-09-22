##
# @file CMakeLists.txt
# @brief MicroPython component for TuyaOpen
#/

if (CONFIG_ENABLE_MICROPYTHON STREQUAL "y")

# MODULE_PATH
set(MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# MODULE_NAME
get_filename_component(MODULE_NAME ${MODULE_PATH} NAME)

# LIB_SRCS
set(LIB_SRCS
    ${MODULE_PATH}/port/t5ai/main.c
    ${MODULE_PATH}/port/t5ai/mphalport.c
)

# Later we will add core sources here
# list(APPEND LIB_SRCS ${MODULE_PATH}/core/*.c)

# LIB_PUBLIC_INC
set(LIB_PUBLIC_INC ${MODULE_PATH}/include)

# LIB_PRIVATE_INC
set(LIB_PRIVATE_INC
    ${MODULE_PATH}/port/t5ai
    ${MODULE_PATH}/core
)

# Compile definitions
add_definitions(-DMICROPYTHON_ENABLE=1)
add_definitions(-DMICROPYTHON_T5AI=1)

########################################
# Target Configure
########################################
add_library(${MODULE_NAME})

target_sources(${MODULE_NAME}
    PRIVATE
        ${LIB_SRCS}
    )

target_include_directories(${MODULE_NAME}
    PRIVATE
        ${LIB_PRIVATE_INC}

    PUBLIC
        ${LIB_PUBLIC_INC}
    )


########################################
# Layer Configure
########################################
list(APPEND COMPONENT_LIBS ${MODULE_NAME})
set(COMPONENT_LIBS "${COMPONENT_LIBS}" PARENT_SCOPE)
list(APPEND COMPONENT_PUBINC ${LIB_PUBLIC_INC})
set(COMPONENT_PUBINC "${COMPONENT_PUBINC}" PARENT_SCOPE)

endif()
