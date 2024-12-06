#include <utils.hpp>

bool isLoop(const std::vector<std::string>& lines, Vec2i pos, Vec2i dir) {
	const int w = lines[0].size();
	const int h = lines.size();
	std::unordered_set<std::tuple<Vec2i, Vec2i>> visited{};

	while (inbounds(pos.x(), pos.y(), w, h)) {
		if (visited.contains({pos, dir})) return true;
		visited.emplace(pos, dir);

		Vec2i newPos = pos + dir;
		if (!inbounds(newPos.x(), newPos.y(), w, h)) break;
		while (inbounds(newPos.x(), newPos.y(), w, h) && lines[newPos.y()][newPos.x()] == '#') {
			dir = {-dir.y(), dir.x()};
			newPos = pos + dir;

			// see t2.txt
			if (visited.contains({pos, dir})) return true;
		}
		pos = newPos;
	}

	return false;
}

int solve(const std::string &input) {
	int result = 0;

    std::regex pattern("(\\d+)");

	auto lines = split(input, "\n");

	Vec2i pos = Vec2i(0, 0);
	Vec2i dir = Vec2i(0, 0);

    for (int i = 0; i < lines.size(); i++) {
    	auto idx = lines[i].find_first_of("^<>v");
    	if (idx == std::string::npos) continue;

    	pos = {idx, i};
    	dir = arrow_dir(lines[i][idx]);
    	break;
    }

	auto startPos = pos;
	auto startDir = dir;

	int w = lines[0].size();
	int h = lines.size();

	std::unordered_set<Vec2i> visited{};
	while (inbounds(pos.x(), pos.y(), w, h)) {
		Vec2i newPos = pos + dir;
		visited.emplace(pos);
		while (inbounds(newPos.x(), newPos.y(), w, h) && lines[newPos.y()][newPos.x()] == '#') {
			dir = {-dir.y(), dir.x()};
			newPos = pos + dir;
		}
		pos = newPos;
	}

	visited.erase(startPos);
	for (const Vec2i& block : visited) {
		lines[block.y()][block.x()] = '#';
		if (isLoop(lines, startPos, startDir)) {
			result++;
		}
		lines[block.y()][block.x()] = '.';
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 6);
    runner.add_test_file("t1.txt", 6);
	runner.add_test_file("t2.txt", 1);

    runner.add_input_file("i1.txt");

    runner.run();
}
