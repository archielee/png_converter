# PNG Converter

Converts images stored in a binary file format into PNG images using OpenCV 3

Requires:
- Linux (tested on Ubuntu 16.04)
- OpenCV 3
- Boost 1.58
- cmake (for compilation)

## Compiling the executable
1. Run `cmake .` in the top-level
2. Many files will be generated, among which is a Makefile. Once this file is present, simply run `make` in the repository to compile.
3. An executable should be generated in the top-level folder named "png_converter"

## Usage
The utility currently only works for RGB24 images, 1280x1024. This will be expanded in the future.

Run the executable in the command-line by running
```
$ ./png_converter <list_of_binary_files>

ex:
$ ./png_converter ~/test_images/*.bin
```
The PNG images will be in the same directory as the binary images.
