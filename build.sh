export CC=/usr/bin/clang;
export CXX=/usr/bin/clang++;

mkdir build;
pushd build;
cmake .. && make && ./blog_server;
popd;