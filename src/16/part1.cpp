#include <utils.hpp>

// Dijkstra mit Position und Richtung

int solve(const std::string &input) {
	Vec2i start = {-1, -1};
	Vec2i end = {-1, -1};

	auto lines = split(input, "\n");
	const int h = lines.size();
	for (int y = 0; y < h; ++y) {
		auto si = lines[y].find('S');
		auto ei = lines[y].find('E');
		if (si != std::string::npos) {
			start = {si, y};
		}
		if (ei != std::string::npos) {
			end = {ei, y};
		}
	}

	typedef std::tuple<Vec2i, Vec2i, int> DataPoint;

	// pos, dir, cost
	std::priority_queue<DataPoint, std::vector<DataPoint>,
		std::function<bool(DataPoint, DataPoint)>> q(
			[](const DataPoint& a, const DataPoint& b) {
				return std::get<2>(a) > std::get<2>(b);
	});
	q.push({start, {1, 0}, 0});

	std::unordered_set<std::tuple<Vec2i, Vec2i>> seen;

	while (!q.empty()) {
		DataPoint p = q.top();
		q.pop();

		Vec2i pos = std::get<0>(p);
		Vec2i dir = std::get<1>(p);
		int cost = std::get<2>(p);

		if (seen.contains({pos, dir})) continue;
		if (pos == end) {
			return cost;
		}
		seen.emplace(pos, dir);

		Vec2i new_pos = pos + dir;
		if (lines[new_pos.y()][new_pos.x()] != '#' && !seen.contains({new_pos, dir})) {
			q.emplace(new_pos, dir, cost+1);
		}

		// 90° drehen
		dir = {-dir.y(), dir.x()};
		if (!seen.contains({pos, dir})) {
			q.emplace(pos, dir, cost+1000);
		}

		// 180° drehen
		dir = {-dir.x(), -dir.y()};
		if (!seen.contains({pos, dir})) {
			q.emplace(pos, dir, cost+1000);
		}
	}

	return -1;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 16);
    runner.add_test_file("t1.txt", 7036);
    runner.add_test_file("t2.txt", 11048);

    runner.add_input_file("i1.txt");

    runner.run();
}