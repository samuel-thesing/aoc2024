#include <utils.hpp>

long long solve(const std::string &input) {
	long long result = 0;

	std::regex pattern("Button A: X\\+(\\d+), Y\\+(\\d+)\nButton B: X\\+(\\d+), Y\\+(\\d+)\nPrize: X=(\\d+), Y=(\\d+)");

	auto blocks = split(input, "\n\n");
	for (const auto& block : blocks) {
        auto [ax, ay, bx, by, xtarget, ytarget] = extract_data<int, int, int, int, int, int>(block, pattern);

		long long i = 0;
		while (ax * i <= xtarget || ay * i <= ytarget) {

			auto btx = (xtarget - ax * i) / bx;
			auto bty = (ytarget - ay * i) / by;
			auto brx = (xtarget - ax * i) % bx;
			auto bry = (ytarget - ay * i) % by;
			if (brx == 0 && bry == 0 && btx == bty) {

				std::cout << i << " " << btx << std::endl;
				result += i * 3 + btx;
			}
			i++;
		}
	}


	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2024, 13);
    runner.add_test_file("t1.txt", 480);

    runner.add_input_file("i1.txt");

    runner.run();
}