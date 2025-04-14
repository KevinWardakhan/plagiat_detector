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
```

## Project Structure
```bash
├── projetc.c               # Main program source code
├── dice.pgm                # Grayscale image representing segment similarities (Dice coefficient)
├── coupling.pgm            # Grayscale image for segment coupling visualization
├── filtering.pgm           # Post-filtering visualization
├── README.md               # Project documentation
├── dev.pdf                 # Development report: design choices, algorithms, complexity, challenges
└──exp.pdf                  # Experiment report: tests and results
```
## Features
Pre-processing:
Removes comments, string literals, spaces, and replaces identifiers with placeholder characters to neutralize variable renaming.

Segmentation:
Splits the code into segments (lines) for granular comparison.

Similarity Calculation:
Computes segment similarities using Dice’s coefficient (bi-grams comparison).

Greedy Coupling:
Pairs segments to minimize total distance.

Post-Filtering:
Eliminates isolated matches to focus on significant, contiguous similarities.

Visualization:
Generates .pgm images for visual analysis of detected similarities.

## Output
Final plagiarism score: a number between 0 and 1 (two decimal places).

Visual files (.pgm) representing segment similarities, coupling, and filtering.



