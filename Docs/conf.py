import subprocess
import os


currentPath = os.path.dirname(__file__)


def createIfNotExists(path):
    isExist = os.path.exists(path)
    if not isExist:
        os.makedirs(path)


project = "7bitInjector"
copyright = "2023, 7BitCoder Sylwester Dawida"
author = "Sylwester Dawida"
version = "0.1.0"

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.doctest",
    "sphinx.ext.mathjax",
    "sphinx.ext.viewcode",
    "sphinx.ext.imgmath",
    "sphinx.ext.todo",
    "sphinx_copybutton",
    "breathe",
]

pathDoxygen = currentPath + "/../build/docs/doxygen"
pathSphinx = currentPath + "/../build/docs/sphinx"
createIfNotExists(pathDoxygen)
createIfNotExists(pathSphinx)

subprocess.call("doxygen ./Doxyfile", shell=True)

breathe_projects = {"7bitInjector": pathDoxygen + "/xml"}
breathe_default_project = "7bitInjector"

html_title = f"{project} v{version}"
html_theme = "furo"
html_scaled_image_link = False
html_static_path = ["_static"]
