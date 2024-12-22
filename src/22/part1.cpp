#include <utils.hpp>

uint64_t solve(const std::string &input, int steps) {
	uint64_t result = 0;

	auto nums = split<uint64_t>(input, "\n");

    for (const auto& num : nums) {
    	auto secret = num;
    	for (int i = 0; i < steps; i++) {
    		auto sec64 = secret << 6;
    		secret ^= secret << 6;
    		secret %= 16777216;
    		secret ^= secret >> 5;
    		secret %= 16777216;
    		secret ^= secret << 11;
    		secret %= 16777216;
    	}
    	result += secret;
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<uint64_t, int>(solve, 2024, 22);
    runner.add_test_file("t1.txt", 15887950, 1);
    runner.add_test_file("t1.txt", 16495136, 2);
    runner.add_test_file("t1.txt", 527345, 3);
    runner.add_test_file("t1.txt", 704524, 4);
    runner.add_test_file("t1.txt", 1553684, 5);
    runner.add_test_file("t1.txt", 12683156, 6);
    runner.add_test_file("t1.txt", 11100544, 7);
    runner.add_test_file("t1.txt", 12249484, 8);
    runner.add_test_file("t1.txt", 7753432, 9);
    runner.add_test_file("t1.txt", 5908254, 10);

	runner.add_test_file("t2.txt", 37327623, 2000);

    runner.add_input_file("i1.txt", 2000);

    runner.run();
}