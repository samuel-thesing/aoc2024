#include <utils.hpp>

// Naja. Auch mal endlich geschafft ...
// Das Beispiel aus Part 2 und der Input sind beides Programme die eine Schleife darstellen.
// Eine der wichtigsten Erkenntnisse ist erstmal, dass adv, bdv und cdv bit-shifts sind.
// Beide Programme shiften Register A jede Iteration um 3 Bits nach rechts und verändern es sonst gar nicht.
// Damit muss Register A also instructions * 3 Bits enthalten.
// Der wirkliche Input ist ein bisschen komplexer, da er A noch ein weiteres Mal in der Rechnung verwendet.
// Dabei werden wieder nur 3 Bits genutzt, die dieses Mal aber auch links von den momentanen 3 Bits liegen können.
// Da links von der ersten Bit-Gruppe keine anderen Bist als 0 sind kann das dabei vernachlässigt werden.
// Wichtig ist allerdings, dass die Zahl von links nach rechts aufgebaut werden muss.
// Die Bit-Gruppe ganz links steht für die letzte Instruction, die zweite Bit-Gruppe für die vorletzte Instruction usw.
// Dann muss man nur noch für jede Bit-Gruppe von links nach rechts die 8 Möglichkeiten durchprobieren und
// den Output gegen die Instruction checken.

std::vector<int> instructions;

std::vector<int> run_prog(long long reg_a) {
	std::vector<int> output{};
	long long reg_b = 0;
	long long reg_c = 0;

	int pc = 0;
	while (pc >= 0 && pc < instructions.size()) {
		int opcode = instructions[pc];
		long long combo = instructions[pc+1];
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
		long long res = -1;

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
				res = combo % 8;
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
			case 9: output.emplace_back(res); break;
			default: Logger::critical("Unknown target '{}'", target);
		}

		pc += 2;
	}

	return output;
}

long long recursive(long long reg_a = 0, int offset = 0) {
	if (offset == instructions.size()) {
		return reg_a;
	}

	for (long long k = 0; k < 8; k++) {
		// neuen Wert an alten anhängen und testen
		auto a = (reg_a << 3) + k;
		auto result = run_prog(a);
		if (result.size() != offset+1) continue;
		// check output
		for (int i = instructions.size() - 1, j = result.size()-1; j >= 0; i--, j--) {
			if (instructions[i] != result[j]) {
				goto outer;
			}
		}

		if (result.size() == instructions.size()) {
			return a;
		} else {
			// Anscheinend mag C++ es nicht, wenn mit einem goto eine Initialisierung übersprungen wird,
			// auch wenn das hier eigentlich egal wäre, da res nie wieder verwendet wird.
			auto res = recursive(a, offset + 1);
			if (res != -1) return res;
		}

		outer:;
	}

	return -1;
}

long long solve(const std::string &input) {
	std::regex pattern("Register A: (\\d+)\nRegister B: (\\d+)\nRegister C: (\\d+)\n\nProgram: (.*)");

	auto [reg_a, reg_b, reg_c, ins_s] = extract_data<int, int, int, std::string>(input, pattern);
	instructions = split<int>(ins_s, ",");

	return recursive();
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2024, 17);
    runner.add_test_file("t7.txt", 117440);

    runner.add_input_file("i1.txt");

    runner.run();
}