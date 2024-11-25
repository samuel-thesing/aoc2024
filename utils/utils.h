#ifndef UTILS_H
#define UTILS_H

#include <regex>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <math.h>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <filesystem>
#include <Logger.h>
#include <utils.h>
#include <set>
#include <iostream>

#include <Logger.h>

#include "matrix.h"


/* ====================================================================================================
 * Typedefs
 */
template <typename Value>
using Table = std::vector<std::vector<Value>>;

template <typename Key, typename Value>
using NamedTable = std::unordered_map<Key, std::unordered_map<Key, Value>>;

/* ====================================================================================================
 * Structs
 */
struct Point {
	int x;
	int y;
};

template<>
struct std::hash<Point> {
	size_t operator()(const Point& point) const {
		return static_cast<size_t>(point.x) ^ (static_cast<size_t>(point.y) << 32);
	}
};

/* ====================================================================================================
 * Whitespace Correction
 */

/**
 * fills the left side of a string to the given length using the filler character
 * @param s string to be filled
 * @param len length to be filled to
 * @param filler char used to fill the string - default space
 * @return filled string
 */
std::string pad_left(const std::string& s, int len, char filler = ' ') {
	return std::string(std::max(len - s.length(), 0ull), filler) + s;
}

/**
 * fills the right side of a string to the given length using the filler character
 * @param s string to be filled
 * @param len length to be filled to
 * @param filler char used to fill the string - default space
 * @return filled string
 */
std::string pad_right(const std::string& s, int len, char filler = ' ') {
	return s + std::string(std::max(len - s.length(), 0ull), filler);
}

std::string pad_center(const std::string& s, int len, char filler = ' ') {
	auto len_half = s.length() / 2;
	return pad_right(pad_left(s, len_half, filler), len - len_half, filler);
}

std::string repeat(char c, int n) {
	return std::string(n, c);
}

/**
 * trims whitespace (recognized using
 * std::isspace()) from both ends of the string
 * @param s string to be trimmed
 * @return trimmed string
 */
std::string trim(std::string s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
		return !std::isspace(c);
	}));

	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) {
		return !std::isspace(c);
	}).base(), s.end());

	return s;
}

std::string replace_all(const std::string& str, const std::string& pattern, const std::string& replace) {
	size_t n = 0;
	std::string str_cpy = str;

	while ((n = str_cpy.find(pattern, n)) != std::string::npos){
		str_cpy.replace(n, pattern.size(), replace);
		n += replace.size();
	}

	return str_cpy;
}

size_t find_nth(const std::string& str, const std::string& pattern, size_t n) {
	int count = 0;
	size_t last_idx = str.find(pattern, n);
	while (last_idx != std::string::npos) {
		if (count == n) {
			return last_idx;
		}
		last_idx = str.find(pattern, last_idx+1);
		++count;
	}
	return std::string::npos;
}

std::optional<std::string> replace_nth(const std::string& str, const std::string& pattern, std::string replace, int n) {
	auto idx = find_nth(str, pattern, n);
	if (idx == std::string::npos) {
		return {};
	}

	return str.substr(0, idx) + replace + str.substr(idx + pattern.size());
}

/* ====================================================================================================
 * Reading Data
 */
/**
 * Reads file
 * @param filename
 * @return content
 * @throws 0xDEAD If file could not be found or opened
 */
std::string read_file(const std::string& filename) {
	auto dir_name = std::filesystem::current_path().filename().string();
	auto cwd = std::filesystem::current_path() / "../../../" / "src" / dir_name;
	cwd = std::filesystem::canonical(cwd);
	auto file = std::ifstream(cwd / filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
		Logger::critical("Failed to open file '{}'. CWD: {}", filename, cwd.string());
    }

	std::stringstream contents;
	contents << file.rdbuf();

	return replace_all(contents.str(), "\r\n", "\n");
}

std::vector<std::string> split_lines(const std::string& s) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string line;
	while (std::getline(ss, line)) {
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		result.emplace_back(line);
	}
	return result;
}

/* ====================================================================================================
 * Printing Data
 */
/**
 * Prints a 2d unordered_map
 * @param mat data to print
 * @param keyFormatter transformation function for key-values
 * @param valueFormatter transformation function for data-values
 */
