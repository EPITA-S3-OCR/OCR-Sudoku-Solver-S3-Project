# OCR Sudoku Solver (2026, S3 Project)

OCR software that outputs a solved grid of sudoku from an image of an unsolved one using image splitting and character recognition (IA).

> [First report](rapport_1.pdf)

**Team members:**  
• Eliott FLECHTNER  
• Titouan VERHILLE  
• Raj Sunil MAHAJAN AKA MALI  
• David Ortiz CALDERON

## How to use

### Librairies

You need to install the following librairies:

- SDL2
- SDL2_image

For example on Linux Debian-based distros install those with :

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

### Compilation

To compile the project, you need to run the following command:

```bash
make # to compile the project
```

### Execution

To execute the project, you need to run the following command:

```bash
# To run the solver
./main solver <path_to_file> # file output under output/file_name.results

# To run the XOR neural network
./main xor-neural-network --train-xor <epoch-number> <path_to_file>
./main xor-neural-network --comp-xor <path_to_file> <binary_number> <binary_number>

# To run the image procesing
./main image-processing <path_to_file>
./main image-rotate <path_to_file> <angle> # to run only manual rotation
./main image-split <path_to_file> # to run only image splitting (from hard-coded values)
```

### Cleaning

To clean the project, you need to run the following command:

```bash
make clean
```
