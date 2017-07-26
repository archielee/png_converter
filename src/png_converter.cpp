#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>

#define DEBUG               0

#define DEFAULT_IMG_W       1280
#define DEFAULT_IMG_H       1080
#define DEFAULT_NUM_CHANNEL 3

const std::vector<int> COMPRESSION_PARAMS = {cv::IMWRITE_PNG_COMPRESSION, 0};

namespace fs = boost::filesystem;
namespace po = boost::program_options;

enum Status {
    SUCCESS     = 0,
    EARLY_EXIT  = 1,
    FAILURE     = 2,
};

enum Img_Type {
    RGB24   = 0,
    RGB32   = 1,
    MONO8   = 2,
    MONO16  = 3,
    UNKNOWN = 4,
};


Status argument_parser(int argc, char** argv, po::variables_map& vm) {
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",
         "Print help messages")
        ("file,f", po::value<std::vector<std::string> >()->multitoken()->required(),
         "Input images for conversion (stored in binary format)")
        ("size,s", po::value<std::string>(),
         "Input image size in format 'WIDTHxHEIGHT' (defaults to 1280x1024)")
        ("type,t", po::value<std::string>(),
         "Image type. Available options are: rgb24 (defaults to rgb24)");

    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

        // help menu
        if (vm.count("help"))
        {
            std::cout << "PNG Converter" << std::endl
                      << "=============" << std::endl << std::endl
                      << "Converts images stored in a binary format into PNG "
                         "images" << std::endl << std::endl
                      << desc << std::endl;
            return Status::EARLY_EXIT;
        }

        po::notify(vm);
    }
    catch(po::error& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return Status::FAILURE;
    }

    return Status::SUCCESS;
}


Img_Type get_image_type(std::string type) {
    Img_Type img_type;

    if (type == std::string("rgb24")) {
        img_type = Img_Type::RGB24;
    }
    else if (type == std::string("rgb32")) {
        img_type = Img_Type::RGB32;
    }
    else if (type == std::string("mono8")) {
        img_type = Img_Type::MONO8;
    }
    else if (type == std::string("mono16")) {
        img_type = Img_Type::MONO16;
    }
    else {
        img_type = Img_Type::UNKNOWN;
    }

    return img_type;
}


std::vector<char> open_file(std::string filename) {
    std::vector<char> img_buffer;
    std::fstream in_file(filename.c_str(),
                         std::fstream::in | std::fstream::binary);

    // open file and copy contents into char vector
    if (in_file) {
        // check length of file
        in_file.seekg(0, in_file.end);
        int length = in_file.tellg();
        in_file.seekg(0, in_file.beg);

        img_buffer.resize(length);

        in_file.read(&img_buffer[0], length);
        in_file.close();
    }

    return img_buffer;
}


void compress_image(std::string filename, int img_w, int img_h,
                    Img_Type img_type) {

    std::vector<char> img_data = open_file(filename);

    // create new file path
    std::string png_path = filename;
    png_path.erase(png_path.end() - 4, png_path.end());
    png_path.append(".png");

    // set image parameters
    int mat_type, num_ch;
    switch (img_type) {
        case Img_Type::RGB24:
            mat_type = CV_8UC3;
            num_ch = 3;
            break;

        case Img_Type::RGB32:
            mat_type = CV_8UC4;
            num_ch = 3;
            break;

        case Img_Type::MONO8:
            mat_type = CV_8UC1;
            num_ch = 1;
            break;

        case Img_Type::MONO16:
            mat_type = CV_16UC1;
            num_ch = 1;
            break;

        default:
            mat_type = CV_8UC3;
            num_ch = 3;
            break;
    }

    // copy vector data into Mat
    char buffer[img_w * img_h * num_ch];
    std::copy(img_data.begin(), img_data.end(), buffer);
    cv::Mat cv_mat = cv::Mat(img_h, img_w, mat_type, buffer);

    // write into image file
    if (cv::imwrite(png_path, cv_mat, COMPRESSION_PARAMS)) {
#if DEBUG
        std::cout << "Saved: " << png_path << std::endl;
#endif
        return;
    }
    std::cout << "imwrite() failed: " << png_path << std::endl;

    return;
}


int main (int argc, char** argv) {
    po::variables_map vm;

    Status res = argument_parser(argc, argv, vm);

    if (res == Status::EARLY_EXIT) {
        return Status::EARLY_EXIT;
    }
    else if (res == Status::FAILURE) {
        return Status::FAILURE;
    }

    std::vector<std::string> files = vm["file"].as<std::vector<std::string> >();

    size_t img_w = DEFAULT_IMG_W;
    size_t img_h = DEFAULT_IMG_H;
    Img_Type img_type = Img_Type::RGB24;

    // parse size argument
    if (vm.count("size")) {
        std::string size_str = vm["size"].as<std::string>();

        size_t pos = size_str.find("x");
        img_w = std::stoi(size_str.substr(0, pos));
        img_h = std::stoi(size_str.substr(pos + 1, size_str.length()));
    }

    // parse image type argument
    if (vm.count("type")) {
        std::string type = vm["type"].as<std::string>();

        img_type = get_image_type(type);
        // invalid type
        if (img_type == Img_Type::UNKNOWN) {
            std::cout << "Unknown image type: " << type << std::endl;
            return Status::FAILURE;
        }
    }

    // process files
    for (size_t i = 0; i < files.size(); i++) {
        // check file extension
        if (fs::path(files[i]).extension() == ".bin") {
#if DEBUG
            std::cout << "Processing " << files[i] << std::endl;
#endif

            compress_image(files[i], img_w, img_h, img_type);
        }
        else {
            std::cout << "Invalid file (" << files[i] << "): only supports "
                         ".bin files" << std::endl;
        }
    }

    std::cout << "Images converted successfully!" << std::endl;

    return Status::SUCCESS;
}
