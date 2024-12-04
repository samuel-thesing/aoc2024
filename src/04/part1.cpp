#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");
	const std::string pattern = "XMAS";

	const auto dirs = all_dirs_diag();

	for (int y = 0; y < lines.size(); y++) {
		for (int x = 0; x < lines[y].size(); x++) {

			for (const auto& dir : dirs) {
				const auto dx = dir.x(), dy = dir.y();
				if (x + dx * (pattern.size()-1) < 0 || x + dx * (pattern.size()-1) >= lines[0].size()
					|| y + dy * (pattern.size()-1) < 0 || y + dy * (pattern.size()-1) >= lines.size()) {
					continue;
				}

				for (int i = 0; i < pattern.size(); i++) {
					if (lines[y + dy * i][x + dx * i] != pattern[i]) {
						goto outer;
					}
				}

				result++;

				outer:;
			}
		}
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<int>(solve, 2024, 4);
    runner.add_test_file("t1.txt", 18);

    runner.add_input_file("i1.txt");

    runner.run();
}