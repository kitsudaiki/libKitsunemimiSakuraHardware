#include <libKitsunemimiSakuraHardware/cpu_package.h>

#include <libKitsunemimiCpu/cpu.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>

namespace Kitsunemimi
{
namespace Sakura
{

CpuPackage::CpuPackage(const uint32_t packageId)
    : packageId(packageId)
{
}

bool
CpuPackage::hasCoreId(const uint32_t coreId) const
{
    for(const CpuCore* core : cpuCores)
    {
        if(core->coreId == coreId) {
            return true;
        }
    }

    return false;
}

CpuCore*
CpuPackage::getCore(const uint32_t coreId) const
{
    for(CpuCore* core : cpuCores)
    {
        if(core->coreId == coreId) {
            return core;
        }
    }

    return nullptr;
}

CpuCore*
CpuPackage::addCore(const uint32_t coreId)
{
    CpuCore* core = getCore(coreId);

    if(core == nullptr)
    {
        core = new CpuCore(coreId);
        cpuCores.push_back(core);
    }

    return core;
}


double
CpuPackage::getThermalSpec() const
{
    if(cpuCores.size() > 0) {
        return cpuCores.at(0)->getThermalSpec();
    }

    return 0.0;
}

double
CpuPackage::getTotalPackagePower()
{
    if(cpuCores.size() > 0) {
        return cpuCores.at(0)->getTotalPackagePower();
    }

    return 0.0;
}

const std::string
CpuPackage::toJsonString()
{
    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(packageId));
    jsonString.append(",\"thermal_spec\":" + std::to_string(getThermalSpec()));
    jsonString.append(",\"power\":" + std::to_string(getTotalPackagePower()));
    jsonString.append(",\"cores\":[");

    for(uint32_t i = 0; i < cpuCores.size(); i++)
    {
        if(i > 0) {
            jsonString.append(",");
        }
        jsonString.append(cpuCores.at(i)->toJsonString());
    }
    jsonString.append("]}");

    return jsonString;
}

}
}
