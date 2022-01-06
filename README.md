# ---
# Under development
# ---


# Rotation Master
- Provide conversion between the following representations of 3D rotation and display the pose
    - Rotation Matrix
    - Quaternion
    - Axis-angle
    - Axis with angle magnitude
    - Euler Angle (Intrinsic(Mobile))
    - Euler Angle (Extrinsic(Fixed))

![](https://user-images.githubusercontent.com/11009876/148202305-43d7d04e-b680-4f58-99ce-03cf6f6d0dd0.jpg)

## CI Status
Status | Build Type
------ | ----------
[![CMake](https://github.com/iwatake2222/rotation_master/actions/workflows/cmake.yml/badge.svg)](https://github.com/iwatake2222/rotation_master/actions/workflows/cmake.yml) | ubuntu-20.04, windows-2019, macos-10.15
[![CodeQL](https://github.com/iwatake2222/rotation_master/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/iwatake2222/rotation_master/actions/workflows/codeql-analysis.yml) | ubuntu-latest

## How to Run
### Desktop
ToDo

### Browser
ToDo

## How to Build
### Desktop
```
docker create -it -v /mnt/c/iwatake/devel:/root/devel -v /etc/localtime:/etc/localtime:ro -e DISPLAY="192.168.1.2:0" -w /root/ -p 8888:8888  --name=ubuntu20_rotation_master ubuntu:20.04
docker start ubuntu20_rotation_master
docker exec -it ubuntu20_rotation_master bash

apt update
apt install -y cmake g++
apt install -y libglu1-mesa-dev mesa-common-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
cd <path-to-rotation_master>/desktop/
mkdir -p build && cd build
cmake ..
make -j4
./rotation_master
```


# License
- Rotation Master
- Copyright 2022 iwatake2222
- [Licensed under the Apache License, Version 2.0](LICENSE)

# Acknowledgements
- [NOTICE.md](NOTICE.md)
- This project is inspired by 3D Rotation Converter
    - https://www.andre-gaschler.com/rotationconverter/
    - https://github.com/gaschler/rotationconverter
