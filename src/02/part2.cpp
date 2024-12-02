#include <utils.hpp>

int sign(int n) {
	return n > 0 ? 1 : -1;
}

bool check(std::vector<int> nums) {
	int increasing = sign(nums[1] - nums[0]);
	for (auto diff : diffs(nums)) {
		if (sign(diff) != increasing || std::abs(diff) < 1 || std::abs(diff) > 3) {
			return false;
		}
	}
	return true;
}

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");
	for (const auto& line : lines) {
		auto nums = split<int>(line, " ");

		if (check(nums)) {
			result++;
			continue;
		}

		for (int i = 0; i < nums.size(); i++) {
			auto cpy = nums;
			cpy.erase(cpy.begin() + i);
			if (check(cpy)) {
				result++;
				break;
			}
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2024, 2);
	runner.add_test_file("t1.txt", 4);

	runner.add_input_file("i1.txt");

	runner.run();
}