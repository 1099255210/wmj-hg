#include <iostream>
#include <string.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace boost::asio;

int main()
{
    int num;
    io_service iosev;

    cout << "input port number:" << endl;
    cin >> num;
    string str = "/dev/pts/" + to_string(num);
    // 串口COM1, Linux下为“/dev/ttyS0”
    serial_port sp(iosev, str);
    cout << "write to port:" << endl;
    // 设置参数
    sp.set_option(serial_port::baud_rate(9600));
    sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
    sp.set_option(serial_port::parity(serial_port::parity::none));
    sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    sp.set_option(serial_port::character_size(8));
    // 向串口写数据
    while (1)
    {
        char str[1000];
        cin >> str;
        write(sp, buffer(str, strlen(str)));
    }
    return 0;
}