#include "BluetoothDiscovery.h"
#include "BluetoothDeviceInfo.h"

BluetoothDiscovery::BluetoothDiscovery(QObject *parent)
    : QObject{parent}
    , mDevDiscovery { new QBluetoothDeviceDiscoveryAgent(this) }
    , mDiscoveryMethods { QBluetoothDeviceDiscoveryAgent::ClassicMethod }
    , mDeviceCoreConfig { QBluetoothDeviceInfo::CoreConfiguration::BaseRateCoreConfiguration }
    , mIsActive { false }
{
    mDevDiscovery->setLowEnergyDiscoveryTimeout(20000);

    connect(mDevDiscovery, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
            &BluetoothDiscovery::addDevice);

    connect(mDevDiscovery, &QBluetoothDeviceDiscoveryAgent::finished, this, [&]() {
        setIsActive(false);
    });
    connect(mDevDiscovery, &QBluetoothDeviceDiscoveryAgent::canceled, this, [&]() {
        setIsActive(false);
    });
    connect(mDevDiscovery, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this,
            &BluetoothDiscovery::errorOccurred);
}

void BluetoothDiscovery::start()
{
    //! First clear current devices
    qDeleteAll(mDevices);
    mDevices.clear();
    emit devicesChanged();

    mDevDiscovery->start(
        QBluetoothDeviceDiscoveryAgent::DiscoveryMethods::fromInt(mDiscoveryMethods));
    setIsActive(true);
}

void BluetoothDiscovery::stop()
{
    if (mDevDiscovery->isActive()) {
        mDevDiscovery->stop();
    }
}

void BluetoothDiscovery::setMethods(DiscoveryMethods methods)
{
    if (mDiscoveryMethods == methods) {
        return;
    }

    mDiscoveryMethods = methods;
    emit methodsChanged();

    switch (mDiscoveryMethods) {
    case DiscoveryMethod::ClassicMethod: //! Only standard bluetooth devices
        mDeviceCoreConfig = QBluetoothDeviceInfo::CoreConfiguration::BaseRateCoreConfiguration;
        break;
    case DiscoveryMethod::LowEnergyMethod: //! Only Low Energy devices
        mDeviceCoreConfig = QBluetoothDeviceInfo::CoreConfiguration::LowEnergyCoreConfiguration;
        break;
    case DiscoveryMethod::LowEnergyMethod | DiscoveryMethod::ClassicMethod:
        //! Both Standard and Low Energy devices
        mDeviceCoreConfig = QBluetoothDeviceInfo::CoreConfiguration::BaseRateAndLowEnergyCoreConfiguration;
        break;
    default:
        mDeviceCoreConfig = QBluetoothDeviceInfo::CoreConfiguration::BaseRateCoreConfiguration;
        break;
    }
}

void BluetoothDiscovery::setIsActive(bool isActive)
{
    if (mIsActive == isActive) {
        return;
    }

    mIsActive = isActive;
    emit isActiveChanged();
}

void BluetoothDiscovery::setTimeOut(int timeout)
{
    if (!mDevDiscovery) {
        return;
    }

    if (mDevDiscovery->lowEnergyDiscoveryTimeout() == timeout) {
        return;
    }

    mDevDiscovery->setLowEnergyDiscoveryTimeout(timeout);
    emit timeOutChanged();
}

void BluetoothDiscovery::addDevice(const QBluetoothDeviceInfo& dev)
{

    if (dev.coreConfigurations() & mDeviceCoreConfig) {
        //! Check if it already exist
        auto iter = std::find_if(mDevices.begin(), mDevices.end(), [&dev](BluetoothDeviceInfo* bd) {
            return bd->address() == dev.address().toString();
        });

        if (iter == mDevices.end()) {
            //! Create a new instance
            mDevices.append(new BluetoothDeviceInfo(dev, this));

            emit devicesChanged();
        } else {
            //! Modify existing one
            (*iter)->setDevice(dev);
        }
    }
}

void BluetoothDiscovery::errorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    setIsActive(false);

    qWarning() << Q_FUNC_INFO << " Error: " << error;
}
