import json
import os
import re
import subprocess
import sys


def get_bin_dir():
    if len(sys.argv) < 2:
        raise Exception("Binary directory not provided")
    bin_dir = sys.argv[1]
    if not os.path.exists(bin_dir):
        raise Exception(f"Binary directory does not exist {bin_dir}")
    return bin_dir


def get_expected_test_data_path():
    if len(sys.argv) < 3:
        raise Exception("Expected test data file path not provided")
    expected_test_data_path = sys.argv[2]
    if not os.path.exists(expected_test_data_path):
        raise Exception(f"Expected test data file does not exist: {expected_test_data_path}")
    return expected_test_data_path


class ExamplesTest:
    def __init__(self, bin_dir, expected_test_data_path):
        self.bin_dir = bin_dir
        self.expected_test_data_path = expected_test_data_path
        self.examples_paths = self.__get_available_examples()
        self.expected_test_map = self.__get_expected_test_map()

    def __get_available_examples(self):
        paths = [os.path.join(self.bin_dir, f) for f in os.listdir(self.bin_dir) if
                 "Example" in f and os.path.splitext(f)[1] in [".exe", ""]]
        return [f for f in paths if os.path.isfile(f)]

    def __get_expected_test_map(self):
        with open(self.expected_test_data_path) as data:
            return json.load(data)

    def run(self):
        all_tests = len(self.examples_paths)
        succeeded_tests = 0
        for count, examplePath in enumerate(self.examples_paths, start=1):
            print(f"Test {count}/{all_tests}")
            succeeded_tests += self.__run_test_and_summarize(examplePath)
        if len(self.expected_test_map):
            raise Exception(f"Some expected tests were not run: {self.expected_test_map.keys()}")
        if succeeded_tests == all_tests:
            print(f"All test succeeded: {succeeded_tests}/{all_tests}")
        else:
            raise Exception(f"Some tests failed: {all_tests - succeeded_tests}/{all_tests}")

    def __run_test_and_summarize(self, example_path):
        file_name = os.path.basename(example_path)
        name = os.path.splitext(file_name)[0].replace("Example", "")
        try:
            self.__run_test(example_path, self.expected_test_map.get(name))
            print(f"{name} example test succeeded")
            return True
        except Exception as e:
            print(f"{name} example test failed: {e}")
        finally:
            del self.expected_test_map[name]
        return False

    @staticmethod
    def __run_test(example_path, expected):
        if expected is None:
            raise Exception(f"Example was not expected")
        result = subprocess.run([example_path], capture_output=True, text=True)
        if result.returncode:
            raise Exception(f"Example returned non zero code {result.returncode}")
        if not re.search(expected, result.stdout):
            raise Exception(f"Result of running example: '{result.stdout}' does not match expected: '{expected}'")


if __name__ == "__main__":
    ExamplesTest(get_bin_dir(), get_expected_test_data_path()).run()
