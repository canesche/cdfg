add_library(hlsmc MODULE
    dataflowPass.cpp
    plugin.cpp
)

target_include_directories(hlsmc PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)
