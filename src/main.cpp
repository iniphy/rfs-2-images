#include <iostream>
#include <cxxopts.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>
#include <RecFusion.h>

int main(int argc, char *argv[]) {
    cxxopts::Options options("rfs-2-images", "Converting RecFusion sequence to images");

    options.add_options()
            ("s,seq", "Path to the RecFusion sequence file (*.rfs)", cxxopts::value<std::string>())
            ("n,num", "Number of sensors in the sequence file", cxxopts::value<int>())
            ("o,out", "Out directory for the extracted images", cxxopts::value<std::string>())
            ("h,help", "Print function usage");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
    }
    if (!result.count("seq")) {
        std::cerr << "You must provide path to the RecFusion sequence file (*.rfs)" << std::endl;
        std::cout << options.help() << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string out = ".";
    if (result.count("out")) {
        out = result["out"].as<std::string>();
    }
    int num = 1;
    if (result.count("num")) {
        num = result["num"].as<int>();
        if (num < 1 || num > 3) {
            std::cout << "Wrong number of sensors, backing to num=1 sensor" << std::endl;
            num = 1;
        }
    }
    std::string seq = result["seq"].as<std::string>();

    RecFusion::RFSPlayback rfs;
    if (!rfs.open(seq.c_str())) {
        std::cerr << "" << std::endl;
        exit(EXIT_FAILURE);
    }

    RecFusion::DepthImage depth_image(rfs.depthWidth(), rfs.depthHeight());
    RecFusion::ColorImage color_image(rfs.colorWidth(), rfs.colorHeight());
    std::cout << "Number of frames: " << rfs.frameCount() << std::endl;
    std::cout << "Depth image size (width: " << rfs.depthWidth() << "; height: " << rfs.depthHeight() << ")"
              << std::endl;
    std::cout << "Color image size (width: " << rfs.colorWidth() << "; height: " << rfs.colorHeight() << ")"
              << std::endl;
    for (int k = 0; k < rfs.frameCount(); ++k) {
        if (!rfs.readImage(k, depth_image, color_image)) {
            std::cerr << "Couldn't read frame number " << k << std::endl;
        }
        // Save the images !
        stbi_write_png_16((std::string("depth/frame_") + std::to_string(k) + ".png").c_str(),
                          rfs.depthWidth(), rfs.depthHeight(), 1,
                          depth_image.data(), depth_image.width() * 2);
        stbi_write_png((std::string("color/frame_") + std::to_string(k) + ".png").c_str(),
                       rfs.colorWidth(), rfs.colorHeight(), 3,
                       color_image.data(), color_image.width() * 3);
    }
    rfs.close();
}
