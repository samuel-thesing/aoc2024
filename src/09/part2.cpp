#include <utils.hpp>

/*
 * I should have stuck with the approach from p1. Just scan the len of the file to move and the free spaces every time.
 * Though that should be slower for large files. Largest file here can only be length 9 ...
 * Merge-logic is a evil
 */

struct DiskPart {
	int pos;
	int size;
	int id = -1;
	bool free = true;
};

long long solve(const std::string &input) {
	std::vector<DiskPart> disk{};

	int id = 0;
	int pos = 0;
    for (int i = 0; i < input.size(); i++) {
    	if (i % 2 == 0) {
    		char size = input[i] - '0';
    		disk.emplace_back(pos, size, id, false);
    		pos += size;
    		id++;
    	} else {
    		char free = input[i] - '0';
    		disk.emplace_back(pos, free);
    		pos += free;
    	}
    }

	// move whole blocks
	id--;
	for (; id >= 0; id--) {
		auto source = std::ranges::find_if(disk,
			[&](const DiskPart& d) { return d.id == id; });
		if (source == disk.end()) {
			Logger::critical("Part {} does not exist", id);
		}

		auto dest = std::find_if(disk.begin(), source,
			[&](const DiskPart& d) { return d.free && d.size >= source->size; });
		if (dest == source) {
			continue;
		}

		// if there is more free space than needed the iterator will be invalidated
		int i = std::distance(disk.begin(), source);

		// move part
		if (dest->size == source->size) {
			dest->free = false;
			dest->id = id;
			source->free = true;
			source->id = -1;
		} else {
			int pos = dest->pos;
			dest->size -= source->size;
			dest->pos += source->size;
			source->free = true;
			source->id = -1;

			disk.insert(dest, {pos, source->size, id, false});
			i++; // source pos moved
		}

		// merge freed parts
		if (i+1 < disk.size() && disk[i+1].free) {
			disk[i].size += disk[i+1].size;
			disk.erase(disk.begin() + (i+1));
		}
		if (disk[i-1].free) {
			disk[i].size += disk[i-1].size;
			disk[i].pos -= disk[i-1].size;
			disk.erase(disk.begin() + (i - 1));
		}
	}

	// checksum
	long long result = 0;
	for (const auto& part : disk) {
		if (part.free) continue;

		for (int j = 0; j < part.size; j++) {
			result += part.id * (part.pos + j);
		}
	}

	return result;
}

int main(int argc, char** argv) {
    auto runner = Runner<long long>(solve, 2024, 9);
    runner.add_test_file("t1.txt", 2858);

    runner.add_input_file("i1.txt");

    runner.run();

	// 1851882345
}