#include <SevenBit/DI.hpp>
#include <iostream>
#include <utility>

using namespace sb::di;

enum class Permission : int
{
    NONE = 0,
    READ = 1,
    UPDATE = 2,
};

Permission operator|(Permission lhs, Permission rhs)
{
    return static_cast<Permission>(static_cast<int>(lhs) | static_cast<int>(rhs));
}
Permission operator&(Permission lhs, Permission rhs)
{
    return static_cast<Permission>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

struct IUserPermission
{
    virtual Permission getPermissions(int userId) = 0;

    virtual ~IUserPermission() = default;
};

struct UserPermission final : IUserPermission
{
    // normally fetch from database
    Permission getPermissions(int userId) override { return Permission::READ; }
};

struct AdminPermission final : IUserPermission
{
    Permission getPermissions(int userId) override { return Permission::READ | Permission::UPDATE; }
};

struct Data
{
    std::string name;
    std::string payload;
};

struct IDataService
{
    virtual const Data &getData() = 0;
    virtual const Data &updateData(Data &&data) = 0;

    virtual ~IDataService() = default;
};

// normally service would interact with database
class DataService final : public IDataService
{
    IUserPermission &_userPermission;
    Data _data = {"test name", "It is a long established fact that a reader will be distracted"};

  public:
    explicit DataService(IUserPermission &userPermission) : _userPermission(userPermission) {}

    const Data &getData() override
    {
        assertPermission(Permission::READ);
        return _data;
    }

    const Data &updateData(Data &&data) override
    {
        assertPermission(Permission::UPDATE);
        _data = std::move(data);
        return _data;
    }

  private:
    void assertPermission(const Permission permission)
    {
        if ((_userPermission.getPermissions(1) & permission) != permission)
        {
            throw std::runtime_error("Insufficient permissions");
        }
    }
};

class Application
{
    IDataService *_dataService;

  public:
    explicit Application(IDataService *dataService) : _dataService(dataService) {}

    void run(const int argc, char *argv[])
    {
        try
        {
            processCmd(argc, argv);
        }
        catch (std::exception &e)
        {
            std::cout << "Error: " << e.what();
        }
    }

  private:
    void processCmd(const int argc, char *argv[])
    {
        const std::string_view operation = argc > 1 ? argv[1] : "read";
        if (operation == "read")
        {
            printData(_dataService->getData());
        }
        else if (operation == "update")
        {
            if (argc < 4)
            {
                throw std::invalid_argument("Books args not provided");
            }
            printData(_dataService->updateData({argv[2], argv[3]}));
        }
        else
        {
            throw std::invalid_argument("Command not recognized");
        }
    }

    void printData(const Data &book)
    {
        std::cout << "Name: " << book.name << ", Payload: " << book.payload << std::endl;
    }
};

int main(int argc, char *argv[])
{
    ServiceCollection services;
    services.addSingleton<IUserPermission, UserPermission>();
    services.addSingleton<IDataService, DataService>();
    services.addSingleton<Application>();

    if (const auto appMode = std::getenv("APP_MODE"); appMode && appMode == std::string_view{"DEVELOPMENT"})
    {
        services.removeAll<IUserPermission>();
        services.addSingleton<IUserPermission, AdminPermission>();
    }
    auto provider = services.buildServiceProvider();

    auto &application = provider.getService<Application>();
    application.run(argc, argv);
}
