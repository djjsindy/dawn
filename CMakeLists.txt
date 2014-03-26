PROJECT(DAWN)
aux_source_directory(./src SRC_LIST)
include_directories(./include)
add_executable(./bin/dawn ${SRC_LIST})