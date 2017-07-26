#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>

#define IMG_W       1280
#define IMG_H       1080
#define NUM_CHANNEL 3

const std::vector<int> COMPRESSION_PARAMS = {cv::IMWRITE_PNG_COMPRESSION, 0};

std::vector<char> open_file(std::string filename) {
    std::vector<char> img_buffer;
    std::fstream in_file(filename.c_str(), std::fstream::in | std::fstream::binary);

    if (in_file) {
        // check length of file
        in_file.seekg (0, in_file.end);
        int length = in_file.tellg();
        in_file.seekg (0, in_file.beg);

        img_buffer.resize(length);

        in_file.read(&img_buffer[0], length);
        in_file.close();
    }

    return img_buffer;
}

void compress_image(std::vector<char> &img_data, std::string filename) {
    // create new file path
    std::string png_path = filename;
    png_path.erase(png_path.end() - 4, png_path.end());
    png_path.append(".png");

    // copy vector data into Mat
    char buffer[IMG_W * IMG_H * NUM_CHANNEL];
    std::copy(img_data.begin(), img_data.end(), buffer);
    cv::Mat cv_mat = cv::Mat(IMG_H, IMG_W, CV_8UC3, buffer);

    if (cv::imwrite(png_path, cv_mat, COMPRESSION_PARAMS)) {
        std::cout << "Saved image to: " << png_path << std::endl;
        return;
    }
    std::cout << "Function imwrite() failed: " << png_path << std::endl;
    return;
}


int main (int argc, char** argv) {
    // loop over all files passed in (except first file, which is itself)
    for (int i = 1; i < argc; i++) {
        // check file extension
        if (boost::filesystem::path(argv[i]).extension() == ".bin") {
            std::cout << "Processing " << argv[i] << std::endl;

            std::vector<char> img_data = open_file(argv[i]);

            compress_image(img_data, argv[i]);
        }
        else {
            std::cout << "Invalid file (" << argv[i] << "): only supports .bin files" << std::endl;
        }
    }

    return 0;
}
