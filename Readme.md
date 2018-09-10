# OpenGL ES 2.0

This repository contains examples from the book "OpenGL(R) ES 2.0 Programming Guide", adapted to Run on Raspberry Pi 1, 2 and GNU/Linux PC (32 and 64 bits).

## How to build the examples

To build the examples the following packages are required on GNU/Linux PC:

 - gcc
 - make
 - build-essential (in debian based distros)
 - tcc (fast compiler)
 - libgles2-mesa-dev
 - git

~~~
sudo apt install tcc git libgles2-mesa-dev build-essential make gcc
~~~

In case to build on Raspberry Pi (1,2), only need the following packages:

 - tcc
 - git

~~~
sudo apt install tcc git
~~~

Now build with the following instructions:

### 1.-Get the source

Clone the Git repository with:

~~~
git clone https://github.com/ecalvadi/opengles2.git
~~~

### 2.-Build

Enter to the new folder and compile:

~~~
cd opengles2/
make
~~~

For build an optimized binary run (Generally takes more time to build).

~~~
make release
~~~

The binary are saved on "bin" folder. To run type in the console the following example:

~~~
./binary_to_run
~~~

Please, send me an email to improve this repository: hector@eclipsedpixel.com 
