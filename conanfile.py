from conan import ConanFile
from conan.tools.files import load
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
import re


class SevenBitInjectorRecipe(ConanFile):
    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of hello package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"buildLibraryType": ["headerOnly", "static", "dynamic"]}
    default_options = {"buildLibraryType": "headerOnly"}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "Source/*", "Include/*", "Cmake/*"

    def init(self):
        self.projectInformation = self.loadProjectInfo()
        self.name = self.projectInformation["name"]
        self.version = self.projectInformation["version"]

    def loadProjectInfo(self):
        content = load(self, "CMakeLists.txt")
        info = re.search("project\((.*) VERSION (.*)\)", content)
        return {"name": info.group(1), "version": info.group(2)}

    def requirements(self):
        self.requires("gtest/cci.20210126")
        self.requires("benchmark/1.7.1")

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["hello"]
