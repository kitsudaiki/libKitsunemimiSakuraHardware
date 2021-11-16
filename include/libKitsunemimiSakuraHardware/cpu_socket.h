#ifndef CPUSOCKET_H
#define CPUSOCKET_H

#include <string>
#include <iostream>
#include <vector>


namespace Kitsunemimi
{
namespace Sakura
{
class CpuCore;

class CpuSocket
{
public:
    CpuSocket(const uint32_t socketId);

    const uint32_t socketId;

    bool hasCoreId(const uint32_t coreId) const;
    CpuCore* getCore(const uint32_t coreId) const;
    CpuCore* addCore(const uint32_t coreId);

    double getThermalSpec() const;
    double getTotalPackagePower();

    const std::string toJsonString();

    std::vector<CpuCore*> cpuCores;
};

}
}

#endif // CPUSOCKET_H
