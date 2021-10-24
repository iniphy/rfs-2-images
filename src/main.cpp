#include <iostream>
#include <cxxopts.hpp>

int main(int argc, char *argv[]) {
    cxxopts::Options options("rfs-2-images", "Converting RecFusion sequence to images");

    options.add_options()
            ("s,seq", "Path to the RecFusion sequence file (*.rfs)", cxxopts::value<std::string>())
            ("o,out", "Out directory for the extracted images", cxxopts::value<std::string>())
            ("h,help", "Print function usage");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    if (!result.count("seq")) {
        std::cerr << "You must provide path to the RecFusion sequence file (*.rfs)" << std::endl;
        std::cout << options.help() << std::endl;
        exit(0);
    }
    std::string out = ".";
    if (result.count("out")) {
        out = result["out"].as<std::string>();
    }
    std::string seq = result["seq"].as<std::string>();

    std::cout << "seq=" << seq << "; out=" << out << std::endl;
}