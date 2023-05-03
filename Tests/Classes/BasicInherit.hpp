
struct TestInheritClass1
{
};
struct TestInheritClass2 : public TestInheritClass1
{
};
struct TestInheritClass3 : public TestInheritClass2
{
};
struct TestInheritClass4 : public TestInheritClass3
{
};
struct TestInheritClass5 final : public TestInheritClass4
{
};