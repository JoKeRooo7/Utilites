#!/bin/bash
MY=$(./s21_grep for test_1_grep.txt test_2_grep.txt)
ORIGINAL=$(grep for test_1_grep.txt test_2_grep.txt)

echo -e "\033[42m$MY\033[0m"
echo -e "\033[41m$ORIGINAL\033[0m"
