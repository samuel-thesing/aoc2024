#include <utils.hpp>

int solve(const std::string &input) {
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

	const int w = lines[0].size();
	const int h = lines.size();

	std::unordered_set<Vec2i> visited{};
	while (inbounds(pos.x(), pos.y(), w, h)) {
		visited.emplace(pos);
		Vec2i newPos = pos + dir;
		while (inbounds(newPos.x(), newPos.y(), w, h) && lines[newPos.y()][newPos.x()] == '#') {
			dir = {-dir.y(), dir.x()};
			newPos = pos + dir;
		}
		pos = newPos;
	}

	return visited.size();
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2024, 6);
	runner.add_test_file("t1.txt", 41);
	runner.add_test_file("t2.txt", 2);

	runner.add_input_file("i1.txt");

	runner.run();
}