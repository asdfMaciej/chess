# Chess 
Project created for the "Object-oriented programming" class at Poznań University of Technology.

## Build process
C++14 and above is required. Use CMake to build the project - example command to run in the project root:

```bash
/usr/bin/cmake --build ./build --config Debug --target Chess -j 8 --
```

SFML is used to create the GUI, under Linux / WSL2 you will need to [install additional packages](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php). For Ubuntu 20.04 running under WSL2 I've used the following command:

```bash
sudo apt install libudev-dev libsfml-dev build-essential mesa-common-dev libx11-dev libxrandr-dev libgl1-mesa-dev libglu1-mesa-dev libfreetype6-dev libopenal-dev libsndfile1-dev libxcursor-dev libxi-dev libxinerama-dev xorg-dev
```
*Sidenote: the list contains excessive elements, as the xorg-dev contains X Window System development libraries, which might have been already installed. However, it works.*

In order to run the project under WSL2, install a X11 server on your Windows host machine (such as VcXsrv) and [configure your WSL2 installation](https://stackoverflow.com/questions/61110603/how-to-set-up-working-x11-forwarding-on-wsl2). Upon launching the X11 server, the game ran under WSL will be able to create and display windows. 


### Used C++ features
C++11 features: 
- unique_ptr, shared_ptr
- initalizer lists 
- constructor inheritance

C++14:
- make_unique

## Used OOP techniques
- dependency injection
- inheritance
- interfaces (abstract classes) 
- object factories

## License
The project is licensed under the Apache License, Version 2.0.