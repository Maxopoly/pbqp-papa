if [ ! -f CMakeLists.txt ]; then
    cd ..
fi

cmake CMakeLists.txt
make clean
make
ctest -V


