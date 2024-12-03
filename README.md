# Advent of Code C++ Template

Ich hatte keine Lust auf meine Seminararbeit und wollte irgendwie prokrastinieren ...

# How to use

1. Jahr in [`generateDays.py`](generateDays.py) anpassen
2. [`generateDays.py`](generateDays.py) ausführen

# Was ist drin?

- [`/templates`](templates) dises Dateien werden für jeden Tag kopiert (ein bisschen replace um den Tag und das Jahr reinzubringen)
- [`/deps`](deps) libraries; für jetzt erstmal nur spdlog
- [`/src`](src) enthält einen Ordner für jeden Tag
  - Textdateien sind für größere Input-Strings; Präfix "t" steht für for "Test"
- [`/utils`](utils) ein paar kleine Util-Funktionen;
  - `extract_data<Ts...>(pattern, string)` regex_match, wobei die captures in die angegebenen Typen umgewandelt werden. Um andere Typen zu unterstützen einfach die Template-Specialization von `generic_from_string<T>(string)` implementieren.\
    (siehe auch `extract_data_all` & `extract_data_opt`)
  - `Runner<Result, Args...>(solver_fn, year, day)` Einfacher wrapper um ein bisschen boilerplate zu sparen
    - tests mit `add_test_string(string, expected, args...)` oder `add_test_file(filename, expected, args...)` hinzufügen
    - echte Inputs mit `add_input_string(string, args...)` oder `add_input_file(filename, args...)`
    - falls das letztendliche Ergebnis noch leicht transformiert werden muss im Gegensatz zu den Testinputs  `set_result_transformation(result_transform_fn)`
    - leider noch keine Unterstützung für Unit-Tests
 

### Beispiele
```cpp
std::regex pattern("(.+) would (.+) (\\d+) happiness units by sitting next to (.+).");
const auto [person_a, signum, strength, person_b] =
    extract_data<std::string, std::string, int, std::string>(pattern, line);
```

```cpp
auto runner = Runner<int, int>(solve, 2015, 17);

runner.add_test_file("t1.txt", 4, 25);
runner.add_test_input("some test data", 10, 43);

runner.add_input_file("i1.txt", 150);

runner.run();
```
