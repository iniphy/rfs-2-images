# rfs-2-images

Convert a single-file RecFusion sequence (`*.rfs`) to a collection of depth and colour PNG images. This program doesn't require a RecFusion license to work. However, you need the license to obtain RecFusion sequence files.

Also, because [RecFusion SDK](https://www.recfusion.net/documentation/index.html) works only on Windows, this program requires Windows as well.

## Installation and running

* Clone the repository and load submodules: `git submodule update --init --recursive`.
* Run cmake and provide the path to RecFusion SDK `cmake CMakeLists.txt -DRECFUSION_PATH="..."` (VS is the recommended generator, e.g. `-G "Visual Studio 16 2019"`).
* Compile the program.
* Run it by providing the path to the `*.rfs` sequence: `rfs-2-images -seq 0.rfs`.
* The images will land to `color/` and `depth/` directories.
