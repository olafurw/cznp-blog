export CC=/usr/bin/clang;
export CXX=/usr/bin/clang++;

mkdir www;
pushd www;
npm run build;
popd;

mkdir build;
pushd build;
cmake .. && make && ./blog_server;
popd;