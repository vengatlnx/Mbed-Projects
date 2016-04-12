# Mbed-Projects

  ARM mbed is a fastest way of creation and deployment of devices based on ARM
  microcontroller. It is designed for IoT platforms which provides an operating
  system, development tools, standard libraies, cloud services etc.
  
  [www.mbed.org](https://www.mbed.org)

## Setup

   To setup and run Mbed Example Projects

```
  $ git clone https://github.com/vengatlnx/Mbed-Projects.git
```
Note: This repository contains `gcc4mbed` command-line tool for compile and
build mbed applications

### gcc4mbed

   gcc4mbed contains install scripts, mbed SDK libraries and sample programs
   for mbed. [gcc4mbed](https://github.com/adamgreen/gcc4mbed)

   Then run an install script for your specific platform
```
  $ cd Mbed-Projects/gcc4mbed
  $ ./linux_install
```
Note: This will download and install arm-gcc compiler for building mbed
application

### Build

   To make build easier, create a Makefile to compile and build mbed application
   for specific target device
   
```
  $ cd projects/Blinky
  $ cat makefile

  PROJECT         := Blinky
  DEVICES         := LPC1768
  GCC4MBED_DIR    := ../..
  NO_FLOAT_SCANF  := 1
  NO_FLOAT_PRINTF := 1
```

```
  $ make (or)
  $ make DEVICES=<Specific_Targets>
```

  Then flash the output binary from <TARGET_DIR>/Blinky.bin to your specific target
  boards