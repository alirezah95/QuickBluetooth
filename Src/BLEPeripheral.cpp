#include "BLEPeripheral.h"
#include "BLEDataService.h"
#include "BluetoothController.h"

#include <QLowEnergyAdvertisingParameters>

BLEPeripheral::BLEPeripheral(QObject *parent)
    : QObject{ parent }
    , mController { nullptr }
{}

void BLEPeripheral::initialize()
{
    if (!BluetoothController::instance().isReady()) {
        qWarning() << "BluetoothController is not ready";
        return;
    }

    mController = QLowEnergyController::createPeripheral(this);
}

void BLEPeripheral::startAdvertising()
{
    if (!mController || mController->localAddress().isNull()) {
        qWarning() << "Can't start advertising, BLEPeripheral is not initialized.";
        return;
    }

    //! Create the list of service class uuids for advertising
    QList<QBluetoothUuid> services;
    for (BLEDataService* srv : mServices) {
        if (srv->setup(*mController)) {
            services.append(srv->serviceUuid());
        }
    }

    //! Setting up the advertising data and parameters
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setIncludePowerLevel(true);
    advertisingData.setLocalName(mLocalName);
    advertisingData.setServices(services);

    //! Start advertising
    mController->startAdvertising(QLowEnergyAdvertisingParameters(),
                                  advertisingData,
                                  advertisingData);
}

ServicesListProperty BLEPeripheral::services()
{
    return QQmlListProperty<BLEDataService>(this, this,
                                            &BLEPeripheral::servicesListAppend,
                                            &BLEPeripheral::servicesListCount,
                                            &BLEPeripheral::servicesListAt,
                                            &BLEPeripheral::servicesListClear);
}

void BLEPeripheral::serviceAdd(BLEDataService* ble)
{
    if (!ble) {
        return;
    }

    //! If a service with this uuid already exist, abort
    auto srvIter = std::find_if(mServices.begin(), mServices.end(), [ble](BLEDataService* item){
        return item->characterUuid() == ble->characterUuid();
    });

    if (srvIter != mServices.end()) {
        qWarning() << "BLEDataService with the " << ble->characterUuid()
                   << " UUID already exist, aborting";
        return;
    }

    mServices.append(ble);
    emit servicesChanged();
}

qsizetype BLEPeripheral::serviceCount() const
{
    return mServices.length();
}

BLEDataService* BLEPeripheral::serviceAt(qsizetype index)
{
    return index >= 0 && index < mServices.length() ? mServices.at(index) : nullptr;
}

void BLEPeripheral::serviceClear()
{
    qDeleteAll(mServices);
    mServices.clear();
}

void BLEPeripheral::servicesListAppend(ServicesListProperty* services, BLEDataService* service)
{
    reinterpret_cast<BLEPeripheral*>(services->object)->serviceAdd(service);
}

BLEDataService* BLEPeripheral::servicesListAt(ServicesListProperty* services, qsizetype index)
{
    return reinterpret_cast<BLEPeripheral*>(services->object)->serviceAt(index);
}

qsizetype BLEPeripheral::servicesListCount(ServicesListProperty* services)
{
    return reinterpret_cast<BLEPeripheral*>(services->object)->serviceCount();
}

void BLEPeripheral::servicesListClear(ServicesListProperty* services)
{
    reinterpret_cast<BLEPeripheral*>(services->object)->serviceClear();
}

QString BLEPeripheral::localName() const
{
    return mLocalName;
}

void BLEPeripheral::setLocalName(const QString& localName)
{
    if (mLocalName == localName) {
        return;
    }

    mLocalName = localName;
    emit localNameChanged();
}
