#ifndef CPUCORE_H
#define CPUCORE_H

#include <string>
#include <iostream>
#include <vector>

namespace Kitsunemimi
{
namespace Sakura
{
class CpuThread;

class CpuCore
{
public:
    CpuCore(const uint32_t coreId);
    ~CpuCore();

    const uint32_t coreId;

    CpuThread* getThread(const uint32_t threadId) const;
    CpuThread* addThread(const uint32_t threadId);
    void addCpuThread(CpuThread *thread);

    double getThermalSpec() const;
    double getTotalPackagePower();

    const std::string toJsonString();

    std::vector<CpuThread*> cpuThreads;
};

}
}

#endif // CPUCORE_H
