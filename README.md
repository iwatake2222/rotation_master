![image](https://user-images.githubusercontent.com/11009876/148997642-8a54899a-8547-44a8-a211-b7d8f4b9b063.png)

# Rotation Master
- Try on a browser: https://iwatake2222.github.io/rotation_master/rotation_master.html
- Provide conversion between the following representations of 3D rotation and visualize a rigid body orientation
    - Rotation Matrix
    - Rotation Vector
    - Axis-angle
    - Quaternion
    - Euler Angles (Intrinsic; Mobile)
    - Euler Angles (Extrinsic; Fixed)

https://user-images.githubusercontent.com/11009876/148988739-5f775a11-c923-4557-aff7-12b9a4811d36.mp4

## CI/CD Status
Status | Details
------ | ----------
[![CMake](https://github.com/iwatake2222/rotation_master/actions/workflows/cmake.yml/badge.svg)](https://github.com/iwatake2222/rotation_master/actions/workflows/cmake.yml) | ubuntu-20.04, windows-2019, macos-10.15
[![CodeQL](https://github.com/iwatake2222/rotation_master/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/iwatake2222/rotation_master/actions/workflows/codeql-analysis.yml) | ubuntu-20.04
[![WASM Deploy](https://github.com/iwatake2222/rotation_master/actions/workflows/release_emscripten.yml/badge.svg)](https://github.com/iwatake2222/rotation_master/actions/workflows/release_emscripten.yml) | https://iwatake2222.github.io/rotation_master/rotation_master.html


## How to Use
https://github.com/iwatake2222/rotation_master/wiki/How-to-Use

## How to Build
https://github.com/iwatake2222/rotation_master/wiki/How-to-Build

# License
- Rotation Master
- Copyright 2022 iwatake2222
- [Licensed under the Apache License, Version 2.0](LICENSE)

# Acknowledgements
- [NOTICE.md](NOTICE.md)
- This project is inspired by 3D Rotation Converter
    - https://www.andre-gaschler.com/rotationconverter/
- @astomih ported this project onto a browser using WASM + Emscripten. I used his/her code as a reference
    - https://github.com/astomih/rotation_master