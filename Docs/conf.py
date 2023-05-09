# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = "7BitInjector"
copyright = "2023, 7BitCoder Sylwester Dawida"
author = "Sylwester Dawida"
version = "1.0.0"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

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
breathe_default_project = "7BitInjector"

exclude_patterns = []


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output
html_title = f"{project} v{version}"
html_theme = "furo"
html_scaled_image_link = False
html_static_path = ["_static"]
html_logo = "_static/7BitInjector-logo.svg"
html_theme_options = {
    "source_repository": "https://github.com/7bitcoder/7bitinjector/",
    "source_branch": "main",
    "source_directory": "Docs/",
}
