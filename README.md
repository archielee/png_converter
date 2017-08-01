# PNG Converter

Converts images stored in a binary file format into PNG images using OpenCV 3

THIS VERSION MAY BE UNSTABLE. A STABLE, SLOWER VERSION EXISTS ON THE VERSION_1 BRANCH

Requires:
- Linux (tested on Ubuntu 16.04)
- OpenCV 3
- Boost 1.58
- cmake (for compilation)

## Compiling the executable
1. Run `cmake .` in the build directory
2. Many files will be generated, among which is a Makefile. Once this file is present, simply run `make` in the build directory to compile
3. An executable should be generated in the top-level folder named "png_converter"

## Usage
The utility currently supports RGB24, RGB32, MONO8, and MONO16 images. I've only tested with images of size 1280x1024, but should theoretically work for other image sizes. This will be expanded in the future.

Run the executable in the command-line
```
$ ./png_converter <options>

help menu:
$ ./png_converter -h
```
The PNG images will be in the same directory as the binary images.
