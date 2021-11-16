/**
 * @file        host.h
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

#ifndef KITSUNEMIMI_SAKURA_HARDWARE_HOST_H
#define KITSUNEMIMI_SAKURA_HARDWARE_HOST_H

#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>

namespace Kitsunemimi
{
namespace Sakura
{
class CpuPackage;

class Host
{
public:
    Host();
    ~Host();

    bool initHost();

    std::string hostName;
    bool hasHyperThrading;
    std::vector<CpuPackage*> cpuPackages;

    CpuPackage* getPackage(const uint32_t packageId) const;
    CpuPackage* addPackage(const uint32_t packageId);

    const std::string toJsonString() const;

private:
    bool readHostName();
    bool initCpuCoresAndThreads();
};

} // namespace Sakura
} // namespace Kitsunemimi

#endif // KITSUNEMIMI_SAKURA_HARDWARE_HOST_H
