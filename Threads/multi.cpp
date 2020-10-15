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