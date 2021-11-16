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
class CpuSocket;

class Host
{
public:
    Host();

    bool initHost();

    std::string hostName;
    bool hasHyperThrading;
    std::vector<CpuSocket*> cpuSockets;

    bool hasSocketId(const uint32_t socketId) const;
    CpuSocket* getSocket(const uint32_t socketId) const;
    CpuSocket* addSocket(const uint32_t socketId);

    const std::string toString();
    const std::string toJsonString() const;

private:
    bool readHostName();
    bool initCpuCoresAndThreads();
};

}
}

#endif // HOST_H
