#include <FlyItf.hpp>

int main()
{
    Logger::getInstance().logful();
    FlyItf flyitf;
    flyitf.run();
}