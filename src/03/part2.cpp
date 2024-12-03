#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	std::regex instruction_pattern("(do\\(\\)|don't\\(\\)|mul\\(\\d{1,3},\\d{1,3}\\))");
	std::regex mult_pattern("mul\\((\\d+),(\\d+)\\)");
	auto instructions = find_all_regex(input, instruction_pattern);

	bool enabled = true;
	for (const auto& instruction : instructions) {
		if (instruction == "don't()") enabled = false;
		else if (instruction == "do()") enabled = true;

		else if (enabled) {
			const auto [a, b] = extract_data<int, int>(instruction, mult_pattern);
			result += a * b;
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2024, 3);
	runner.add_test_file("t2.txt", 48);

	runner.add_input_file("i1.txt");

	runner.run();
}