template<typename Key, typename Value>
void printAdjacencyMatrix(const NamedTable<Key, Value>& mat,
	std::function<std::string(Key)> keyFormatter, std::function<std::string(Value)> valueFormatter) {

	std::set<Key> keys;
	int max_key_len = 0;
	for (const auto& [key, row] : mat) {
		keys.emplace(key);
		max_key_len = std::max(max_key_len, (int)(keyFormatter(key).size()));
		for (const auto& [key2, _] : row) {
			keys.emplace(key2);
			max_key_len = std::max(max_key_len, (int)(keyFormatter(key2).size()));
		}
	}

	int keyCounter = 0;
	std::unordered_map<Key, int> keyIdx;
	for (const auto& key : keys) {
		keyIdx.emplace(key, keyCounter);
		keyCounter++;
	}

	std::vector<std::vector<std::string>> values;
	for (int i = 0; i < keyCounter; i++) {
		values.emplace_back(keyCounter, pad_left("-", max_key_len) + "  ");
	}

	for (const auto& [key, row] : mat) {
		const auto x = keyIdx.find(key);
		for (const auto& [key2, value] : row) {
			const auto y = keyIdx.find(key2);
			values[x->second][y->second] = pad_left(valueFormatter(value), max_key_len) + "  ";
		}
	}

	std::cout << pad_right("", max_key_len);
	for (const auto& key : keys) {
		std::cout << "  " << pad_right(std::string(key), max_key_len);
	}
	std::cout << std::endl;

	for (const auto& [key, row] : mat) {
		std::cout << pad_right(std::string(key), max_key_len) << "  ";
		const auto idx = keyIdx.find(key);
		for (const auto& value : values[idx->second]) {
			std::cout << value;
		}
		std::cout << std::endl;
	}
}

/* ====================================================================================================
 * Splitting
 */

/**
 *	Splits a given string at the given delimiter and trims the parts
 *	@param s string to be split
 *	@param delim delimiter (can be longer than 1 char)
 */
std::vector<std::string> split(const std::string& s, const std::string& delim) {
	if (delim.empty()) {
		Logger::critical("`split` received an empty delimiter");
		return {};
	}
	if (trim(s).empty()) {
		return {};
	}

	size_t lastDelim = 0;
	auto curDelim = s.find(delim, lastDelim);
	std::vector<std::string> parts{};

	while (curDelim != std::string::npos) {
		std::string part = s.substr(lastDelim, curDelim - lastDelim);
		parts.emplace_back(trim(part));
		lastDelim = curDelim + delim.size();
		curDelim = s.find(delim, lastDelim);
	}

	auto lastPart = s.substr(lastDelim);
	parts.emplace_back(trim(lastPart));

	return parts;
}

/**
 *	Splits a given string at the given delimiter and trims the parts before converting them using the given function.
 *	@param s string to be split
 *	@param delim delimiter (can be longer than 1 char)
 *	@param fn converts parts after splitting using this function
 */
template<typename T>
std::vector<T> split(const std::string& s, const std::string& delim, std::function<T(std::string)> fn) {
	auto splitted = split(s, delim);
	auto result = std::vector<T>();
	result.reserve(splitted.size());
	for (auto str : splitted) {
		result.emplace_back(fn(str));
	}
	return result;
}

/**
 *	Splits a given string at the first occurrence of the given delimiter and returns both parts as pair
 *	@param s string to be split
 *	@param delim delimiter (can be longer than 1 char)
 *	@param fn converts parts after splitting using this function
 */
std::pair<std::string, std::string> split_once(const std::string& s, const std::string& delim) {
	auto idx = s.find(delim);
	if (idx == std::string::npos) {
		return {s, ""};
	}

	return {s.substr(0, idx), s.substr(idx + delim.size())};
}

template<typename T, typename... Args> requires (std::is_same_v<T, Args...>)
T max(T arg, Args... args) {
	return max(arg, max(args));
}

template<typename T, typename U> requires (std::is_same_v<T, U>)
T max(T t, U u) {
	return t > u ? t : u;
}

template<typename T, typename... Args> requires (std::is_same_v<T, Args...>)
T min(T arg, Args... args) {
	return min(arg, min(args));
}

template<typename T, typename U> requires (std::is_same_v<T, U>)
T min(T t, U u) {
	return t < u ? t : u;
}



template<typename T>
T string_to_generic(std::string s) {
	Logger::critical("No implementation for generating this generic");
	return T();
}

template<>
std::string string_to_generic<std::string>(std::string s) {
	return s;
}

template<>
int string_to_generic<int>(std::string s) {
	return std::stoi(s);
}

template<>
long string_to_generic<long>(std::string s) {
	return std::stol(s);
}

template<>
double string_to_generic<double>(std::string s) {
	return std::stod(s);
}

template<>
char string_to_generic<char>(std::string s) {
	return s[0];
}


template<typename... Args, std::size_t... Indices>
std::tuple<Args...> make_tuple_from_match(const std::smatch& match, std::index_sequence<Indices...>) {
	return std::make_tuple<Args...>(string_to_generic<Args>(match[Indices + 1].str())...);
}

/**
 * Extracts data from a given string using a regex and converts the matches to the correct type using the generics provided
 * @tparam Args Types of the matches. A template specialization of string_to_generic must be provided
 * @param pattern regex pattern to match
 * @param s	string to be matched
 * @return tuple of the converted matches
 */
template<typename... Args>
std::tuple<Args...> extract_data(const std::regex& pattern, std::string s) {
	std::smatch match;
	if (!std::regex_match(s, match, pattern)) {
		Logger::critical("Failed to match regex for '{}'", s);
	}

	return make_tuple_from_match<Args...>(match, std::index_sequence_for<Args...>{});
}

bool isDigit(char c) {
	return '0' <= c && c <= '9';
}

bool isLowercase(char c) {
	return 'a' <= c && c <= 'z';
}

