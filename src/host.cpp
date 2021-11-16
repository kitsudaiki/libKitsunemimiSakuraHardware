#include <libKitsunemimiSakuraHardware/host.h>
#include <libKitsunemimiCpu/cpu.h>
#include <libKitsunemimiSakuraHardware/cpu_socket.h>
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
Host::hasSocketId(const uint32_t socketId) const
{
    for(const CpuSocket* socket : cpuSockets)
    {
        if(socket->socketId == socketId) {
            return true;
        }
    }

    return false;
}

CpuSocket*
Host::getSocket(const uint32_t socketId) const
{
    for(CpuSocket* socket : cpuSockets)
    {
        if(socket->socketId == socketId) {
            return socket;
        }
    }

    return nullptr;
}

CpuSocket*
Host::addSocket(const uint32_t socketId)
{
    CpuSocket* socket = getSocket(socketId);

    if(socket == nullptr)
    {
        socket = new CpuSocket(socketId);
        cpuSockets.push_back(socket);
    }

    return socket;
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

    jsonString.append("\"sockets\":[");

    for(uint32_t i = 0; i < cpuSockets.size(); i++)
    {
        if(i > 0) {
            jsonString.append(",");
        }
        jsonString.append(cpuSockets.at(i)->toJsonString());
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

