#include <utils.hpp>

std::unordered_map<std::string, uint64_t> cache;

uint64_t match(const std::vector<std::string>& patterns, const std::string& target) {
	if (target.empty()) return 1;

	auto it = cache.find(target);
	if (it != cache.end()) return it->second;

	uint64_t count = 0;
	for (const auto& pattern : patterns) {
		if (pattern.size() > target.size()) continue;
		for (int i = 0; i < pattern.size(); ++i) {
			if (pattern[i] != target[i]) goto outer;
		}

		count += match(patterns, target.substr(pattern.size()));

		outer:;
	}

	cache.emplace(target, count);
	return count;
}

uint64_t solve(const std::string &input) {
	uint64_t result = 0;

	cache.clear();

	auto [patterns, targets] = split_once(input, "\n\n");
	auto pattern_vec = split(patterns, ", ");
	auto target_vec = split(targets, "\n");

	for (int i = 0; i < target_vec.size(); ++i) {
		result += match(pattern_vec, target_vec[i]);
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<uint64_t>(solve, 2024, 19);
	runner.add_test_file("t1.txt", 16);

	runner.add_input_file("i1.txt");

	runner.run();
}