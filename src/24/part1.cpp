#include <bitset>
#include <utils.hpp>

enum class Op {
	AND, OR, XOR
};

struct Expr {
	std::string left;
	std::string right;
	Op op;
	std::string target;
};

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

uint64_t solve(const std::string &input) {
	auto [init_s, expr_s] = split_once(input, "\n\n");
	auto inits = split(init_s, "\n");
	auto exprs = split(expr_s, "\n");

	std::unordered_map<std::string, bool> reg;
	for (const auto& init : inits) {
		auto [name, val] = split_once<std::string, int>(init, ": ");
		reg[name] = val != 0;
	}

	std::regex pattern("([a-z0-9]+) ([A-Z]+) ([a-z0-9]+) -> ([a-z0-9]+)");

	auto q = std::queue<Expr>();
    for (const auto& expr : exprs) {
    	const auto [left, op, right, target] =
    		extract_data<std::string, std::string, std::string, std::string>(expr, pattern);

		Op o;
    	if (op == "AND") o = Op::AND;
    	else if (op == "OR") o = Op::OR;
    	else if (op == "XOR") o = Op::XOR;
    	else Logger::critical("Unknown operation {}", op);

    	auto e = Expr{left, right, o, target};

		if (!eval_expr(reg, e)) {
			q.push(e);
		}
    }

	while (!q.empty()) {
		auto e = q.front();
		q.pop();
		if (!eval_expr(reg, e)) {
			q.push(e);
		}
	}

	uint64_t result = 0;
	for (const auto& [name, val] : reg) {
		if (name[0] != 'z') continue;
		std::cout << name << ": " << (int)val << std::endl;
		if (!val) continue;

		int offset = std::stoi(name.substr(1));

		result |= 1ull << offset;
	}

	std::bitset<64> bits(result);
	std::cout << bits << std::endl;

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<uint64_t>(solve, 2024, 24);
    runner.add_test_file("t1.txt", 4);
    runner.add_test_file("t2.txt", 2024);

    runner.add_input_file("i1.txt");

    runner.run();
}