bool isUppercase(char c) {
	return 'A' <= c && c <= 'Z';
}

std::string format_time(std::chrono::duration<std::chrono::nanoseconds::rep, std::chrono::nanoseconds::period> duration) {
	std::string result = "";
	const int64_t lengths[] = {
		// nano
		1000, //micro
		1000, //milli
		1000, //sec
		60, // min
		60, // hours
		24, // days
		365 // years
	};

	const char* names[] = {" ns", " \xE6s ", " ms ", " s ", " min ", " h ", " d "};

	auto rest = duration.count();
	int i = 0;
	while (rest != 0 && i < 7) {
		result.insert(0, std::to_string(rest % lengths[i]) + names[i]);
		rest /= lengths[i];
		i++;
	}
	if (rest != 0) {
		result = std::to_string(rest) + " a " + result;
	}
	return result;
}

template<typename Result, typename... Args>
struct Test {
	std::string input;
	Result expected;
	bool file;
	std::tuple<Args...> args;
};

template<typename... Args>
struct Input {
	std::string input;
	bool file;
	std::tuple<Args...> args;
};

template <typename Result, typename... Args>
class Runner {
private:
	typedef std::function<Result(std::string, Args...)> SolverFn;
	typedef std::function<std::string(Result)> ResultTransformFn;

	SolverFn solve_fn;
	ResultTransformFn result_transform_fn = nullptr;

	std::vector<Test<Result, Args...>> tests;
	std::vector<Input<Args...>> inputs;
	std::vector<Result> results;

	unsigned tests_failed;
	unsigned tests_succeeded;

public:
	Runner(SolverFn solve_fn, const int year, const int day) : solve_fn(solve_fn), tests_failed(0), tests_succeeded(0) {
		Logger::init();
		Logger::info("==================================================");
		Logger::info("=========== Advent of Code {} Day {} ===========", year, pad_left(std::to_string(day), 2, '0'));
		Logger::info("==================================================");
	}

	void set_result_transformation(std::function<std::string(Result)> result_transform_fn) {
		this->result_transform_fn = result_transform_fn;
	}

	void add_test_string(const std::string& input, Result expected, Args... args) {
		tests.push_back(Test<Result, Args...>(input, expected, false, args...));
	}

	void add_test_file(const std::string& filename, Result expected, Args... args) {
		tests.push_back(Test<Result, Args...>(filename, expected, true, args...));
	}

	void add_input_string(const std::string& input, Args... args) {
		inputs.push_back(Input<Args...>(input, false, args...));
	}

	void add_input_file(const std::string& filename, Args... args) {
		inputs.push_back(Input<Args...>(filename, true, args...));
	}

	bool run_test(const Test<Result, Args...>& test) {
		auto input = test.input;
		if (test.file) {
			input = read_file(input);
		}
		Result result = std::apply(
			[=](auto&&... args) -> Result {
				return solve_fn(input, args...);
			},
			test.args
		);

		if (result == test.expected) {
			tests_succeeded++;
			return true;
		}

		Logger::error("Failed Test '{}': Expected {} but got {}", test.input, test.expected, result);
		tests_failed++;
		return false;
	}

	bool run_tests() {
		if (tests.empty()) return true;

		Logger::info("==================================================");
		Logger::info("Running {} Tests", tests.size());
		Logger::info("==================================================");

		tests_failed = 0;
		tests_succeeded = 0;

		for (auto& test : tests) {
			run_test(test);
		}

		Logger::info("==================================================");
		Logger::info("Tests Finished");
		Logger::info("--------------------------------------------------");
		if (tests_succeeded != 0) {
			Logger::info("Succeded: {}", tests_succeeded);
		}

		if (tests_failed != 0) {
			Logger::error("Failed: {}", tests_failed);
		}
		Logger::info("==================================================");

		return tests_failed == 0;
	}

	Result run_input(const Input<Args...>& input) {
		auto input_str = input.input;
		if (input.file) {
			input_str = read_file(input_str);
		}
		auto start_time = std::chrono::high_resolution_clock::now();
		Result result = std::apply(
			[=](auto&&... args) -> Result {
				return solve_fn(input_str, args...);
			},
			input.args
		);
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration =
			std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

		if (result_transform_fn == nullptr) {
			Logger::info("Input Finished '{}': {} ({})", input.input, result, format_time(duration));
		} else {
			Logger::info("Input Finished '{}': {} ({})", input.input, result_transform_fn(result), format_time(duration));
		}

		return result;
	}

	std::vector<Result> run_inputs() {
		results.clear();
		for (auto& input : inputs) {
			results.emplace_back(run_input(input));
		}
		return results;
	}

	std::vector<Result> run() {
		if (!run_tests()) return {};
		run_inputs();
		return results;
	}
};

std::vector<size_t> find_all_idx(const std::string& s, const std::string& pattern) {
	std::vector<size_t> idxs{};
	size_t last_idx = s.find(pattern);
	while (last_idx != std::string::npos) {
		idxs.push_back(last_idx);
		last_idx = s.find(pattern, last_idx+1);
	}
	return idxs;
}


#endif //UTILS_H
