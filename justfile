# default target
default: build

# run cmake build
build:
    cmake --build build

# cmake configure
setup:
    cmake --preset Debug

# show this information
help:
    @just --list

# run pre-commit
pre-commit:
    pre-commit run -a
