#include <utils.hpp>

int solve(const std::string &input, const int min_cheat) {
	int result = 0;

    std::regex pattern("(\\d+)");

	auto lines = split(input, "\n");

	const int w = lines[0].size();
	const int h = lines.size();

	Vec2i start = Vec2i(-1, -1);
	Vec2i end = Vec2i(-1, -1);

    for (int y = 0; y < h; ++y) {
		auto sidx = lines[y].find("S");
    	if (sidx != std::string::npos) {
    		start = {sidx, y};
    	}
    	auto eidx = lines[y].find("E");
    	if (eidx != std::string::npos) {
    		end = {eidx, y};
    	}
    }
	if (start == Vec2i(-1, -1) || end == Vec2i(-1, -1)) {
		Logger::critical("Failed to find start or end");
	}

	const auto dirs = all_dirs();

	std::unordered_map<Vec2i, int> dists;
	int curDist = 1;
	dists.emplace(start, 0);

	std::unordered_set<Vec2i> seen;

	auto cur = start;
	seen.emplace(start);
	while (cur != end) {
		for (const auto &dir : dirs) {
			auto new_pos = cur + dir;
			if (lines[new_pos.y()][new_pos.x()] == '#'
				|| seen.contains(new_pos)) continue;

			cur = new_pos;
			goto outer;
		}
		Logger::critical("No path found from ({}, {})", cur.x(), cur.y());

		outer:;
		seen.emplace(cur);
		dists.emplace(cur, curDist);
		curDist++;
	}

	for (const auto& pos : seen) {
		auto it1 = dists.find(pos);
		if (it1 == dists.end()) {
			Logger::critical("Non path found in seen");
		}
		for (const auto &dir : dirs) {
			auto new_pos = pos + 2 * dir;
			auto it2 = dists.find(new_pos);
			if (it2 == dists.end()) continue;
			if (it2->second - it1->second - 2  >= min_cheat) {
				result++;
			}
		}
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int, int>(solve, 2024, 20);
    runner.add_test_file("t1.txt", 1, 64);
    runner.add_test_file("t1.txt", 2, 40);
    runner.add_test_file("t1.txt", 30, 4);

    runner.add_input_file("i1.txt", 100);

    runner.run();
}