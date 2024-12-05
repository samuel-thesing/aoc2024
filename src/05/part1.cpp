#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	auto [rules, orders] = split_once(input, "\n\n");

	std::map<int, std::vector<int>> rules_map;
	for (const auto& rule : split(rules, "\n")) {
		const auto [Rx, Ry] = extract_data<int, int>(rule, std::regex(R"((\d+)\|(\d+))"));
		auto it = rules_map.find(Ry);
		if (it == rules_map.end()) {
			rules_map[Ry] = std::vector<int>();
		}
		rules_map[Ry].push_back(Rx);
	}

    for (const auto& order : split(orders, "\n")) {
    	std::set<int> visited{};
    	auto nums = split<int>(order, ",");
    	std::set<int> all{nums.begin(), nums.end()};
    	for (const auto& num : nums) {
    		visited.insert(num);
    		auto it = rules_map.find(num);
    		if (it != rules_map.end()) {
    			for (const auto& rule : it->second) {
    				if (!visited.contains(rule) && all.contains(rule)) {
    					goto outer;
    				}
    			}
    		}
    	}

    	result += nums[nums.size() / 2];

    	outer:;
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

		if (nums == cpy) {
			result += cpy[cpy.size() / 2];
		}
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 5);
    runner.add_test_file("t1.txt", 143);

    runner.add_input_file("i1.txt");

    runner.run();

	auto runner2 = Runner<int>(solve2, 2024, 5);
	runner2.add_test_file("t1.txt", 143);
	runner2.add_input_file("i1.txt");
	runner2.run();
}