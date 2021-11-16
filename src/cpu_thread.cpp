#include <libKitsunemimiSakuraHardware/cpu_thread.h>
#include <libKitsunemimiSakuraHardware/cpu_package.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/host.h>
#include <libKitsunemimiCpu/cpu.h>

namespace Kitsunemimi
{
namespace Sakura
{

/**
 * @brief constructor
 *
 * @param threadId id of the physical cpu thread, which belongs to this thread
 */
CpuThread::CpuThread(const uint32_t threadId)
    : threadId(threadId),
      minSpeed(Kitsunemimi::Cpu::getMinimumSpeed(threadId)),
      maxSpeed(Kitsunemimi::Cpu::getMaximumSpeed(threadId)),
      currentMinSpeed(Kitsunemimi::Cpu::getCurrentMinimumSpeed(threadId)),
      currentMaxSpeed(Kitsunemimi::Cpu::getCurrentMaximumSpeed(threadId)),
      m_rapl(threadId) {}

/**
 * @brief destructor
 */
CpuThread::~CpuThread() {}

/**
 * @brief initialize the new thread-object and add the thread to the topological tree of objects
 *
 * @param host pointer to the host-object at the top
 *
 * @return true, if successfull, else false
 */
bool
CpuThread::initThread(Host* host)
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

    const int32_t packageId = Kitsunemimi::Cpu::getCpuPackageId(threadId);
    if(packageId < 0)
    {
        // TODO: handle error
        return false;
    }

    // try to init rapl
    m_rapl.initRapl();

    // add thread to the topological overview
    CpuPackage* package = host->addPackage(packageId);
    CpuCore* core = package->addCore(coreId);
    core->addCpuThread(this);

    return true;
}

/**
 * @brief get current speed of the core
 *
 * @return -1, if reading the speed failed, else speed of the core
 */
int64_t
CpuThread::getCurrentSpeed() const
{
    return Kitsunemimi::Cpu::getCurrentSpeed(threadId);
}

/**
 * @brief get maximum thermal spec of the package
 *
 * @return 0.0 if RAPL is not initialized, else thermal spec of the cpu-package
 */
double
CpuThread::getThermalSpec() const
{
    // check if RAPL was successfully initialized
    if(m_rapl.isActive() == false) {
        return 0.0;
    }

    return  m_rapl.getInfo().thermal_spec_power;
}

/**
 * @brief get current total power consumption of the cpu-package since the last check
 *
 * @return 0.0 if RAPL is not initialized, else current total power consumption of the cpu-package
 */
double
CpuThread::getTotalPackagePower()
{
    // check if RAPL was successfully initialized
    if(m_rapl.isActive() == false) {
        return 0.0;
    }

    return m_rapl.calculateDiff().pkgAvg;
}

/**
 * @brief get information of the thread as json-formated string
 *
 * @return json-formated string with the information
 */
const std::string
CpuThread::toJsonString()
{
    getCurrentSpeed();

    std::string jsonString = "{";
    jsonString.append("\"id\":" + std::to_string(threadId));
    jsonString.append("}");

    return jsonString;
}

}
}
