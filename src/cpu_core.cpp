/**
 * @file        cpu_core.cpp
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2021 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include <libKitsunemimiSakuraHardware/cpu_core.h>

#include <libKitsunemimiSakuraHardware/cpu_thread.h>

#include <libKitsunemimiCommon/common_items/data_items.h>

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
        jsonString.append(",\"minimum_speed\":" + std::to_string(thread->minSpeed));
        jsonString.append(",\"maximum_speed\":" + std::to_string(thread->maxSpeed));
        jsonString.append(",\"current_minimum_speed\":" + std::to_string(thread->currentMinSpeed));
        jsonString.append(",\"current_maximum_speed\":" + std::to_string(thread->currentMaxSpeed));
        jsonString.append(",\"current_speed\":" + std::to_string(thread->getCurrentSpeed()));
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

/**
 * @brief get information of the core as json-like item-tree

 * @return json-like item-tree with the information
 */
DataMap*
CpuCore::toJson()
{
    DataMap* result = new DataMap();

    result->insert("id", new DataValue((long)coreId));

    // get information of the core coming from the first thread of the core, because multiple
    // threads here means that hyperthreading is enabled, the all threads of the core have
    // exactly the same information
    if(cpuThreads.size() > 0)
    {
        const CpuThread* thread = cpuThreads.at(0);
        result->insert("minimum_speed", new DataValue((long)thread->minSpeed));
        result->insert("maximum_speed", new DataValue((long)thread->maxSpeed));
        result->insert("current_minimum_speed", new DataValue((long)thread->currentMinSpeed));
        result->insert("current_maximum_speed", new DataValue((long)thread->currentMaxSpeed));
        result->insert("current_speed", new DataValue((long)thread->getCurrentSpeed()));
    }

    // print information of the threads
    DataArray* threads = new DataArray();
    for(uint32_t i = 0; i < cpuThreads.size(); i++) {
        threads->append(cpuThreads.at(i)->toJson());
    }
    result->insert("threads", threads);

    return result;
}

} // namespace Sakura
} // namespace Kitsunemimi
