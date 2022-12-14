# OCR Sudoku Solver (2026, S3 Project)

OCR software that outputs a solved grid of sudoku from an image of an unsolved one using image splitting and character recognition (IA).

> [First report](rapport_1.pdf)

> [Final report](rapport_2.pdf)

**Team members:**  
• Eliott FLECHTNER  
• Titouan VERHILLE  
• Raj Sunil MAHAJAN AKA MALI  
• David Ortiz CALDERON

## How to use

### Librairies

You need to install the following librairies:

- [SDL2](https://wiki.libsdl.org/SDL2/Installation)
- [SDL2_image](https://wiki.libsdl.org/SDL_image/FrontPage)
- [GTK3](https://www.gtk.org/docs/installations/linux)
- [ImageMagick](https://imagemagick.org/script/install-source.php)

For example on Linux Debian-based distros install those with :

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libgtk-3-dev libmagickwand-dev
```

### Compilation

To compile the project, you need to run the following command:

```bash
make # To compile the project
```

### Execution

To execute the project, you need to run the following command:

```bash
./main ui # Will launch a user interface
```

### Cleaning

To clean the project, you need to run the following command:

```bash
# To clean the project
make clean
# To clean the output folder
make clean-output
```

### Create documentation

You will need the doxygen package to create the documentation.

For example on Linux Debian-based distros install those with :

```bash
sudo apt-get install doxygen
```

To create the documentation, you need to run the following command:

```bash
# To create the documentation
make doc
# To clean the documentation
make clean-doc
```

Open the [`doc/html/index.html`](doc/html/index.html) file in your browser to see the documentation.
