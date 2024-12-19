#include <utils.hpp>

bool match(const std::vector<std::string>& patterns, const std::string& target, int idx = 0) {
	if (idx >= target.size()) return true;
	for (const auto& pattern : patterns) {
		if (idx + pattern.size() > target.size()) continue;
		for (int i = 0; i < pattern.size(); ++i) {
			if (pattern[i] != target[idx + i]) goto outer;
		}

		if (match(patterns, target, idx + pattern.size())) {
			return true;
		}

		outer:;
	}
	return false;
}

int solve(const std::string &input) {
	int result = 0;

	auto [patterns, targets] = split_once(input, "\n\n");
	auto pattern_vec = split(patterns, ", ");
	auto target_vec = split(targets, "\n");

	for (int i = 0; i < target_vec.size(); ++i) {
		if (match(pattern_vec, target_vec[i])) {
			result++;
		}
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 19);
    runner.add_test_file("t1.txt", 6);

    runner.add_input_file("i1.txt");

    runner.run();
}