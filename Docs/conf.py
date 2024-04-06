import os
import subprocess

currentPath = os.path.dirname(__file__)


def createIfNotExists(path):
    if not os.path.exists(path):
        os.makedirs(path)


project = "7bitDI"
copyright = "2023, 7BitCoder Sylwester Dawida"
author = "Sylwester Dawida"
version = "3.1.0"

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

breathe_projects = {"7bitDI": pathDoxygen + "/xml"}
breathe_default_project = "7bitDI"

html_title = f"{project} v{version}"
html_theme = "furo"
html_scaled_image_link = False
html_static_path = ["_static"]
