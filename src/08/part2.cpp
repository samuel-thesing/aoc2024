#include <utils.hpp>

int solve(const std::string &input) {
	auto lines = split(input, "\n");

	const int w = lines[0].size();
	const int h = lines.size();

	std::map<char, std::vector<Vec2i>> antennas{};
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			const auto c = lines[y][x];
			if (c == '.') continue;

			auto it = antennas.find(c);
			if (it == antennas.end()) {
				antennas.emplace(c, std::vector<Vec2i>{});
			}
			antennas[c].emplace_back(x, y);
		}
	}
	std::unordered_set<Vec2i> syms{};

	for (const auto& poss: antennas | std::views::values) {
		for (int i = 0; i < poss.size()-1; ++i) {
			for (int j = i+1; j < poss.size(); ++j) {
				const auto diff = poss[j] - poss[i];
				auto mpos = poss[i];
				while (inbounds(mpos.x(), mpos.y(), w, h)) {
					syms.insert(mpos);
					mpos -= diff;
				}

				mpos = poss[j];
				while (inbounds(mpos.x(), mpos.y(), w, h)) {
					syms.insert(mpos);
					mpos += diff;
				}
			}
		}
	}

	return syms.size();
}

int main(int argc, char** argv) {
	auto runner = Runner<int>(solve, 2024, 8);
	runner.add_test_file("t1.txt", 34);

	runner.add_input_file("i1.txt");

	runner.run();
}