#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>

namespace Kitsunemimi
{
namespace Sakura
{

CpuCore::CpuCore(const uint32_t coreId)
    : coreId(coreId)
{
}

bool
CpuCore::hasThreadId(const uint32_t threadId) const
{
    for(const CpuThread* thread : cpuThreads)
    {
        if(thread->threadId == threadId) {
            return true;
        }
    }

    return false;
}

CpuThread*
CpuCore::getThread(const uint32_t threadId) const
{
    for(CpuThread* thread : cpuThreads)
    {
        if(thread->threadId == threadId) {
            return thread;
        }
    }

    return nullptr;
}

CpuThread*
CpuCore::addThread(const uint32_t threadId)
{
    CpuThread* thread = getThread(threadId);

    if(thread == nullptr)
    {
        thread = new CpuThread(threadId);
        cpuThreads.push_back(thread);
    }

    return thread;
}

void
CpuCore::addCpuThread(CpuThread* thread)
{
    if(hasThreadId(thread->threadId) == false) {
        cpuThreads.push_back(thread);
    }
}

const std::string
CpuCore::toJsonString() const
{
    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(coreId) + ",");
    jsonString.append("\"threads\":[");

    for(uint32_t i = 0; i < cpuThreads.size(); i++)
    {
        if(i > 0) {
            jsonString.append(",");
        }
        jsonString.append(cpuThreads.at(i)->toJsonString());
    }
    jsonString.append("]}");

    return jsonString;
}

}
}
