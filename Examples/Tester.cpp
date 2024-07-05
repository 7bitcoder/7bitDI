#include <SevenBit/DI.hpp>
#include <iostream>
#include <utility>

using namespace sb::di;

struct TestFailed final : std::runtime_error
{
    explicit TestFailed(const std::string &message) : std::runtime_error(message) {}
};

class Expect
{
  public:
    template <class L, class R> void static equal(const L &lhs, const R &rhs)
    {
        throwIfFalse(lhs == rhs, "Expected values to be equal");
    }
    template <class L, class R> void static notEqual(const L &lhs, const R &rhs)
    {
        throwIfFalse(lhs != rhs, "Expected values not to be equal");
    }

    template <class T> void static truthy(const T &value)
    {
        throwIfFalse(static_cast<bool>(value), "Expected value to be true");
    }
    template <class T> void static falsy(const T &value)
    {
        throwIfFalse(!static_cast<bool>(value), "Expected value to be false");
    }

  private:
    static void throwIfFalse(const bool value, const std::string_view message)
    {
        if (!value)
        {
            throw TestFailed(std::string{message});
        }
    }
};

struct ITest
{
    virtual std::string name() = 0;

    virtual void run() = 0;

    virtual ~ITest() = default;
};

class TestRunner
{
    std::vector<ITest *> tests;

  public:
    explicit TestRunner(std::vector<ITest *> tests) : tests(std::move(tests)) {}

    int run()
    {
        const size_t total = tests.size();
        size_t passed = 0;
        for (const auto test : tests)
        {
            passed += runTest(*test);
        }
        std::cout << "Tests run: " << passed << "/" << total << std::endl;
        return passed != total;
    }

  private:
    bool runTest(ITest &test)
    {
        try
        {
            test.run();
            std::cout << "Test '" + test.name() + "' passed" << std::endl;
            return true;
        }
        catch (std::exception &fail)
        {
            std::cerr << "Test '" + test.name() + "' failed: " << fail.what() << std::endl;
        }
        return false;
    }
};

template <class TestImpl> struct Test : ITest, RegisterScoped<ITest, TestImpl>
{
    std::string name() override { return typeid(TestImpl).name(); }
};

struct AddTest final : Test<AddTest>
{
    void run() override
    {
        Expect::equal(2 + 2, 4);
        Expect::notEqual(2 + 1, 4);
        Expect::falsy(1 + -1);
        Expect::truthy(1 + 2);
    }
};

struct MultiplyTest final : Test<MultiplyTest>
{
    void run() override
    {
        Expect::equal(2 * 2, 4);
        Expect::notEqual(2 * 3, 4);
        Expect::falsy(1 * 0);
        Expect::truthy(1 * 2);
    }
};

int main()
{
    ServiceProvider provider = GlobalServices::instance().addSingleton<TestRunner>().buildServiceProvider();

    auto &runner = provider.getService<TestRunner>();
    return runner.run();
}
