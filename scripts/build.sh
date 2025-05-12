rm -rf build; rm -rf install; rm -f ./lib/*
mkdir build
mkdir install
cd build

# cmake -DEIGEN3_INCLUDE_DIR=/home/jlab/github/unpacking/submodules/eigen/cmake ..
# cmake -DCMAKE_INSTALL_PREFIX=../install/ ..
# cmake -DEIGEN_INCLUDE_PATH=/home/jlab/github/unpacking/submodules/eigen/install/include/eigen3 ..
cmake ..
make
make install