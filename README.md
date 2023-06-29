# Raymarcher
A fun little SDF/DDA based renderer written in GLSL/C++

## Screenshots
![Screenshot_20230629_160104](https://github.com/n0tcnj1eck4h/raymarcher/assets/48856951/d1753c58-d894-411d-9ff3-68f9d0794887)
![Screenshot_20230629_160217](https://github.com/n0tcnj1eck4h/raymarcher/assets/48856951/dbd45f4a-6e56-4a31-b492-17b5254e5602)
![Screenshot_20230629_160333](https://github.com/n0tcnj1eck4h/raymarcher/assets/48856951/98872aba-db60-412d-a995-5cc4a9175c41)

## Building
The required libraries are SDL2, glbinding and glm. They need to be installed on your system in one way or another.
```
$ cmake --B build -S .
$ cmake --build build
$ ./build/raymarcher
```
