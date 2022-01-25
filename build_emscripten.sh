# apt update
# apt install -y libglu1-mesa-dev mesa-common-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev xorg-dev

# apt install -y git
# git clone https://github.com/emscripten-core/emsdk.git -b 3.1.1
# cd emsdk
# ./emsdk install 3.1.1
# ./emsdk activate 3.1.1
# source /root/emsdk/emsdk_env.sh

rm -rf build_em
mkdir build_em && cd build_em
emcmake cmake ../src/ -DBUILD_TESTS=OFF
make
