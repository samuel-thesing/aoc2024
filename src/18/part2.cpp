#include <utils.hpp>

bool solvable(const std::unordered_set<Vec2i>& blocked, const int size) {
	const auto dirs = all_dirs();
	std::unordered_set<Vec2i> seen{};
	std::deque<Vec2i> q;
	q.emplace_back(0, 0);
	while (!q.empty()) {
		auto n = q.front();
		q.pop_front();

		if (seen.contains(n)) continue;
		seen.emplace(n);
		if (n == Vec2i{size, size}) {
			return true;
		}

		for (const auto& dir : dirs) {
			auto new_pos = n + dir;
			if (seen.contains(new_pos)
				|| blocked.contains(new_pos)
				|| !inbounds(new_pos.x(), new_pos.y(), size+1, size+1)) continue;
			q.emplace_back(new_pos);
		}
	}

	return false;
}

Vec2i solve(const std::string &input, int size, int num) {
	auto lines = split(input, "\n");

	std::unordered_set<Vec2i> blocked{};

	for (int i = 0; i < num; i++) {
		const auto& line = lines[i];
		const auto [x, y] = extract_data<int, int>(line, std::regex("(\\d+),(\\d+)"));
		blocked.emplace(x, y);
	}

	for (int i = num; i < lines.size(); i++) {
		const auto& line = lines[i];
		const auto [x, y] = extract_data<int, int>(line, std::regex("(\\d+),(\\d+)"));
		blocked.emplace(x, y);
		if (!solvable(blocked, size)) {
			return {x, y};
		}
	}

	return {-1, -1};
}

int main(int argc, char** argv) {
	auto runner = Runner<Vec2i, int, int>(solve, 2024, 18);
	runner.add_test_file("t1.txt", Vec2i{6,1}, 6, 12);

	runner.add_input_file("i1.txt", 70, 1024);

	runner.run();
}