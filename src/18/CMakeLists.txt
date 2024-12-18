# ================================ Part 1 ================================

add_executable(aoc2024_18_1
        part1.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)

target_link_dependency(aoc2024_18_1 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_18_1 PUBLIC ../../utils)

target_set_output_dir(aoc2024_18_1 18_1)

# ================================ Part 2 ================================

add_executable(aoc2024_18_2
        part2.cpp
        ../../utils/Logger.cpp
        ../../utils/utils.hpp
        ../../utils/matrix.hpp
)

target_link_dependency(aoc2024_18_2 HEADER spdlog-1.14.1)
target_include_directories(aoc2024_18_2 PUBLIC ../../utils)

target_set_output_dir(aoc2024_18_2 18_2)