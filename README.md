# SimpleGrid

Debugging grid with SDL and OpenGL, this helps to visualize the 3D
environment, camera is a simple floating implementation.

Implementation taken from this [post](https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/) (thank you Marie!).

![image](https://github.com/user-attachments/assets/c23eae52-2622-4c5d-991b-27e81229053c)

## Dependencies

* SDL 3
* Linux / Unix (-fm actually, to compile for windows edit CMakeLists file and remove m lib)

## How to compile

Pull submodules `git submodule update --recursive` then:

```shell
mkdir cmake-build-debug
cmake -B cmake-build-debug -S .
```
