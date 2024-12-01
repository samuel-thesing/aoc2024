#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");

	auto pattern = std::regex(" +");

	std::vector<int> vec1{}, vec2{};
    for (const auto& line : lines) {
    	const auto [a, b] = split_once_regex<int, int>(line, pattern);
    	vec1.push_back(a);
    	vec2.push_back(b);
    }
	std::ranges::sort(vec1);
	std::ranges::sort(vec2);

	for (int i = 0; i < vec1.size(); i++) {
		result += abs(vec1[i] - vec2[i]);
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 1);
    runner.add_test_file("t1.txt", 11);

    runner.add_input_file("i1.txt");

    runner.run();
}