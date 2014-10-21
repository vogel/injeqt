#!/bin/bash

lcov --capture --directory . --output-file coverage.info
lcov --directory . --output-file coverage.info --remove coverage.info "/usr/*" "*.moc" "test/*"
genhtml coverage.info
