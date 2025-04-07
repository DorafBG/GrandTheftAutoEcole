# Skeleton code for OpenGL using C++

This is the skeleton code for creating graphics applications using OpenGL and C++.

# Installation

You would need CMake(to build our project) which can be downloaded from [here](https://cmake.org/download/).

# Setup

After cloning this repo, you also need to load the submodules GLEW, GLFW and GLM using the following command.

    git submodule update --init --recursive


## Run the Application

To launch the window, simply run:

    cmake .
    make
    ./opengl_program
