#include <utils.hpp>

struct Pad {
	std::string name;
	Vec2i forbidden = Vec2i{-1, -1};
	std::unordered_map<char, Vec2i> mapping{};
};

uint64_t find_min(const std::vector<Pad>& pads, const std::string target, int cur_pad = 0) {
	if (cur_pad >= pads.size()) {
		return target.size();
	}

	const auto& pad = pads[cur_pad];
	auto cur_pos = pad.mapping.at('A');
	uint64_t cost = 0;
	for (const char c : target) {
		uint64_t min_cost = std::numeric_limits<uint64_t>::max();
		const auto target_pos = pad.mapping.at(c);
		const auto delta = target_pos - cur_pos;

		const char dxc = delta.x() < 0 ? '<' : '>';
		const char dyc = delta.y() < 0 ? '^' : 'v';

		const auto dxs = std::string(std::abs(delta.x()), dxc);
		const auto dys = std::string(std::abs(delta.y()), dyc);

		if (cur_pos + Vec2i{delta.x(), 0} != pad.forbidden) {
			min_cost = min(min_cost, find_min(pads, dxs + dys + 'A', cur_pad + 1));
		}
		if (cur_pos + Vec2i{0, delta.y()} != pad.forbidden) {
			min_cost = min(min_cost, find_min(pads, dys + dxs + 'A', cur_pad + 1));
		}

		if (min_cost == std::numeric_limits<uint64_t>::max()) {
			Logger::critical("Two forbidden found?!");
		}

		cost += min_cost;
		cur_pos = target_pos;
	}

	return cost;
}

uint64_t solve(const std::string &input, int num_dpads) {
	uint64_t result = 0;

	const std::vector<std::string> numpad_s = {
		"789",
		"456",
		"123",
		".0A"
	};

	const std::vector<std::string> dirpad_s = {
		".^A",
		"<v>"
	};

	Pad numpad{"Numpad"};
	for (int y = 0; y < numpad_s.size(); y++) {
		for (int x = 0; x < numpad_s[y].size(); x++) {
			if (numpad_s[y][x] == '.') {
				numpad.forbidden = Vec2i{x, y};
				continue;
			}
			numpad.mapping.emplace(numpad_s[y][x], Vec2i{x, y});
		}
	}

	Pad dirpad{"Dirpad"};
	for (int y = 0; y < dirpad_s.size(); y++) {
		for (int x = 0; x < dirpad_s[y].size(); x++) {
			if (dirpad_s[y][x] == '.') {
				dirpad.forbidden = Vec2i{x, y};
				continue;
			}
			dirpad.mapping.emplace(dirpad_s[y][x], Vec2i{x, y});
		}
	}

	std::vector pads {numpad};
	for (int i = 0; i < num_dpads; i++) {
		pads.push_back(dirpad);
	}

	auto lines = split(input, "\n");
    for (const auto& line : lines) {
		result += find_min(pads, line) * std::stoull(line.substr(0, line.size() - 1));
    }

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<uint64_t, int>(solve, 2024, 21);
    runner.add_test_file("t2.txt", 348, 0);
    runner.add_test_file("t2.txt", 812, 1);
    runner.add_test_file("t1.txt", 126384, 2);

    runner.add_input_file("i1.txt", 2);

    runner.run();
}