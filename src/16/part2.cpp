#include <utils.hpp>

// Dijkstra mit angepasster Abbruch-Bedingung:
// Weitergehen bis Kosten vom ersten Mal Ziel erreichen überstiegen wurden,
// da das Ziel theoretisch mit denselben Kosten von unterschiedlichen Richtungen erreicht werden kann.
// Bei der Suche die parents speichern und
// danach von den Lösungen aus traversieren und nur die Positionen zählen.

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

	std::unordered_map<DataPoint, std::vector<DataPoint>> parents;

	int endCost = std::numeric_limits<int>::max();

	std::vector<DataPoint> solutions;

	while (!q.empty()) {
		DataPoint p = q.top();
		q.pop();

		Vec2i pos = std::get<0>(p);
		Vec2i dir = std::get<1>(p);
		int cost = std::get<2>(p);

		if (cost > endCost) break;

		if (seen.contains({pos, dir})) continue;
		seen.emplace(pos, dir);
		if (pos == end) {
			endCost = cost;
			solutions.emplace_back(p);
			continue;
		}

		Vec2i new_pos = pos + dir;
		if (lines[new_pos.y()][new_pos.x()] != '#' && !seen.contains({new_pos, dir})) {
			DataPoint pn{new_pos, dir, cost+1};
			auto it = parents.find(pn);
			if (it == parents.end()) {
				parents.emplace(pn, std::vector<DataPoint>());
			}
			parents[pn].emplace_back(p);
			q.push(std::move(pn));
		}

		dir = {-dir.y(), dir.x()};
		if (!seen.contains({pos, dir})) {
			DataPoint pn{pos, dir, cost+1000};
			auto it = parents.find(pn);
			if (it == parents.end()) {
				parents.emplace(pn, std::vector<DataPoint>());
			}
			parents[pn].emplace_back(p);
			q.push(std::move(pn));
		}

		dir = {-dir.x(), -dir.y()};
		if (!seen.contains({pos, dir})) {
			DataPoint pn{pos, dir, cost+1000};
			auto it = parents.find(pn);
			if (it == parents.end()) {
				parents.emplace(pn, std::vector<DataPoint>());
			}
			parents[pn].emplace_back(p);
			q.push(std::move(pn));
		}
	}

	std::unordered_set<Vec2i> tiles{};

	std::queue<DataPoint> q2;
	q2.push_range(solutions);
	while (!q2.empty()) {
		DataPoint p = q2.front();
		q2.pop();

		Vec2i pos = std::get<0>(p);
		Vec2i dir = std::get<1>(p);

		// Hier hatte ich das insert erst nach der Abbruchbedingung stehen,
		// was bei den Beispielen funktioniert hat ...
		tiles.insert(pos);
		if (pos == start && dir == Vec2i{1, 0}) continue;

		for (const auto& dp : parents[p]) {
			q2.push(dp);
		}
	}

	return tiles.size();
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 16);
    runner.add_test_file("t1.txt", 45);
    runner.add_test_file("t2.txt", 64);

    runner.add_input_file("i1.txt");

    runner.run();

	// 457
}