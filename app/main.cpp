#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <conio.h>
#include <httplib.h>

int main(int argc, char *argvs[])
{
    // HTTPS
    httplib::Client cli("https://yhirose.github.io");

    if (auto res = cli.Get("/hi"))
    {
        res->status;
        res->body;
    }
    std::cout << "Press any key to exit...";
    _getch();
    return 0;
}
