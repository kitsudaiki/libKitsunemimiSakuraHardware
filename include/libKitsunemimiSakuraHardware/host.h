#ifndef HOST_H
#define HOST_H

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

}
}

#endif // HOST_H
