# Implementation of Pairwise Distance Calculator in C

## Overview

This is a sample implementation of pairwise distance calculation in C. It gives good performance, but never optimal. You may turn on ```//#define TIME_COUNT 1``` in ```pdist.c``` to see the ratio of time on matrix multiplication and unnecessary vector operations. ```1-[the ratio]``` indicates the improvement potential by fusing vector operations into GEMM kernel.

## Syntax
```
Input   
        float *X : m - by - k
        float *Y : n - by - k
        float *D : m - by - n
        int m
        int n
        int k
Output
        D : m - by - n
```

## Requirement
* Unix-like system (Mac OS or Linux - CentOS/RedHat/Ubuntu)

## How to build
*On Mac system*
```
$ wget https://github.com/intel/mkl-dnn/releases/download/v0.18/mklml_mac_2019.0.3.20190220.tgz
$ tar -xvzf mklml_mac_2019.0.3.20190220.tgz
$ mv -f mklml_mac_2019.0.3.20190220 mkldnn
$ rm *.tgz
$ mkdir build
$ cd build
$ rm -rf * && cmake .. && make
```
*On Linux system*
```
$ wget https://github.com/intel/mkl-dnn/releases/download/v0.18/mklml_lnx_2019.0.3.20190220.tgz
$ tar -xvzf mklml_lnx_2019.0.3.20190220.tgz
$ mv -f mklml_lnx_2019.0.3.20190220 mkldnn
$ rm *.tgz
$ mkdir build
$ cd build
$ rm -rf * && cmake .. && make
```

## How to run
Now you have the binary ***mypdist***, we can run it. Before that, you should add the absolute path of dynamic libraries you link to environment path. For example, on my labtop, the absolute path is
```
YujiamatoMBP:lib yujiazhai$ pwd
/Users/yujiazhai/Desktop/pairwise-dist/mkldnn/lib
```
We can just add it to env path by
```
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/Users/yujiazhai/Desktop/pairwise-dist/mkldnn/lib
```
Note that this should be done <strong>EVERY</strong> time you log in, unless you write it into ```.bashrc``` (Linux) or ```.bash_profile``` (Mac).

Now let us run the code.
```
$ ./mypdist [m] [n] [k]
```
For example, you want to calculate the pairwise distance between two dataset, X (10000 - by - 3) and Y (20000 - by - 3), just type in:
```
$ ./mypdist 10000 20000 3
```

That's it, good luck!