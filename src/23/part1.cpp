#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");

	std::unordered_map<std::string, std::unordered_set<std::string>> map;

    for (const auto& line : lines) {
    	const auto [a, b] = split_once<std::string, std::string>(line, "-");

    	auto it1 = map.find(a);
    	if (it1 == map.end()) {
    		map.emplace(a, std::unordered_set<std::string>());
    	}
    	map[a].emplace(b);

    	auto it2 = map.find(b);
    	if (it2 == map.end()) {
    		map.emplace(b, std::unordered_set<std::string>());
    	}
    	map[b].emplace(a);
    }

	std::unordered_set<std::array<std::string, 3>> groups;

	for (const auto [com1, coms] : map) {
		for (const auto com2 : coms) {
			for (const auto com3 : map[com2]) {
				if (com1 == com3) continue;
				if (coms.contains(com3)) {
					std::array<std::string, 3> group{com1, com2, com3};
					std::ranges::sort(group);
					groups.emplace(group);
				}
			}
		}
	}


	std::unordered_set<std::array<std::string, 3>> tgroups;
	for (const auto& group : groups) {
		if (group[0][0] == 't' || group[1][0] == 't' || group[2][0] == 't') {
			tgroups.emplace(group);
			result++;
		}
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 23);
    runner.add_test_file("t1.txt", 7);

    runner.add_input_file("i1.txt");

    runner.run();
}