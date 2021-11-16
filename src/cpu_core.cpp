#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>

namespace Kitsunemimi
{
namespace Sakura
{

/**
 * @brief constructor
 *
 * @param coreId id of the core
 */
CpuCore::CpuCore(const uint32_t coreId)
    : coreId(coreId) {}

/**
 * @brief destructor
 */
CpuCore::~CpuCore()
{
    for(uint32_t i = 0; i < cpuThreads.size(); i++) {
        delete cpuThreads[i];
    }

    cpuThreads.clear();
}

/**
 * @brief get a thread by id
 *
 * @param threadId id of the requested thread
 *
 * @return nullptr, if there is no thead with the id, else pointer to the thread
 */
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

/**
 * @brief add a new thread to the core
 *
 * @param threadId id of the thread
 *
 * @return pointer to the thread in list, if id already exist, else pointer to the new
 *         created thread-object
 */
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

/**
 * @brief add existing thread-object the the list of threads of the core
 *
 * @param thread pointer to thread to add to list
 */
void
CpuCore::addCpuThread(CpuThread* thread)
{
    if(getThread(thread->threadId) == nullptr) {
        cpuThreads.push_back(thread);
    }
}

/**
 * @brief get maximum thermal spec of the package
 *
 * @return 0.0 if RAPL is not initialized, else thermal spec of the cpu-package
 */
double
CpuCore::getThermalSpec() const
{
    if(cpuThreads.size() > 0) {
        return cpuThreads.at(0)->getThermalSpec();
    }

    return 0.0;
}

/**
 * @brief get current total power consumption of the cpu-package since the last check
 *
 * @return 0.0 if RAPL is not initialized, else current total power consumption of the cpu-package
 */
double
CpuCore::getTotalPackagePower()
{
    if(cpuThreads.size() > 0) {
        return cpuThreads.at(0)->getTotalPackagePower();
    }

    return 0.0;
}

/**
 * @brief get information of the core as json-formated string
 *
 * @return json-formated string with the information
 */
const std::string
CpuCore::toJsonString()
{
    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(coreId));
    // get information of the core coming from the first thread of the core, because multiple
    // threads here means that hyperthreading is enabled, the all threads of the core have
    // exactly the same information
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

    // print information of the threads
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
