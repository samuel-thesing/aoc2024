# ================================ Part 1 ================================

add_executable(aoc2024_03_1
        part1.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)

target_link_dependency(aoc2024_03_1 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_03_1 PUBLIC ../../utils)

target_set_output_dir(aoc2024_03_1 03_1)

# ================================ Part 2 ================================

add_executable(aoc2024_03_2
        part2.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)

target_link_dependency(aoc2024_03_2 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_03_2 PUBLIC ../../utils)

target_set_output_dir(aoc2024_03_2 03_2)