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

class CliHandler
{
    std::unordered_map<std::string, IOptionHandler *> _options;

  public:
    explicit CliHandler(std::vector<IOptionHandler *> optionHandlers)
    {
        for (const auto handler : optionHandlers)
        {
            _options[handler->getName()] = handler;
        }
    }

    void handle(const int argc, char *argv[])
    {
        for (int i = 1; i < argc; ++i)
        {
            auto [name, value] = parseOption(argv[i]);
            if (auto it = _options.find(name); it != _options.end())
            {
                const auto handler = it->second;
                handler->handle(value);
            }
        }
    }

  private:
    static std::pair<std::string, std::string> parseOption(const std::string &option)
    {
        int idx = eatDashes(option);
        return {getName(idx, option), getValue(idx, option)};
    }

    static int eatDashes(const std::string &option)
    {
        if (option.size() > 1 && option[0] == '-' && option[1] == '-')
        {
            return 2;
        }
        throw std::runtime_error("wrong option scheme use -- before each options");
    }

    static std::string getName(int &index, const std::string &option)
    {
        const int begin = index;
        while (index < option.size() && option[index] != '=')
        {
            index++;
        }
        auto name = option.substr(begin, index - begin);
        if (name.empty())
        {
            throw std::runtime_error("option cannot be empty");
        }
        return name;
    }

    static std::string getValue(int &index, const std::string &option)
    {
        if (index < option.size() && option[index] == '=')
        {
            index++;
        }
        return option.substr(index);
    }
};

class Application
{
    CliHandler &_cliHandler;

  public:
    explicit Application(CliHandler *cliHandler) : _cliHandler(*cliHandler) {}

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
    const ServiceProvider::Ptr provider = ServiceCollection{}
                                              .addSingleton<IOptionHandler, HelpOption>()
                                              .addSingleton<IOptionHandler, InfoOption>()
                                              .addSingleton<IOptionHandler, NumberOption>()
                                              .addSingleton<CliHandler>()
                                              .addScoped<Application>()
                                              .buildServiceProvider();

    const Application &app = provider->getService<Application>();
    return app.run(argc, argv);
}
