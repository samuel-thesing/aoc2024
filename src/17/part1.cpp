#include <utils.hpp>

std::string solve(const std::string &input, bool mod_out) {
	std::string result = "";

	std::regex pattern("Register A: (\\d+)\nRegister B: (\\d+)\nRegister C: (\\d+)\n\nProgram: (.*)");

	auto [reg_a, reg_b, reg_c, ins_s] = extract_data<int, int, int, std::string>(input, pattern);
	auto instructions = split<int>(ins_s, ",");

	int pc = 0;
	while (pc >= 0 && pc < instructions.size()) {
		int opcode = instructions[pc];
		int combo = instructions[pc+1];
		if (opcode != 1) {
			switch (combo) {
				case 4: combo = reg_a; break;
				case 5: combo = reg_b; break;
				case 6: combo = reg_c; break;
				case 7: Logger::critical("Encountererd combo 7");
				default: break;
			}
		}

		int target = 0;
		int res = -1;

		// target:
		// 4 -> A
		// 5 -> B
		// 6 -> C
		// 8 -> void
		// 9 -> output
		switch (opcode) {
			case 0: {
				target = 4;
				res = reg_a >> combo;
			} break;
			case 1: {
				target = 5;
				res = reg_b ^ combo;
			} break;
			case 2: {
				target = 5;
				res = combo % 8;
			} break;
			case 3: {
				target = 8;
				if (reg_a != 0) {
					pc = combo;
					continue;
				}
			} break;
			case 4: {
				target = 5;
				res = reg_b ^ reg_c;
			} break;
			case 5: {
				target = 9;
				res = combo;
			} break;
			case 6: {
				target = 5;
				res = reg_a >> combo;
			} break;
			case 7: {
				target = 6;
				res = reg_a >> combo;
			} break;
		}

		switch (target) {
			case 4: reg_a = res; break;
			case 5: reg_b = res; break;
			case 6: reg_c = res; break;
			case 8: break;
			case 9:
				if (mod_out) {
					res %= 8;
				}
			result += std::to_string(res) + ',';
			break;
			default: Logger::critical("Unknown target '{}'", target);
		}

		pc += 2;
	}

	if (!result.empty())
		result.pop_back();
	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<std::string, bool>(solve, 2024, 17);
    runner.add_test_file("t1.txt", "1", false);
    runner.add_test_file("t2.txt", "0,1,2", true);
    runner.add_test_file("t3.txt", "4,2,5,6,7,7,7,7,3,1,0,0", true);
    runner.add_test_file("t4.txt", "26", false);
    runner.add_test_file("t5.txt", "44354", false);

    runner.add_input_file("i1.txt", true);

    runner.run();
}