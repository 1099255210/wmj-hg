# wmj-hg

## CMake 进阶

### find_package 的原理

find_package() 是用来查找依赖包的。它具有两种模式，一种模式是 **Module** 模式，另一种是 **Config** 模式。

Module 模式下，CMake 会优先搜索 `CMAKE_MODULE_PATH` 指定的目录；如果在 `CMakeLists.txt` 中没有设置 `CMAKE_MODULE_PATH`， 就会去搜索 `<CMAKE_ROOT>/share/cmake-x.y/Modules` 这个路径。搜索的内容是一个 `Find<LibraryName>.cmake` 的文件，这个文件负责找到库所在的路径。

如果 Module 模式下没有找到对应的 `Find<LibraryName>.cmake` 文件，就会转入 Config 模式进行搜索。Config 则主要通过 `<LibraryName>Config.cmake` 和`<lower-case-package-name>-config.cmake` 这两个文件引入库。

区分 Module 模式和 Config 模式的方法是，如果在 `find_package()` 中指定了 `CONFIG` 关键字，或者 `NO_MODULE` 关键字，或者使用了不在 MODULE 模式下支持配置的关键字，则为 Config 模式。

### add_library / add_executable / link_directories 的区别

前两个命令的共同点都是生成一个 target，即最终生成的目标。add_library 意为生成一个**库目标**，add_executable 意为生成一个**可执行目标**。

link_directories 主要作用是指定要链接的库文件的路径，可用于指定自己写的动态库文件的路径。

### 如何使用多级的 CMakeLists 管理子目录

```dir
|-bin
    |-test
|-build
    |-CMakeLists.txt
|-include
    |-power.h
|-lib
|-src
    |-CMakeLists.txt
    |-power.cpp
|-main.cpp
```

其中　`/src/CMakeLists.txt` 用来添加库 ，其中用到的是 `add_library`，而 `/build/CMakeLists.txt` 则是编译`main.cpp` 使用 `add_executable` 生成可执行文件 test。

## 串口通信

### 通信协议

通信协议分为**物理层**和**协议层**。物理层规定通讯系统中具有机械、电子功能部分的特性，即硬件部分；协议层规定通讯逻辑，即软件部分。

### 波特率

波特率指数据信号对载波的调制速率，它用单位时间内载波调制状态改变次数来表示，单位为每秒比特数(bps)。波特率越大，传输速率越快。

### 数据位

数据位是通信中真正的有效信息。

### 校验位

校验分为奇校验、偶校验、0 校验、1 校验和无校验。奇校验要求数据包括校验位中有奇数个'1'，偶校验要求数据包括校验位中有偶数个'1'；0 校验要求校验位始终为'0'，1 校验要求校验位始终为'1'。

奇偶校验是为了方便接收方验证发送方在传输过程中所传数据是否因为某些原因造成了破坏的方法。

## Boost 串口通信

### python 写一个虚拟串口

首先引入`pty`库，写下串口创建函数，返回创建的串口：

```py
import pty
import os

def mkpty():
    master, slave = pty.openpty()
    slaveName = os.ttyname(slave)
    print('\nslave names: ', slaveName)
    return master
```

这里的 slaveName 就是串口的名称。在 linux 下貌似是一个路径名(/dev/pts/x)。

主函数中读取串口数据：

```py
if __name__ == "__main__"
    master = mkpty()
    while True:
        r, w, ex = select.select([master], [], [], 0.5)
        for masters in r:
            data = os.read(masters, 128)
            print("read %d data." % len(data), data)
```

其中 r 是可读列表，w 是可写列表，ex 是错误列表，0.5 对应的是时长限制，128 是指一次最多读入 128 个数据。

### 用 boost::asio 写一个串口通信程序

写好上面的虚拟串口之后，需要写一个程序向串口写入数据。利用 boost 库中的 asio 可以实现：

```cpp
    #include<boost/asio.hpp>

    using namespace boost::asio;

int main(){
    int num;
    io_service iosev;

    cout << "input port number:" << endl;
    cin >> num;
    string str = "/dev/pts/" + to_string(num);

    serial_port sp(iosev, str);
    //串口名 /dev/pts/x
    sp.set_option(serial_port::baud_rate(9600));
    //波特率 9600
    sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
    sp.set_option(serial_port::parity(serial_port::parity::none));
    //校验 无
    sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    //停止位 1
    sp.set_option(serial_port::character_size(8));
    //字节大小 8
}
```

手动输入数据确认串口接受数据：

```cpp
    while(1){
        char str[1000]
        cin >> str;
        write(sp, buffer(str, strlen(str)));
    }
```

经测试可用。

## modern C++ 的新特性

### 多线程

多线程部分引入 thread，创建线程，传入参数:

```cpp
#include <iostream>
#include <thread>

using namespace std;
using namespace this_thread;


void print_tid(int num)
{
    cout << num << endl;
    cout << "子线程id为" << get_id() << endl;
}

int main()
{
    for (int i = 0; i < 10; i++)
    {
        thread th(print_tid, i);
        th.join();
    }
    return 0;
}
```

此程序创建了 10 个线程，每个线程输出其 id。这里`th.join()`是一个阻塞主程序的函数，在当前线程未完成时进行等待，直到线程结束再继续运行。

### 右值引用

首先是左值与右值的概念。左值表示一个变量，而右值表示变量所对应的值。从根本上说，左值是可以取地址的值，右值是不能取地址的值。

左值引用，绑定的是返回左值引用的函数、赋值、下标等等。

右值引用，是对临时对象的一种引用，它是在初始化时完成引用的，但并不代表引用临时对象后就不能改变所引对象的值，比如：

```cpp
int &&i = 1;
int n = 2;

cout << i << endl;
i = b;
cout << i << endl;
```

上面代码输出的结果是 1 和 2。

下面引用的方式即为错误的：

```cpp
int &&i = 1;
int &&n = i;
```

i 是一个右值引用变量，而变量都是左值，因此 n 引用左值是违法的。

## 装甲板识别

### 识别装甲板

由于机器的ffmpeg没有装好，折腾了一下午也没弄好。到这里就没做下去。

### 发送至虚拟串口
