#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	auto [map_s, moves] = split_once(input, "\n\n");
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
		if (c == '\n') continue;
		auto dir = arrow_dir(c);
		auto new_pos = robot + dir;
		auto mapc = map[new_pos.y()][new_pos.x()];
		if (mapc == '#') continue;
		if (mapc == '.') {
			robot = new_pos;
			continue;
		}

		while (mapc == 'O') {
			new_pos += dir;
			mapc = map[new_pos.y()][new_pos.x()];
		}
		if (mapc == '.') {
			map[new_pos.y()][new_pos.x()] = 'O';
			robot += dir;
			map[robot.y()][robot.x()] = '.';
		} else { // # after O snake
			continue;
		}
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (map[y][x] == 'O') {
				result += 100 * y + x;
			}
		}
	}


	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 15);
    runner.add_test_file("t2.txt", 2028);
    runner.add_test_file("t1.txt", 10092);

    runner.add_input_file("i1.txt");

    runner.run();
}