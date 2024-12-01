#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");
	auto lines2 = split_int(input, "\n");

	std::vector<int> vec1{};
	std::vector<int> vec2{};
    for (const auto& line : lines) {
    	const auto [a, b] = extract_data<int, int>(std::regex("(\\d+) *(\\d+)"), line);
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
    auto runner = Runner<int>(solve, 2024, 01);
    runner.add_test_file("t1.txt", 11);

    runner.add_input_file("i1.txt");

    runner.run();
}