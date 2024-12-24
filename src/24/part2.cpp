#include <bitset>
#include <utils.hpp>

enum class Op {
	NONE, AND, OR, XOR, ADD
};

struct Expr {
	std::string left;
	std::string right;
	Op op;
	std::string target;
};

std::vector<std::vector<int>> recursive_perms(std::vector<int> path, int n, int e_max) {
	if (n < 1) return {path};

	std::vector<std::vector<int>> perms;

	for (int i = 0; i < e_max; i++) {
		if (std::ranges::find(path, i) != path.end()) continue;

		path.emplace_back(i);
		perms.append_range(recursive_perms(path, n - 1, e_max));
		path.pop_back();
	}

	return perms;
}

std::vector<std::vector<int>> get_perms(int n, int e_max) {
	return recursive_perms({}, n, e_max);
}

bool eval_expr(std::unordered_map<std::string, bool>& reg, const Expr& e) {
	if (!reg.contains(e.left) || !reg.contains(e.right)) {
		return false;
	}

	bool left = reg[e.left];
	bool right = reg[e.right];
	bool res;
	switch (e.op) {
		case Op::AND: res = left && right; break;
		case Op::OR:  res = left || right; break;
		case Op::XOR: res = left != right; break;
		default: Logger::critical("Unknown operation");
	}
	reg[e.target] = res;
	return true;
}

uint64_t get_number(const std::unordered_map<std::string, bool>& reg, char prefix) {
	uint64_t result = 0;
	for (const auto& [name, val] : reg) {
		if (name[0] != prefix) continue;
		if (!val) continue;

		int offset = std::stoi(name.substr(1));

		result |= 1ull << offset;
	}

	return result;
}

uint64_t eval_sys(std::unordered_map<std::string, bool> reg, const std::vector<Expr>& es) {
	std::queue<Expr> q{es.begin(), es.end()};
	q.emplace("", "", Op::NONE, "");
	bool changed = false;
	while (q.size() > 1) {
		auto e = q.front();
		q.pop();
		if (e.op != Op::NONE) {
			if (!eval_expr(reg, e)) {
				q.push(e);
			} else {
				changed = true;
			}
			continue;
		}

		if (changed) {
			changed = false;
			q.push(e);
			continue;
		}

		Logger::error("Not solvable");
		std::unordered_set<std::string> produced{};
		produced.insert_range(map_key_set(reg));
		std::unordered_set<std::string> missing{};
		while (!q.empty()) {
			const auto e = q.front();
			q.pop();
			if (!produced.contains(e.left)) {
				missing.emplace(e.left);
			}
			if (!produced.contains(e.right)) {
				missing.emplace(e.right);
			}
		}
		std::cout << "Missing: " << missing << std::endl;
		return 0;
	}

	auto result = get_number(reg, 'z');
	return result;
}

void find_diff(uint64_t expected, uint64_t actual) {
	if (expected == actual) {
		std::cout << "No difference" << std::endl;
		return;
	}

	std::cout << std::bitset<46>(expected) << std::endl;
	std::cout << std::bitset<46>(actual) << std::endl;

	auto diff = expected ^ actual;
	std::cout << std::bitset<46>(diff) << std::endl;

	int off = 0;
	std::cout << "Broken units: ";
	while (off < 46) {
		if ((1ull << off) & diff) {
			std::cout << off << ',';
			while ((1ull << off) & diff) off++;
		} else off++;
	}
	std::cout << std::endl;
}

uint64_t run_with(const std::vector<Expr>& es, uint64_t x, uint64_t y) {
	std::unordered_map<std::string, bool> reg;
	for (int i = 0; i < 46; i++) {
		const auto name = i < 10 ? "0" + std::to_string(i) : std::to_string(i);
		reg["x" + name] = !!((1ull << i) & x);
		reg["y" + name] = !!((1ull << i) & y);
	}

	auto result = eval_sys(reg, es);
	return result;
}

std::string solve(const std::string &input, Op wanted, int switched) {
	auto [init_s, expr_s] = split_once(input, "\n\n");
	auto inits = split(init_s, "\n");
	auto exprs = split(expr_s, "\n");

	std::unordered_map<std::string, bool> reg;
	for (const auto& init : inits) {
		auto [name, val] = split_once<std::string, int>(init, ": ");
		reg[name] = val != 0;
	}

	std::regex pattern("([a-z0-9]+) ([A-Z]+) ([a-z0-9]+) -> ([a-z0-9]+)");

	auto xfull = get_number(reg, 'x');
	auto yfull = get_number(reg, 'y');
	uint64_t expected;
	switch (wanted) {
		case Op::AND: expected = xfull & yfull; break;
		case Op::OR:  expected = xfull | yfull; break;
		case Op::XOR: expected = xfull ^ yfull; break;
		case Op::ADD: expected = xfull + yfull; break;
		default: Logger::critical("Unknown operation");
	}

	auto exprs_vec = std::vector<Expr>();
    for (const auto& expr : exprs) {
    	const auto [left, op, right, target] =
    		extract_data<std::string, std::string, std::string, std::string>(expr, pattern);

		Op o;
    	if (op == "AND") o = Op::AND;
    	else if (op == "OR") o = Op::OR;
    	else if (op == "XOR") o = Op::XOR;
    	else Logger::critical("Unknown operation {}", op);

		exprs_vec.emplace_back(left, right, o, target);
    }

	// print all edges (i visualized them using https://cosmograph.app/
	// which doesn't allow moving nodes by hand and was thus painfully slow.
	/*for (int i = 0; i < exprs_vec.size(); i++) {
		std::cout << exprs_vec[i].left << "," << exprs_vec[i].target << std::endl;
		std::cout << exprs_vec[i].right << "," << exprs_vec[i].target << std::endl;
	}*/

	// I quickly realized the repeating addition-units and that none of them where connected over a large distance.
	// Meaning i only had to fix individual addition-units.
	// Using the difference off the expected and actual value i could locate the problems.
	// Writing down each operation happening in one addition-unit helped me find the mistakes in the broken units.

	std::cout << "Before swapping:\n";
	find_diff(expected, eval_sys(reg, exprs_vec));

	std::vector<std::pair<std::string, std::string>> switches{
		{"vcf", "z10"},
		{"z39", "tnc"},
		{"z17", "fhg"},
		{"dvb", "fsq"}
	};

	for (Expr& e : exprs_vec) {
		for (const auto& [a, b] : switches) {
			if (e.target == a) {
				e.target = b;
			} else if (e.target == b) {
				e.target = a;
			} else continue;
			break;
		}
	}

	auto res_after = eval_sys(reg, exprs_vec);
	std::cout << "\nAfter swapping:\n";
	find_diff(expected, res_after);

	if (res_after != expected) {
		return "Not the solution";
	}

	// checks if each simple bit-addition works (helped me find a little mistake in my logic)
	std::cout << "Additional checks for broken units:\n";
	for (int i = 0; i < 45; i++) {
		auto val = 1ull << i;
		auto res = run_with(exprs_vec, val, val);
		if (res != 2 * val) {
			std::cout << std::bitset<45>(val) << " -> " << std::bitset<45>(res) << '\n';
		}
	}

	std::vector<std::string> swapped{};
	for (const auto& [a, b] : switches) {
		swapped.emplace_back(a);
		swapped.emplace_back(b);
	}
	std::ranges::sort(swapped);
	std::string result;
	for (const auto& e : swapped) {
		if (e != swapped.front()) result += ',';
		result += e;
	}
	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<std::string, Op, int>(solve, 2024, 24);

    runner.add_input_file("i1.txt", Op::ADD, 4);

    runner.run();
}