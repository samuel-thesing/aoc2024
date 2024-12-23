#include <utils.hpp>

std::unordered_map<std::string, std::unordered_set<std::string>> map;
std::unordered_set<std::vector<std::string>> groups;

std::unordered_set<std::unordered_set<std::string>> seen;

void find_groups(std::string cur_com, std::unordered_set<std::string> cur_group) {
	cur_group.emplace(cur_com);
	if (seen.contains(cur_group)) return;
	seen.emplace(cur_group);
	bool changed = false;
	for (const auto& com : map[cur_com]) {
		if (cur_group.contains(com)) continue;
		if (!std::ranges::all_of(cur_group,
			[&com](const auto& gc) { return map[gc].contains(com); })) continue;

		changed = true;
		find_groups(com, cur_group);
	}

	if (!changed) {
		std::vector<std::string> group{cur_group.begin(), cur_group.end()};
		std::ranges::sort(group);
		groups.emplace(group);
	}
}

std::string solve(const std::string &input) {
	auto lines = split(input, "\n");

	map.clear();
	groups.clear();

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

		map[a].emplace(a);
		map[b].emplace(b);
	}

	for (const auto com : map | std::views::keys) {
		find_groups(com, std::unordered_set<std::string>());
	}

	auto m = std::ranges::max_element(groups, {}, [](const std::vector<std::string>& g) {return g.size();});

	std::string result = "";
	for (const auto& com : *m) {
		if (com != *m->begin()) {
			result += ",";
		}
		result += com;
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<std::string>(solve, 2024, 23);
	runner.add_test_file("t1.txt", "co,de,ka,ta");

	runner.add_input_file("i1.txt");

	runner.run();
}