#include "BLECentral.h"
#include "BLEDataService.h"

BLECentral::BLECentral(QObject *parent)
    : BLERole{ parent }
{}

void BLECentral::setDevice(BluetoothDeviceInfo* dev)
{
    if (mDevice == dev) {
        return;
    }

    mDevice = dev;

    if (mController) {
        mController->disconnectFromDevice();
        delete mController;
        mController = nullptr;
    }

    if (mDevice) {
        mController = QLowEnergyController::createCentral(mDevice->device(), this);
        mController->setRemoteAddressType(QLowEnergyController::PublicAddress);

        connect(mController, &QLowEnergyController::serviceDiscovered,
                this, &BLECentral::serviceDiscovered);

        connect(mController, &QLowEnergyController::errorOccurred, this,
                [this](QLowEnergyController::Error error) {
                    emit stateChanged();
                    Q_UNUSED(error);
                    qWarning("BLECentral: Cannot connect to remote device.");
                });
        connect(mController, &QLowEnergyController::connected, this, [this]() {
            emit stateChanged();
            mController->discoverServices();
        });
        connect(mController, &QLowEnergyController::disconnected, this, [this]() {
            emit stateChanged();
            qWarning("BLECentral: LowEnergy controller disconnected");
        });

        // Connect
        mController->connectToDevice();
    }

    emit deviceChanged();
    emit stateChanged();
}

void BLECentral::disconnect()
{
    if (mController->state() == QLowEnergyController::ConnectedState) {
        mController->disconnectFromDevice();
        emit stateChanged();
    }
}

void BLECentral::serviceDiscovered(const QBluetoothUuid& uuid)
{
    auto dtService = std::find_if(mServices.begin(), mServices.end(), [&](BLEDataService* s) {
        return s->serviceBluetoothUuid() == uuid;
    });

    if (dtService == mServices.end()) {
        return;
    }

    if (QLowEnergyService* service = (*dtService)->setup(*mController)) {
        service->discoverDetails();
    }
}

void BLECentral::readData(const QBluetoothUuid& uuid)
{

}

void BLECentral::writeData(const QBluetoothUuid& uuid, const QVariant& value)
{

}
