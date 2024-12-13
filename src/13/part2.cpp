#include <utils.hpp>

long long solve(const std::string &input) {
	long long result = 0;

	const std::regex pattern("Button A: X\\+(\\d+), Y\\+(\\d+)\nButton B: X\\+(\\d+), Y\\+(\\d+)\nPrize: X=(\\d+), Y=(\\d+)");

	auto blocks = split(input, "\n\n");
	for (const auto& block : blocks) {
        auto [A, D, B, E, C, F] = extract_data<long long, long long, long long, long long, long long, long long>(block, pattern);
		C += 10000000000000L;
		F += 10000000000000L;

		auto denom = A*E - B*D;

		if (denom == 0) continue;

		auto adet = (C*E - B*F);
		auto bdet = (A*F - C*D);

		if (adet % denom != 0 || bdet % denom != 0) continue;

		auto asol = (C*E - B*F) / denom;
		auto bsol = (A*F - C*D) / denom;

		if (asol < 0 || bsol < 0) continue;
		result += asol * 3 + bsol;
	}


	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<long long>(solve, 2024, 13);

	runner.add_input_file("i1.txt");

	runner.run();
}