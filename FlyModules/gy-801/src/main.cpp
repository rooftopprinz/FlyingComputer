#include <iostream>
#include <memory>
#include <regex>
#include <Logger.hpp>
#include <IHwApi.hpp>
#include <App.hpp>

int main(int argc, const char* argv[])
{
    std::regex arger("^--(.+?)=(.+?)$");
    std::smatch match;
    std::map<std::string, std::string> options; 

    for (int i=1; i<argc; i++)
    {
        auto s = std::string(argv[i]);
        if (std::regex_match(s, match, arger))
        {
            options.emplace(match[1].str(), match[2].str());
        }
        else
        {
            throw std::runtime_error(std::string("invalid argument: `") + argv[i] + "`");
        }
    }

    Logger::getInstance().logless();

    std::unique_ptr<net::IUdpFactory> udpFactory = std::make_unique<net::UdpFactory>();
    app::Args args(options);
    hwapi::setup();
    app::App app(*udpFactory, args);
    return app.run();
}