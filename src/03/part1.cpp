#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	std::regex pattern("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    for (const auto& [a, b] : extract_data_all<int, int>(input, pattern)) {
    	result += a * b;
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 3);
	runner.add_test_string("mul(44,46)", 2024);
	runner.add_test_string("mul(123,4)", 492);
	runner.add_test_string("mul(4*", 0);
	runner.add_test_string("mul(6,9!", 0);
	runner.add_test_string("?(12,34)", 0);
	runner.add_test_string("mul ( 2 , 4 )", 0);
    runner.add_test_file("t1.txt", 161);

    runner.add_input_file("i1.txt");

    runner.run();
}