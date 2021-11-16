#ifndef CPUTHREAD_H
#define CPUTHREAD_H

#include <string>
#include <iostream>
#include <vector>


namespace Kitsunemimi
{
namespace Sakura
{
class Host;

class CpuThread
{
public:
    CpuThread(const uint32_t threadId);

    const uint32_t threadId;
    const int64_t minSpeed = 0;
    const int64_t maxSpeed = 0;

    int64_t currentMinSpeed = 0;
    int64_t currentMaxSpeed = 0;

    bool initThread(Host* host);
    bool updateCurrentSpeed();

    const std::string toJsonString();
};

}
}

#endif // CPUTHREAD_H
