#include <utils.hpp>

bool recurse(const long long target, std::vector<long long>& nums, const int idx, const long long current) {
	if (idx >= nums.size())
		return target == current;

	return recurse(target, nums, idx + 1, current + nums[idx])
		|| recurse(target, nums, idx + 1, current * nums[idx]);
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
    auto runner = Runner<long long>(solve, 2024, 7);
    runner.add_test_file("t1.txt", 3749);

    runner.add_input_file("i1.txt");

    runner.run();
}