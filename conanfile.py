from conan import ConanFile
from conan.tools.build import check_min_cppstd


class SevenBitDIRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def requirements(self):
        self.requires("gtest/cci.20210126")
