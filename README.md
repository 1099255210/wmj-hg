# wmj-hg

## CMake 进阶

### find_package 的原理

find_package() 是用来查找依赖包的。它具有两种模式，一种模式是 Module 模式，另一种是 Config 模式。

Module 模式下，CMake 会优先搜索 CMAKE_MODULE_PATH 指定的目录；如果在 CMakeLists.txt 中没有设置 CMAKE_MODULE_PATH， 就会去搜索 <CMAKE_ROOT>/share/cmake-x.y/Modules 这个路径。

### add_library / add_executable / link_directories 的区别

### 如何手动链接一个 OpenCV 库

### 如何使用多级的 CMakeLists 管理子目录

## Python

## 串口通信

### 通信协议

### 波特率

### 数据位

### 校验位

### ...

## Boost 串口通信

### python写一个虚拟串口

### 用 boost::asio 写一个串口通信程序

## modern C++ 的新特性

### 多线程

### 右值引用

## 装甲板识别

### 识别装甲板

### 发送至虚拟串口

