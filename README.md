# C Programming Project — Plagiarism Detection | L2 MIDO 2022-2023

This project aims to develop a program capable of detecting plagiarism between multiple C source code files. It is a pedagogical project carried out as part of the second-year MIDO program at Université Paris Dauphine - PSL.

## Project Objectives

- Detect potential plagiarism between two or more C source files.
- Identify similarities even in the case of variable or function renaming, code block rearrangements, or added non-essential content.
- Generate a similarity report and visual `.pgm` image files representing segment similarities.

## Authors

- Wardakhan Kévin
- Dhoomun Yousouf

## Compilation and Execution

To compile and execute the project, use the following commands:

```bash
 gcc -Wall projetc.c -o «  name_of_executable_file  ».out -lm
./ « name_of_executable_file ».out « file_1 » « file_1 » with file_1 and file_2 the two files to test.

