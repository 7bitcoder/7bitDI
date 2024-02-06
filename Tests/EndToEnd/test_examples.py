import sys
import os
import subprocess
import re
import json


def getBinDir():
    if len(sys.argv) != 2:
        raise Exception("binary directory not provided")
    binDir = sys.argv[1]
    if not os.path.exists(binDir):
        raise Exception("Binary directory does not exist")
    return binDir


class ExamplesTest:
    def __init__(self, binDir):
        self.binDir = binDir
        self.examplesPaths = self.__getAvailableExamples()
        self.expectedTestMap = self.__getExpectedTestMap()

    def __getAvailableExamples(self):
        paths = [os.path.join(self.binDir, f) for f in os.listdir(self.binDir) if
                 "Example" in f and os.path.splitext(f)[1] in [".exe", ""]]
        return [f for f in paths if os.path.isfile(f)]

    def __getExpectedTestMap(self):
        with open('expectedTestData.json') as data:
            return json.load(data)

    def __runTest(self, examplePath, expected):
        if expected is None:
            raise Exception(f"example was not expected")
        result = subprocess.run([examplePath], capture_output=True, text=True)
        if result.returncode:
            raise Exception(f"example returned non zero code {result.returncode}")
        if not re.search(expected, result.stdout):
            raise Exception(f"result of running example: '{result.stdout}' does not match expected: '{expected}'")

    def __runTestAndSummarize(self, examplePath):
        fileName = os.path.basename(examplePath)
        name = os.path.splitext(fileName)[0].replace("Example", "")
        try:
            self.__runTest(examplePath, self.expectedTestMap.get(name))
            print(f"{name} example test succeeded")
            return True
        except Exception as e:
            print(f"{name} example test failed: {e}")
        finally:
            del self.expectedTestMap[name]
        return False

    def run(self):
        allTests = len(self.examplesPaths)
        succeededTests = 0
        for count, examplePath in enumerate(self.examplesPaths, start=1):
            print(f"Test {count}/{allTests}")
            succeededTests += self.__runTestAndSummarize(examplePath)
        if len(self.expectedTestMap):
            raise Exception(f"Some expected tests were not run: {self.expectedTestMap.keys()}")
        if succeededTests == allTests:
            print(f"All test succeeded: {succeededTests}/{allTests}")
        else:
            raise Exception(f"Some tests failed: {allTests - succeededTests}/{allTests}")


if __name__ == "__main__":
    ExamplesTest(getBinDir()).run()
