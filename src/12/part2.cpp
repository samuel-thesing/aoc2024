#include <utils.hpp>

const auto dirs = all_dirs();

int w, h;

// calculate area/collect positions
int plot_garden(const std::vector<std::string>& map, std::unordered_set<Vec2i>& garden, Vec2i start) {
	int area = 1;
	garden.emplace(start);
	for (const auto& dir : dirs) {
		auto n = start + dir;

		if (garden.contains(n)
			|| !inbounds(n.x(), n.y(), w, h)
			|| map[n.y()][n.x()] != map[start.y()][start.x()]) {
			continue;
		}

		area += plot_garden(map, garden, n);
	}

	return area;
}

int count_sides(const std::vector<std::string>& map, std::unordered_set<Vec2i>& garden) {
	int perim = 0;
	// for each cardinal direction
	for (const auto& side : dirs) {
		std::unordered_set<Vec2i> seen{}; // reset after each direction
		auto ortho = Vec2i{side.y(), -side.x()};

		for (const auto& part : garden) {
			// if is a part with fence
			if (seen.contains(part) || garden.contains(part+side)) continue;

			perim++;

			// walk along fence and mark
			auto pos = part;
			do {
				seen.emplace(pos);
				pos += ortho;
			} while (inbounds(pos.x(), pos.y(), w, h) && garden.contains(pos) && !garden.contains(pos+side));

			pos = part;
			do {
				seen.emplace(pos);
				pos -= ortho;
			} while (inbounds(pos.x(), pos.y(), w, h) && garden.contains(pos) && !garden.contains(pos+side));
		}
	}

	return perim;
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

			std::unordered_set<Vec2i> garden;
			auto area = plot_garden(lines, garden, Vec2i{x, y});
			int sides = count_sides(lines, garden);

			seen.insert_range(garden);

			result += sides * area;
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<long long>(solve, 2024, 12);
	runner.add_test_file("t1.txt", 80);
	runner.add_test_file("t2.txt", 436);
	runner.add_test_file("t4.txt", 236);
	runner.add_test_file("t5.txt", 368);
	runner.add_test_file("t3.txt", 1206);

	runner.add_input_file("i1.txt");

	runner.run();
}