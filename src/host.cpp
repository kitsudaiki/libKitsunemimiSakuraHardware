﻿/**
 * @file        host.cpp
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

#include <libKitsunemimiSakuraHardware/host.h>
#include <libKitsunemimiSakuraHardware/cpu_package.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>

#include <libKitsunemimiCpu/cpu.h>
#include <libKitsunemimiCommon/common_items/data_items.h>

namespace Kitsunemimi
{
namespace Sakura
{

/**
 * @brief constructor
 */
Host::Host() {}

/**
 * @brief destructor
 */
Host::~Host()
{
    for(uint32_t i = 0; i < cpuPackages.size(); i++) {
        delete cpuPackages[i];
    }

    cpuPackages.clear();
}

/**
 * @brief initialize the host-object with all necessary information of the system
 *
 * @return true, if successful, else false
 */
bool
Host::initHost(ErrorContainer &error)
{
    if(readHostName(error) == false) {
        return false;
    }
    if(initCpuCoresAndThreads(error) == false) {
        return false;
    }

    return true;
}

/**
 * @brief get a package by id
 *
 * @param packageId id of the requested package
 *
 * @return nullptr, if there is no package with the id, else pointer to the package
 */
CpuPackage*
Host::getPackage(const uint32_t packageId) const
{
    for(CpuPackage* package : cpuPackages)
    {
        if(package->packageId == packageId) {
            return package;
        }
    }

    return nullptr;
}

/**
 * @brief add a new package to the core
 *
 * @param packageId id of the package
 *
 * @return pointer to the package in list, if id already exist, else pointer to the new
 *         created package-object
 */
CpuPackage*
Host::addPackage(const uint32_t packageId)
{
    CpuPackage* package = getPackage(packageId);

    if(package == nullptr)
    {
        package = new CpuPackage(packageId);
        cpuPackages.push_back(package);
    }

    return package;
}

/**
 * @brief get information of the host as json-formated string
 *
 * @return json-formated string with the information
 */
const std::string
Host::toJsonString() const
{
    std::string jsonString = "{";

    // convert host-specific information
    jsonString.append("\"hostname\":\"" + hostName + "\",");
    if(hasHyperThrading) {
        jsonString.append("\"has_hyperthreading\":true,");
    } else {
        jsonString.append("\"has_hyperthreading\":false,");
    }

    // convert cpu-specific information
    jsonString.append("\"packages\":[");
    for(uint32_t i = 0; i < cpuPackages.size(); i++)
    {
        if(i > 0) {
            jsonString.append(",");
        }
        jsonString.append(cpuPackages.at(i)->toJsonString());
    }
    jsonString.append("]}");

    return jsonString;
}

/**
 * @brief get information of the host as json-like item-tree

 * @return json-like item-tree with the information
 */
DataMap*
Host::toJson() const
{
    Kitsunemimi::DataMap* result = new DataMap();

    // convert host-specific information
    result->insert("hostname", new DataValue(hostName));
    result->insert("has_hyperthreading", new DataValue(hasHyperThrading));

    // convert cpu-specific information
    DataArray* packages = new DataArray();
    for(uint32_t i = 0; i < cpuPackages.size(); i++) {
        packages->append(cpuPackages.at(i)->toJson());
    }
    result->insert("packages", packages);

    return result;
}

/**
 * @brief read host-name of the local system
 *
 * @return false, if reading the host-name failed, else true
 */
bool
Host::readHostName(ErrorContainer &error)
{
    const uint32_t maxHostNameLength = 256;
    char tempHostName[maxHostNameLength];
    if(gethostname(tempHostName, maxHostNameLength) < 0)
    {
        error.addMeesage("Failed to read host-name");
        LOG_ERROR(error);
        return false;
    }

    hostName = std::string(tempHostName);
    return true;
}

/**
 * @brief initalize all cpu-resources of the host
 *
 * @return true, if successful, else false
 */
bool
Host::initCpuCoresAndThreads(ErrorContainer &error)
{
    // get common information
    hasHyperThrading = Kitsunemimi::Cpu::isHyperthreadingEnabled(error);
    uint64_t numberOfCpuThreads = 0;
    if(Kitsunemimi::Cpu::getNumberOfCpuThreads(numberOfCpuThreads, error) < 1)
    {
        LOG_ERROR(error);
        return false;
    }

    // init the number of cpu-threads based on the physical number of threads
    for(uint64_t i = 0; i < numberOfCpuThreads; i++)
    {
        CpuThread* thread = new CpuThread(i);
        if(thread->initThread(this) == false) {
            return false;
        }
    }

    return true;
}

} // namespace Sakura
} // namespace Kitsunemimi
