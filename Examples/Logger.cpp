#include <SevenBit/DI.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace sb::di;

struct ILogger
{
    virtual void log(const std::string &message) = 0;

    virtual ~ILogger() = default;
};

struct StdLogger final : ILogger
{
    void log(const std::string &message) override { std::cout << message << std::endl; }
};

struct FileLogger final : ILogger
{
    std::ofstream logFile{"log.txt"};

    void log(const std::string &message) override { logFile << message << std::endl; }
};

struct VoidLogger final : ILogger
{
    void log(const std::string &message) override {}
};

class Application
{
    ILogger &_logger;

  public:
    explicit Application(ILogger &logger) : _logger(logger) {}

    int run()
    {
        _logger.log("Initializing");
        init();
        _logger.log("Processing");
        process();
        _logger.log("Finalizing");
        finalize();
        return 0;
    }

  private:
    void init() {}
    void process() {}
    void finalize() {}
};

void addLogger(const std::string_view loggerName, ServiceCollection &services)
{
    if (loggerName == "fileLogger")
    {
        services.addSingleton<ILogger, FileLogger>();
    }
    else if (loggerName == "voidLogger")
    {
        services.addSingleton<ILogger, VoidLogger>();
    }
    else
    {
        services.addSingleton<ILogger, StdLogger>();
    }
}

int main(int argc, char *argv[])
{
    ServiceCollection services;
    const auto loggerName = argc > 1 ? argv[1] : "stdLogger";
    addLogger(loggerName, services);
    services.addSingleton<Application>();

    ServiceProvider provider = services.buildServiceProvider();

    auto &app = provider.getService<Application>();
    return app.run();
}
