#include <libKitsunemimiSakuraHardware/cpu_socket.h>

#include <libKitsunemimiCpu/cpu.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>

namespace Kitsunemimi
{
namespace Sakura
{

CpuSocket::CpuSocket(const uint32_t socketId)
    : socketId(socketId)
{
}

bool
CpuSocket::hasCoreId(const uint32_t coreId) const
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
CpuSocket::getCore(const uint32_t coreId) const
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
CpuSocket::addCore(const uint32_t coreId)
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
CpuSocket::getThermalSpec() const
{
    if(cpuCores.size() > 0) {
        return cpuCores.at(0)->getThermalSpec();
    }

    return 0.0;
}

double
CpuSocket::getTotalPackagePower()
{
    if(cpuCores.size() > 0) {
        return cpuCores.at(0)->getTotalPackagePower();
    }

    return 0.0;
}

const std::string
CpuSocket::toJsonString()
{
    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(socketId));
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
