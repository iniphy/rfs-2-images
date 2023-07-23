#include <iostream>
#include <string>
#include <cxxopts.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <RecFusion.h>
#include <PngIO.h>

using namespace RecFusion;
using namespace std;

int main(int argc, char *argv[]) {
    // Turn off compression
    stbi_write_png_compression_level = 0;
    RecFusion::RecFusionSDK::init();
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

    std::string seq = result["seq"].as<std::string>();
    std::cout << "Seq file=" << seq << std::endl;

    RecFusion::RFSPlayback rfs;
    if (!rfs.open(seq.c_str())) {
        std::cerr << "Can't open the sequence" << std::endl;
        exit(EXIT_FAILURE);
    }
    int num = rfs.frameCount();
    if (result.count("num")) {
        num = result["num"].as<int>();
        if (num < 0 || num > rfs.frameCount()) {
            std::cout << "Wrong number of frames to read. Reading them all." << std::endl;
            num = rfs.frameCount();
        }
    }
    std::cout << "Number of frames: " << rfs.frameCount() << std::endl;
    std::cout << "Reading frames: " << num << std::endl;
    std::cout << "Depth image size (width: " << rfs.depthWidth() << "; height: " << rfs.depthHeight() << ")"
              << std::endl;
    std::cout << "Color image size (width: " << rfs.colorWidth() << "; height: " << rfs.colorHeight() << ")"
              << std::endl;
    RecFusion::DepthImage depth_image(rfs.depthWidth(), rfs.depthHeight());
    RecFusion::ColorImage color_image(rfs.colorWidth(), rfs.colorHeight());
    for (int k = 0; k < num; ++k) {
        if (!rfs.readImage(k, depth_image, color_image)) {
            std::cerr << "Couldn't read frame number " << k << std::endl;
            continue;
        }
        // Save the images !
        PngIO::writeImage((std::string("color/frame_") + std::to_string(k) + ".png").c_str(), color_image.data(),
                          color_image.width(), color_image.height(), 3, 0, 0);
        unsigned char buffer[640 * 480 * 2] = {0};
        union conv {
            uint16_t uint;
            unsigned char uc[2];
        };
        int conv_idx = 0;
        for (int i = 0; i < 640 * 480; ++i) {
            uint16_t tmp = static_cast<uint16_t>(depth_image.data()[i]);
            conv c{tmp};
            buffer[conv_idx] = c.uc[1];
            buffer[conv_idx + 1] = c.uc[0];
            conv_idx += 2;
        }
        PngIO::writeImage((std::string("depth/frame_") + std::to_string(k) + ".png").c_str(), buffer,
                          depth_image.width(), depth_image.height(), 1, 1, 0);

//        stbi_write_png_16((std::string("depth/frame_") + std::to_string(k) + ".png").c_str(),
//                          rfs.depthWidth(), rfs.depthHeight(), 1,
//                          depth_image.data(), depth_image.width() * 2);

//        stbi_write_png((std::string("color/frame_") + std::to_string(k) + ".png").c_str(),
//                       rfs.colorWidth(), rfs.colorHeight(), 3,
//                       color_image.data(), color_image.width() * 3);
    }
    rfs.close();
    RecFusion::RecFusionSDK::deinit();
}
