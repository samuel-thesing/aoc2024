#include <utils.hpp>

int test_sequence(const std::vector<std::pair<std::array<int, 2000>, std::array<int, 2001>>>& buyers, int i, int j, int k, int l) {
	if (std::abs(i+j+k+l) > 9) return 0;

	int winning = 0;
	for (const auto& [diffs, secrets] : buyers) {
		for (int m = 0; m+3 < 2000; m++) {
			if (diffs[m] == i && diffs[m+1] == j && diffs[m+2] == k && diffs[m+3] == l) {
				winning += secrets[m+4];
				goto outer;
			}
		}
		outer:;
	}

	return winning;
}

uint64_t solve(const std::string &input) {
	auto nums = split<uint64_t>(input, "\n");

	std::vector<std::pair<std::array<int, 2000>, std::array<int, 2001>>> diffs{};

	for (const auto& num : nums) {
		auto secret = num;
		std::array<int, 2000> diff{};
		std::array<int, 2001> secrets{};
		secrets[0] = secret;
		for (int i = 0; i < 2000; i++) {
			secret ^= secret << 6;
			secret %= 16777216;
			secret ^= secret >> 5;
			secret %= 16777216;
			secret ^= secret << 11;
			secret %= 16777216;
			secrets[i+1] = secret % 10;
			diff[i] = secrets[i+1] - secrets[i];
		}
		diffs.emplace_back(diff, secrets);
	}

	auto result = 0ll;

	for (int i = -9; i <= 9; i++) {
		for (int j = -9; j <= 9; j++) {
			std::cout << i << " " << j << " " << result << std::endl;
			for (int k = -9; k <= 9; k++) {
				for (int l = -9; l <= 9; l++) {
					int winning = test_sequence(diffs, i, j, k, l);
					result = max(result, (long long)winning);
				}
			}
		}
	}

	return result;
}

int main(int argc, char** argv) {
	auto runner = Runner<uint64_t>(solve, 2024, 22);

	runner.add_test_file("t3.txt", 23);

	runner.add_input_file("i1.txt");

	runner.run();
}