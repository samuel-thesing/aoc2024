
add_executable(aocREPLACE_YEAR_REPLACE_DAY_1
        part1.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.h
        ../../utils/matrix.h
)

target_link_dependency(aocREPLACE_YEAR_REPLACE_DAY_1 HEADER spdlog-1.14.1)
target_include_directories(aocREPLACE_YEAR_REPLACE_DAY_1 PUBLIC ../../utils)


add_executable(aocREPLACE_YEAR_REPLACE_DAY_2
        part2.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.h
        ../../utils/matrix.h
)


target_link_dependency(aocREPLACE_YEAR_REPLACE_DAY_2 HEADER spdlog-1.14.1)
target_include_directories(aocREPLACE_YEAR_REPLACE_DAY_2 PUBLIC ../../utils)
