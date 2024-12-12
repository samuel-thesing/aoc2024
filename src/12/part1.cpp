#include <utils.hpp>

const auto dirs = all_dirs();

int w, h;

// perim, area
std::pair<int, int> plot_garden(const std::vector<std::string>& map, std::unordered_set<Vec2i>& seen, Vec2i prev) {
	int perim = 0;
	int area = 1;
	seen.emplace(prev);
	for (const auto& dir : dirs) {
		auto n = prev + dir;

		if (!inbounds(n.x(), n.y(), w, h)
			|| map[n.y()][n.x()] != map[prev.y()][prev.x()]) {
			perim++;
			continue;
		}

		if (seen.contains(n)) continue;

		auto r = plot_garden(map, seen, n);
		perim += r.first;
		area += r.second;
	}

	return {perim, area};
}

long long solve(const std::string &input) {
	long long result = 0;

	const auto lines = split(input, "\n");

	std::unordered_set<Vec2i> seen{};

	w = lines[0].size();
	h = lines.size();



    for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			if (seen.contains(Vec2i{x, y})) continue;

			auto [perim, area] = plot_garden(lines, seen, Vec2i{x, y});
			result += perim * area;
		}
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2024, 12);
    runner.add_test_file("t1.txt", 140);
    runner.add_test_file("t2.txt", 772);
    runner.add_test_file("t3.txt", 1930);

    runner.add_input_file("i1.txt");

    runner.run();
}