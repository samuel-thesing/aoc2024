#include <utils.hpp>

int solve(const std::string &input) {
	int result = 0;

	auto lines = split(input, "\n");
	std::vector<std::string> pattern = {
		"M.S",
		".A.",
		"M.S"
	};

	for (int y = 0; y < lines.size() - pattern[0].size()+1; y++) {
		for (int x = 0; x < lines[y].size() - pattern[0].size()+1; x++) {

			for (int rot = 0; rot < 4; rot++) {
				for (int i = 0; i < pattern.size(); i++) {
					for (int j = 0; j < pattern[0].size(); j++) {
						if (pattern[i][j] == '.') continue;
						if (lines[y+i][x+j] != pattern[i][j])
							goto outer;
					}
				}

				result++;
				break;
				outer:;

				pattern = rotate90c(pattern);
			}
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2024, 4);
	runner.add_test_file("t1.txt", 9);

	runner.add_input_file("i1.txt");

	runner.run();
}