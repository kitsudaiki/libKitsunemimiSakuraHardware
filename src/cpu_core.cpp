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

double
CpuCore::getThermalSpec() const
{
    if(cpuThreads.size() > 0) {
        return cpuThreads.at(0)->getThermalSpec();
    }

    return 0.0;
}

double
CpuCore::getTotalPackagePower()
{
    if(cpuThreads.size() > 0) {
        return cpuThreads.at(0)->getTotalPackagePower();
    }

    return 0.0;
}

const std::string
CpuCore::toJsonString()
{
    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(coreId));
    if(cpuThreads.size() > 0)
    {
        const CpuThread* thread = cpuThreads.at(0);
        const int64_t currentSpeed = thread->getCurrentSpeed();
        jsonString.append(",\"minimum_speed\":" + std::to_string(thread->minSpeed));
        jsonString.append(",\"maximum_speed\":" + std::to_string(thread->maxSpeed));
        jsonString.append(",\"current_minimum_speed\":" + std::to_string(thread->currentMinSpeed));
        jsonString.append(",\"current_maximum_speed\":" + std::to_string(thread->currentMaxSpeed));
        jsonString.append(",\"current_speed\":" + std::to_string(currentSpeed));
    }

    jsonString.append(",\"threads\":[");
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
