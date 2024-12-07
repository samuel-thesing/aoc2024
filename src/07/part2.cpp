#include <utils.hpp>

long long concat_num(long long a, long long b) {
	long long tmp = b;
	do {
		a *= 10;
	} while ((tmp /= 10) > 0);

	return a + b;
}

bool recurse(const long long target, std::vector<long long>& nums, const int idx, const long long current) {
	if (idx >= nums.size())
		return target == current;

	return recurse(target, nums, idx + 1, current + nums[idx])
		|| recurse(target, nums, idx + 1, current * nums[idx])
		|| recurse(target, nums, idx + 1, concat_num(current, nums[idx]));
}

long long solve(const std::string &input) {
	long long result = 0;

	auto lines = split(input, "\n");

	for (const auto& line : lines) {
		const auto [target, num_str] = split_once<long long, std::string>(line, ": ");
		auto nums = split<long long>(num_str, " ");

		if (recurse(target, nums, 1, nums[0])) {
			result += target;
		}
	}

	return result;
}

int main(int argc, char** argv) {
	std::cout << concat_num(42, 1399) << std::endl;

	auto runner = Runner<long long>(solve, 2024, 7);
	runner.add_test_file("t1.txt", 11387);

	runner.add_input_file("i1.txt");

	runner.run();
}