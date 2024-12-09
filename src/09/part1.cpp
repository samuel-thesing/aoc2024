#include <utils.hpp>

struct DiskPart {
	int id = 0;
	bool free = true;
};

long long solve(const std::string &input) {
	std::vector<DiskPart> disk{};

	int id = 0;
	int pos = 0;
    for (int i = 0; i < input.size(); i++) {
    	if (i % 2 == 0) {
    		char size = input[i] - '0';
    		for (int j = 0; j < size; j++) {
    			disk.emplace_back(id, false);
    		}
    		pos += size;
    		id++;
    	} else {
    		char free = input[i] - '0';
    		for (int j = 0; j < free; j++) {
    			disk.emplace_back();
    		}
    		pos += free;
    	}
    }

	// move single blocks
	int end = disk.size() - 1;
	int start = 0;
	while (start < end) {
		while (!disk[start].free && start < end) {
			start++;
		}
		while (disk[end].free && end > start) {
			end--;
		}
		if (end <= start) {
			break;
		}

		disk[start].free = false;
		disk[start].id = disk[end].id;
		disk[end].free = true;
	}

	// checksum
	long long result = 0;
	for (int i = 0; i < disk.size(); i++) {
		if (disk[i].free) continue;

		result += disk[i].id * i;
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2024, 9);
    runner.add_test_file("t2.txt", 60);
    runner.add_test_file("t3.txt", 513);
    runner.add_test_file("t1.txt", 1928);

    runner.add_input_file("i1.txt");

    runner.run();

	//
}