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

    const uint32_t coreId;

    bool hasThreadId(const uint32_t threadId) const;
    CpuThread* getThread(const uint32_t threadId) const;
    CpuThread* addThread(const uint32_t threadId);
    void addCpuThread(CpuThread *thread);

    const std::string toJsonString() const;

    std::vector<CpuThread*> cpuThreads;
};

}
}

#endif // CPUCORE_H
