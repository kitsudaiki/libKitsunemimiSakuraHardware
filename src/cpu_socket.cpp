#include <libKitsunemimiSakuraHardware/cpu_socket.h>

#include <libKitsunemimiCpu/cpu.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>

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

const std::string
CpuSocket::toJsonString() const
{
    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(socketId) + ",");
    jsonString.append("\"cores\":[");

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
