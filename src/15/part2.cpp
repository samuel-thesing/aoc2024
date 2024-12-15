#include <utils.hpp>

// had to debug the small example because i made 2 small mistakes
void print_map(const std::vector<std::string>& map, Vec2i robot) {
	for (int y = 0; y < map.size(); ++y) {
		if (y == robot.y()) {
			for (int x = 0; x < map[0].size(); ++x) {
				if (x == robot.x()) {
					std::cout << '@';
				} else {
					std::cout << map[y][x];
				}
			}
			std::cout << '\n';
		} else {
			std::cout << map[y] << "\n";
		}
	}
	std::cout << std::endl;
}

int solve(const std::string &input) {
	int result = 0;

	auto [map_s, moves] = split_once(input, "\n\n");

	// lucky me for writing this function beforehand
	map_s = replace_all(map_s, "#", "##");
	map_s = replace_all(map_s, "O", "[]");
	map_s = replace_all(map_s, ".", "..");
	map_s = replace_all(map_s, "@", "@.");

	auto map = split(map_s, "\n");
	const int w = map[0].size();
	const int h = map.size();
	Vec2i robot{-1, -1};
	for (int y = 0; y < h; y++) {
		int idx = map[y].find_first_of("@");
		if (idx != std::string::npos) {
			map[y][idx] = '.';
			robot = {idx, y};
			break;
		}
	}

	if (robot == Vec2i{-1, -1}) {
		Logger::critical("Failed to find robot.");
	}

	for (const auto& c : moves) {
		// print_map(map, robot);
		if (c == '\n') continue;
		auto dir = arrow_dir(c);
		auto new_pos = robot + dir;
		auto mapc = map[new_pos.y()][new_pos.x()];
		if (mapc == '#') continue;
		if (mapc == '.') {
			robot = new_pos;
			continue;
		}

		if (dir.y() == 0) { // horizontal is still a simple check
			while (mapc == '[' || mapc == ']') {
				new_pos += dir;
				mapc = map[new_pos.y()][new_pos.x()];
			}
			if (mapc != '.') {
				continue;
			}

			// could be overwritten systematically instead
			for (auto p = new_pos; p != robot; p -= dir) {
				map[p.y()][p.x()] = map[p.y()][p.x() - dir.x()];
			}
			robot += dir;

			continue;
		}

		// dir.x() == 0 from here on

		// this part would be cleaner with 2 recursive functions. one for testing and one for pushing

		std::vector<Vec2i> all_checks; // all box-parts that would be pushed
		std::unordered_set<Vec2i> check; // all box-parts on the current line
		std::unordered_set<Vec2i> check_next;
		check.emplace(new_pos);
		if (mapc == ']') {
			check.emplace(new_pos - Vec2i{1, 0});
		} else {
			check.emplace(new_pos + Vec2i{1, 0});
		}

		while (!check.empty()) {
			for (auto& ch : check) {
				auto p = ch + dir;
				switch (map[p.y()][p.x()]) {
					case '.': continue;
					case '#': goto outer; // whole tree fails
					case '[': {
						check_next.emplace(p);
						check_next.emplace(p + Vec2i{1, 0});
					} break;
					case ']': {
						check_next.emplace(p);
						check_next.emplace(p - Vec2i{1, 0});
					} break;
				}
			}
			all_checks.insert(all_checks.end(), check.begin(), check.end());
			check = std::move(check_next);
			check_next = std::unordered_set<Vec2i>();

			mapc = map[new_pos.y()][new_pos.x()];
		}

		// iterate in reverse over all box-parts to not overwrite the later ones
		for (const auto& ch : all_checks | std::views::reverse) {
			auto p = ch + dir;
			map[p.y()][p.x()] = map[ch.y()][ch.x()];
			map[ch.y()][ch.x()] = '.';
		}
		robot = new_pos;

		outer:;
	}

	// print_map(map, robot);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (map[y][x] == '[') {
				result += 100 * y + x;
			}
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2024, 15);
	runner.add_test_file("t3.txt", 618);
	runner.add_test_file("t1.txt", 9021);

	runner.add_input_file("i1.txt");

	runner.run();
}