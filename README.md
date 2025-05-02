# FauxtoShop — BMP Image Rotator in C

This project implements a lightweight C program that reads a BMP image and outputs a new BMP rotated 90° clockwise — no fancy GUI, no libraries, just raw performance.

---

## 🚀 Features

✅ Reads 24-bit BMP files directly from `stdin`  
✅ Writes rotated BMP to `stdout`  
✅ Handles BMP headers, pixel arrays, and padding manually  
✅ Uses dynamic memory allocation for flexible image sizes  
✅ Runs fast and completes within 10 seconds for large images

---

## 🏗 How to Build

Compile using:
```bash
gcc -O3 -Wall -Wextra -Werror -pedantic -o fauxtoshop fauxtoshop.c
```
🏃 How to Run
Basic usage (rotate one image):
```bash
./fauxtoshop < input.bmp > output.bmp
```
Chain rotations (rotate 360° in four steps):
```bash
./fauxtoshop < input.bmp | ./fauxtoshop | ./fauxtoshop | ./fauxtoshop > output.bmp
```
## 📊 Technical Details
  -  Assumes 24-bit BMP format (monochrome not supported)

  -  Reads BMP headers (≥ 54 bytes) and preserves non-pixel data

  -  Calculates and applies row padding as per BMP format

  -  Rearranges pixel data to rotate the image without external libraries
## 🌍 Why This Matters
While most people use graphical editors like Photoshop, handling image transformations at the binary level builds skills in:

  -  Low-level file operations

  -  Memory management

  -  Working with binary formats and offsets

  -  Performance-oriented programming
##💡 Future Ideas
  -  Support additional transformations (e.g., mirror, flip, scale)

  -  Add a command-line interface for specifying transformations

  -  Build a graphical frontend using a library like SDL or OpenGL
  -  
# 🛠 Author
Andreas Hadjitofis

Computer Science Student @ National and Kapodistrian University of Athens


This project showcases C programming applied to real-world systems tasks.

