#include <SevenBit/DI.hpp>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace sb::di;

struct IOptionHandler
{
    virtual std::string getName() = 0;

    virtual void handle(const std::string &value) = 0;

    virtual ~IOptionHandler() = default;
};

struct HelpOption final : IOptionHandler
{
    std::string getName() override { return "help"; }

    void handle(const std::string &value) override { std::cout << "help commands ..." << std::endl; }
};

struct InfoOption final : IOptionHandler
{
    std::string getName() override { return "info"; }

    void handle(const std::string &value) override { std::cout << "some info ..." << std::endl; }
};

struct NumberOption final : IOptionHandler
{
    std::string getName() override { return "number"; }

    void handle(const std::string &value) override { std::cout << "number is: " << std::stoi(value) << std::endl; }
};

struct IOptionParser
{
    virtual std::pair<std::string, std::string> parse(const std::string &option) = 0;

    virtual ~IOptionParser() = default;
};

struct OptionParser final : IOptionParser
{
    std::pair<std::string, std::string> parse(const std::string &option) override
    {
        if (const auto splitIndex = option.find_first_of('='); splitIndex != std::string::npos)
        {
            return {option.substr(0, splitIndex), option.substr(splitIndex + 1)};
        }
        return {option, ""};
    }
};

struct ICliHandler
{
    virtual void handle(int argc, char *argv[]) = 0;

    virtual ~ICliHandler() = default;
};

class CliHandler final : public ICliHandler
{
    IOptionParser &_optionParser;
    std::unordered_map<std::string, IOptionHandler *> _options;

  public:
    explicit CliHandler(IOptionParser &optionParser, std::vector<IOptionHandler *> optionHandlers)
        : _optionParser(optionParser)
    {
        for (const auto handler : optionHandlers)
        {
            _options[handler->getName()] = handler;
        }
    }

    void handle(const int argc, char *argv[]) override
    {
        for (int i = 1; i < argc; ++i)
        {
            auto [name, value] = _optionParser.parse(argv[i]);
            if (auto it = _options.find(name); it != _options.end())
            {
                const auto handler = it->second;
                handler->handle(value);
            }
            else
            {
                std::cout << "Unrecognized option: " << argv[i] << std::endl;
            }
        }
    }
};

class Application
{
    ICliHandler &_cliHandler;

  public:
    explicit Application(ICliHandler &cliHandler) : _cliHandler(cliHandler) {}

    int run(const int argc, char *argv[]) const
    {
        try
        {
            _cliHandler.handle(argc, argv);
        }
        catch (std::exception &e)
        {
            std::cout << e.what();
        }
        return 0;
    }
};

int main(int argc, char *argv[])
{
    ServiceProvider provider = ServiceCollection{}
                                   .addSingleton<IOptionHandler, HelpOption>()
                                   .addSingleton<IOptionHandler, InfoOption>()
                                   .addSingleton<IOptionHandler, NumberOption>()
                                   .addSingleton<IOptionParser, OptionParser>()
                                   .addSingleton<ICliHandler, CliHandler>()
                                   .addScoped<Application>()
                                   .buildServiceProvider();

    const Application &app = provider.getService<Application>();
    return app.run(argc, argv);
}
