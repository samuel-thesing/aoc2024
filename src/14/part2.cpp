#include <utils.hpp>

void printRobots(const std::vector<std::pair<Vec2i, Vec2i>> &robots, int w, int h) {
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			auto it = std::find_if(robots.begin(), robots.end(), [&](const auto& r) {
				return r.first == Vec2i{x, y};
			});
			if (it != robots.end()) {
				std::cout << '#';
			} else {
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int solve(const std::string &input, const int w, const int h) {
	std::regex pattern(R"(p=(\d+),(\d+) v=(-?\d+),(-?\d+))");

	auto lines = split(input, "\n");

	std::vector<std::pair<Vec2i, Vec2i>> robots{};

	for (const auto& line : lines) {
		const auto [px, py, vx, vy] = extract_data<int, int, int, int>(line, pattern);
		Vec2i pos = {px, py};
		Vec2i vel = {vx, vy};

		robots.emplace_back(pos, vel);
	}

	// a picture involves getting the robots closer to each other and fairly centered

	int step = 1;
	while (true) {
		double variance = 0;
		for (auto& robot : robots) {
			auto& [pos, vel] = robot;
			pos += vel;

			pos.x() = mod_math(pos.x(), w);
			pos.y() = mod_math(pos.y(), h);

			variance += std::pow((w/2 - pos.x()) / static_cast<double>(w), 2)
				+ std::pow((h/2 - pos.y()) / static_cast<double>(h), 2);
		}
		variance /= robots.size();
		// set after seeing the variance of reoccurring bulks that appeared about every 101 steps
		if (variance < 0.1) {
			printRobots(robots, w, h);
			std::cout << step << " " << variance << std::endl;
			// std::cin.get();
			return step;
		}

		step++;

	}
	Logger::critical("How did we get here?");
}

int main(int argc, char** argv) {
	auto runner = Runner<int, int, int>(solve, 2024, 14);

	runner.add_input_file("i1.txt", 101, 103);

	runner.run();
}