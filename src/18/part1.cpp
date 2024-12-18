#include <utils.hpp>

int solve(const std::string &input, int size, int num) {
	auto lines = split(input, "\n");

	std::unordered_set<Vec2i> blocked{};

    for (int i = 0; i < num; i++) {
    	const auto& line = lines[i];
    	const auto [x, y] = extract_data<int, int>(line, std::regex("(\\d+),(\\d+)"));
    	blocked.emplace(x, y);
    }

	const auto dirs = all_dirs();
	std::unordered_set<Vec2i> seen{};
	std::deque<std::tuple<Vec2i, int>> q;
	q.emplace_back(Vec2i{0, 0}, 0);
	while (!q.empty()) {
		auto [n, step] = q.front();
		q.pop_front();

		if (seen.contains(n)) continue;
		seen.emplace(n);
		if (n == Vec2i{size, size}) {
			return step;
		}

		for (const auto& dir : dirs) {
			auto new_pos = n + dir;
			if (seen.contains(new_pos)
				|| blocked.contains(new_pos)
				|| !inbounds(new_pos.x(), new_pos.y(), size+1, size+1)) continue;
			q.emplace_back(new_pos, step+1);
		}
	}

	return -1;
}

int main(int argc, char** argv) {
    auto runner = Runner<int, int, int>(solve, 2024, 18);
    runner.add_test_file("t1.txt", 22, 6, 12);

    runner.add_input_file("i1.txt", 70, 1024);

    runner.run();
}