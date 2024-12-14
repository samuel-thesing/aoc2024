#include <utils.hpp>

int solve(const std::string &input, const int w, const int h, const int steps) {
    std::regex pattern(R"(p=(\d+),(\d+) v=(-?\d+),(-?\d+))");

	auto lines = split(input, "\n");

	std::vector<Vec2i> robots{};

    for (const auto& line : lines) {
    	const auto [px, py, vx, vy] = extract_data<int, int, int, int>(line, pattern);
		Vec2i pos = {px, py};
		Vec2i vel = {vx, vy};

		pos += vel * steps;

    	pos.x() = mod_math(pos.x(), w);
		pos.y() = mod_math(pos.y(), h);

		robots.emplace_back(pos);
    }

	int qw = w / 2;
	int qh = h / 2;
	std::array<int, 4> quads{0};
	for (const auto& robot : robots) {
		std::strong_ordering lr = robot.x() <=> qw;
		std::strong_ordering tb = robot.y() <=> qh;

		if (lr == std::strong_ordering::less) {
			if (tb == std::strong_ordering::less) {
				quads[0]++;
			} else if (tb == std::strong_ordering::greater) {
				quads[1]++;
			}
		} else if (lr == std::strong_ordering::greater) {
			if (tb == std::strong_ordering::less) {
				quads[2]++;
			} else if (tb == std::strong_ordering::greater) {
				quads[3]++;
			}
		}
	}


	return quads[0] * quads[1] * quads[2] * quads[3];
}

int main(int argc, char** argv) {
    auto runner = Runner<int, int, int, int>(solve, 2024, 14);
    runner.add_test_file("t1.txt", 12, 11, 7, 100);

    runner.add_input_file("i1.txt", 101, 103, 100);

    runner.run();
}