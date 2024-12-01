# ================================ Part 1 ================================

add_executable(aoc2024_01_1
        part1.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)

target_link_dependency(aoc2024_01_1 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_01_1 PUBLIC ../../utils)

target_set_output_dir(aoc2024_01_1 01_1)

# ================================ Part 2 ================================

add_executable(aoc2024_01_2
        part2.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)

target_link_dependency(aoc2024_01_2 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_01_2 PUBLIC ../../utils)

target_set_output_dir(aoc2024_01_2 01_2)
