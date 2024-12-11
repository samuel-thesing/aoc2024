#include <utils.hpp>

long long solve(const std::string &input, const int blinks) {
	const auto lines = split<long long>(input, " ");

	std::unordered_map<long long, long long> counts{};
	for (auto n : lines) {
		counts[n]++;
	}

	for (int b = 0; b < blinks; b++) {
		std::unordered_map<long long, long long> counts_next{};
		for (const auto [num, count] : counts) {
			if (num == 0) {
				counts_next[1] += count;
				continue;
			}
			int len = num_len(num);
			if (len % 2 == 0) {
				const long long cutoff = static_cast<long long>(std::pow(10, len / 2));
				counts_next[num % cutoff] += count;
				counts_next[num / cutoff] += count;
				continue;
			}

			counts_next[num * 2024] += count;
		}
		counts = counts_next;
	}

	long long result = 0;
	for (const auto count: counts | std::views::values) {
		result += count;
	}

	return result;
}

int main(int argc, char** argv) {
	std::cout << std::log10(42) << std::endl;
	auto runner = Runner<long long, int>(solve, 2024, 11);
	runner.add_test_file("t1.txt", 3, 1);
	runner.add_test_file("t1.txt", 4, 2);
	runner.add_test_file("t1.txt", 5, 3);
	runner.add_test_file("t1.txt", 9, 4);
	runner.add_test_file("t1.txt", 13, 5);
	runner.add_test_file("t1.txt", 22, 6);
	runner.add_test_file("t1.txt", 55312, 25);

	runner.add_input_file("i1.txt", 75);

	runner.run();
}