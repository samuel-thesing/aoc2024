
add_executable(aoc2024_01_1
        part1.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)

target_link_dependency(aoc2024_01_1 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_01_1 PUBLIC ../../utils)


add_executable(aoc2024_01_2
        part2.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)


target_link_dependency(aoc2024_01_2 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_01_2 PUBLIC ../../utils)
