#include <utils.hpp>

int w, h;

int find_num_paths(const std::vector<std::string>& map, Vec2i start) {
	std::unordered_set<Vec2i> seen{};
	std::queue<Vec2i> q;
	q.push(start);

	int result = 0;

	const auto dirs = all_dirs();

	while (!q.empty()) {
		Vec2i n = q.front();
		q.pop();

		char c = map[n.y()][n.x()];

		if (seen.contains(n)) continue;

		seen.insert(n);
		if (c == '9') result++;

		for (const auto dir : dirs) {
			Vec2i new_n = n + dir;
			if (seen.contains(new_n)
				|| !inbounds(new_n.x(), new_n.y(), w, h)
				|| map[new_n.y()][new_n.x()] != c+1) continue;
			q.push(new_n);
		}
	}

	return result;
}

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");

	w = lines[0].size();
	h = lines.size();

	std::vector<Vec2i> zeros{};

    for (int y = 0; y < h; ++y) {
    	for (int x = 0; x < w; ++x) {
    		if (lines[y][x] == '0') {
    			zeros.emplace_back(x, y);
    		}
    	}
    }

	for (const auto& start : zeros) {
		result += find_num_paths(lines, start);
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 10);
    runner.add_test_file("t1.txt", 2);
    runner.add_test_file("t2.txt", 4);
    runner.add_test_file("t3.txt", 3);
    runner.add_test_file("t4.txt", 36);

    runner.add_input_file("i1.txt");

    runner.run();
}