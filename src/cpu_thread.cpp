#include <libKitsunemimiSakuraHardware/cpu_thread.h>
#include <libKitsunemimiSakuraHardware/cpu_socket.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/host.h>
#include <libKitsunemimiCpu/cpu.h>

namespace Kitsunemimi
{
namespace Sakura
{

CpuThread::CpuThread(const uint32_t threadId)
    : threadId(threadId),
      minSpeed(Kitsunemimi::Cpu::getMinimumSpeed(threadId)),
      maxSpeed(Kitsunemimi::Cpu::getMaximumSpeed(threadId)),
      m_rapl(threadId)
{

}

bool CpuThread::initThread(Host* host)
{
    if(minSpeed < 0) {
        // TODO: handle error
        return false;
    }

    if(maxSpeed < 0) {
        // TODO: handle error
        return false;
    }

    const int32_t coreId = Kitsunemimi::Cpu::getCpuCoreId(threadId);
    if(coreId < 0)
    {
        // TODO: handle error
        return false;
    }

    const int32_t socketId = Kitsunemimi::Cpu::getCpuSocketId(threadId);
    if(socketId < 0)
    {
        // TODO: handle error
        return false;
    }

    // try to init rapl
    m_rapl.initRapl();

    CpuSocket* socket = host->addSocket(socketId);
    CpuCore* core = socket->addCore(coreId);
    core->addCpuThread(this);

    return true;
}

bool
CpuThread::updateCurrentSpeed()
{
    currentMinSpeed = Kitsunemimi::Cpu::getCurrentMinimumSpeed(threadId);
    if(currentMinSpeed < 0) {
        return false;
    }

    currentMaxSpeed = Kitsunemimi::Cpu::getCurrentMaximumSpeed(threadId);
    if(currentMaxSpeed < 0) {
        return false;
    }

    return true;
}

double
CpuThread::getThermalSpec() const
{
    if(m_rapl.isActive() == false) {
        return 0.0;
    }

    const Kitsunemimi::Cpu::RaplInfo info = m_rapl.getInfo();
    return info.thermal_spec_power;
}

double
CpuThread::getTotalPackagePower()
{
    if(m_rapl.isActive() == false) {
        return 0.0;
    }

    const Kitsunemimi::Cpu::RaplDiff diff = m_rapl.calculateDiff();
    return diff.pkgAvg;
}

const std::string
CpuThread::toJsonString()
{
    updateCurrentSpeed();

    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(threadId));
    jsonString.append("}");

    return jsonString;
}

}
}
