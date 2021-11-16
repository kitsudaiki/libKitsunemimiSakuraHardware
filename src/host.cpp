#include <libKitsunemimiSakuraHardware/host.h>
#include <libKitsunemimiCpu/cpu.h>
#include <libKitsunemimiSakuraHardware/cpu_package.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>

namespace Kitsunemimi
{
namespace Sakura
{

Host::Host()
{

}

bool
Host::initHost()
{
    if(readHostName() == false) {
        return false;
    }
    if(initCpuCoresAndThreads() == false) {
        return false;
    }

    return true;
}

bool
Host::hasPackageId(const uint32_t packageId) const
{
    for(const CpuPackage* package : cpuPackages)
    {
        if(package->packageId == packageId) {
            return true;
        }
    }

    return false;
}

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

const
std::string Host::toString()
{
    return "";
}

const
std::string Host::toJsonString() const
{
    std::string jsonString = "{";
    jsonString.append("\"hostname\":\"" + hostName + "\",");
    if(hasHyperThrading) {
        jsonString.append("\"has_hyperthreading\":true,");
    } else {
        jsonString.append("\"has_hyperthreading\":false,");
    }

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
 * @brief Host::readHostName
 * @return
 */
bool
Host::readHostName()
{
    const uint32_t maxHostNameLength = 256;
    char tempHostName[maxHostNameLength];
    if(gethostname(tempHostName, maxHostNameLength) < 0) {
        return false;
    }

    hostName = std::string(tempHostName);
    return true;
}

/**
 * @brief Host::initCpuCoresAndThreads
 * @return
 */
bool
Host::initCpuCoresAndThreads()
{
    hasHyperThrading = Kitsunemimi::Cpu::isHyperthreadingEnabled();
    const int32_t numberOfCpuThreads = Kitsunemimi::Cpu::getNumberOfCpuThreads();
    if(numberOfCpuThreads < 1)
    {
        // TODO: handle error
        return false;
    }

    std::vector<CpuThread*> tempThreadBuffer;
    std::vector<CpuCore*> tempCoreBuffer;

    // init the number of cpu-threads based on the physical number of threads
    for(int32_t i = 0; i < numberOfCpuThreads; i++)
    {
        CpuThread* thread = new CpuThread(i);
        if(thread->initThread(this) == false) {
            return false;
        }
    }

    return true;
}

}
}

