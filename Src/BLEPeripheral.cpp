#include "BLEPeripheral.h"
#include "BLEDataService.h"
#include "BluetoothController.h"

#include <QLowEnergyAdvertisingParameters>

BLEPeripheral::BLEPeripheral(QObject *parent)
    : BLERole{ parent }
{}

void BLEPeripheral::initialize()
{
    if (!BluetoothController::instance().isReady()) {
        qWarning() << "BluetoothController is not ready";
        return;
    }

    mController = QLowEnergyController::createPeripheral(this);
    connect(
        mController, &QLowEnergyController::errorOccurred, this, &BLEPeripheral::onErrorOccured);
    connect(mController, &QLowEnergyController::connected, this, [&]() {
        emit stateChanged();
    });
    connect(mController, &QLowEnergyController::disconnected, this, [&]() {
        emit stateChanged();
    });
}

void BLEPeripheral::startAdvertising()
{
    if (!mController || mController->localAddress().isNull()) {
        qWarning() << "Can't start advertising, BLEPeripheral is not initialized.";
        return;
    }

    if (mLocalName.isEmpty()) {
        qWarning() << "Can't start advertising when local name is empty";
        return;
    }

    //! Create the list of service class uuids for advertising
    QList<QBluetoothUuid> services;
    for (BLEDataService* srv : mServices) {
        //! If a service with the same service uuid is already added abort adding this
        auto buIt = std::find_if(services.begin(), services.end(), [srv](const QBluetoothUuid& bu) {
            return bu == srv->serviceBluetoothUuid();
        });
        if (buIt != services.end()) {
            //! This service is already added
            qWarning() << "BLEDataService with uuid: " << srv->serviceBluetoothUuid().toUInt32()
                       << " is already added.";
            continue;
        }

        if (srv->setup(*mController)) {
            services.append(srv->serviceBluetoothUuid());
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

    qDebug() << "BLEPeripheral advertising started with name : " << mLocalName;
}

void BLEPeripheral::onErrorOccured(QLowEnergyController::Error error)
{
    qWarning() << "BLEPeripheral " << error << ", " << mController->errorString();
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

void BLEPeripheral::readData(const QBluetoothUuid& uuid)
{

}

void BLEPeripheral::writeData(const QBluetoothUuid& uuid, const QVariant& value)
{

}
