#include <utils.hpp>

int correct_ordering(const std::map<int, std::set<int>>& rules_map, std::vector<int> nums) {
	std::set<int> all_nums{nums.begin(), nums.end()};
	std::vector<int> ordered{};
	std::set<int> seen{};
	while (ordered.size() < all_nums.size()) {
		for (const auto& num : all_nums) {
			if (seen.contains(num)) continue;
			auto rule = rules_map.find(num);
			if (rule != rules_map.end()) {
				for (const auto before : rule->second) {
					if (all_nums.contains(before) && !seen.contains(before)) {
						goto outer;
					}
				}
			}
			ordered.emplace_back(num);
			seen.emplace(num);

			outer:;
		}
	}

	if (ordered == nums) return 0;
	return ordered[ordered.size() / 2];
}

int solve(const std::string &input) {
	int result = 0;

	auto [rules, orders] = split_once(input, "\n\n");

	std::map<int, std::set<int>> rules_map;
	for (const auto& rule : split(rules, "\n")) {
		const auto [Rx, Ry] = extract_data<int, int>(rule, std::regex(R"((\d+)\|(\d+))"));
		auto it = rules_map.find(Ry);
		if (it == rules_map.end()) {
			rules_map[Ry] = std::set<int>();
		}
		rules_map[Ry].emplace(Rx);
	}

	for (const auto& order : split(orders, "\n")) {
		auto nums = split<int>(order, ",");
		result += correct_ordering(rules_map, nums);
	}

	return result;
}

int solve2(const std::string& input) {
	int result = 0;

	auto [rules, orders] = split_once(input, "\n\n");

	std::map<int, std::set<int>> rules_map;
	for (const auto& rule : split(rules, "\n")) {
		const auto [Rx, Ry] = extract_data<int, int>(rule, std::regex(R"((\d+)\|(\d+))"));
		auto it = rules_map.find(Ry);
		if (it == rules_map.end()) {
			rules_map[Ry] = std::set<int>();
		}
		rules_map[Ry].emplace(Rx);
	}

	for (const auto& order : split(orders, "\n")) {
		auto nums = split<int>(order, ",");
		auto cpy = nums;
		std::sort(cpy.begin(), cpy.end(), [&](int a, int b) {
			if (a == b) [[unlikely]] return false;
			return rules_map[b].contains(a);
		});

		if (nums != cpy) {
			result += cpy[cpy.size() / 2];
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2024, 5);
	runner.add_test_file("t1.txt", 123);
	runner.add_test_file("t2.txt", 0);

	runner.add_input_file("i1.txt");

	runner.run();

	auto runner2 = Runner<int>(solve2, 2024, 5);
	runner2.add_test_file("t1.txt", 123);
	runner.add_test_file("t2.txt", 0);
	runner2.add_input_file("i1.txt");
	runner2.run();
}