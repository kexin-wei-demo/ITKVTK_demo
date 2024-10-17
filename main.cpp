#include <iostream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
namespace Log = spdlog;

#include "TestReadImageDirection.h"
#include "TestvtkImageConnectivityFilter.h"
void setUpSpdlog()
{
    try
    {
        Log::set_level(Log::level::debug);
        Log::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
        Log::info("Logging system initialized.");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

int main()
{
    setUpSpdlog();

    // testReadImageDirection();
    testvtkImageConnectivityFilter();
    return 0;
}