#include "BLEDataService.h"

#include <QtEndian>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyDescriptorData>
#include <QLowEnergyServiceData>

BLEDataService::BLEDataService(QObject *parent)
    : QObject{ parent }
    , mValueLength { 2 }
{}

bool BLEDataService::setup(QLowEnergyController& leController)
{
    if (mServiceUuid.isNull() || mCharacterUuid.isNull() || mDescriptorUuid.isNull()) {
        return false;
    }

    //! First set up characteristic data
    QLowEnergyCharacteristicData charData;
    charData.setUuid(mCharacterUuid);
    charData.setValue(QByteArray(mValueLength, 0));
    charData.setProperties(QLowEnergyCharacteristic::Read
                           | QLowEnergyCharacteristic::Write
                           | QLowEnergyCharacteristic::Notify);

    //! Add Descriptor data
    QLowEnergyDescriptorData des(mDescriptorUuid, QByteArray(mValueLength, 0));
    charData.addDescriptor(des);

    //! Create a service data
    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(mServiceUuid);
    serviceData.addCharacteristic(charData);

    return leController.addService(serviceData) != nullptr;
}

void BLEDataService::setValue(QByteArray value)
{
    if (value.length() == 0) {
        return;
    }

    QVariant newValue;
    auto data = reinterpret_cast<const quint8*>(value.constData());

    switch (mDataType) {
    case DataType::Int:
        if (value.length() < 2) { //! Two bytes required
            return;
        }
        newValue = qFromLittleEndian<uint16_t>(data[0]);
        break;
    case DataType::Float:
        if (value.length() < sizeof(float)) {
            return;
        }
        newValue = qFromLittleEndian<float>(data[0]);
        break;
    case DataType::String:
        newValue = QString(value);
        break;
    }

    if (mValue == newValue) {
        return;
    }

    mValue = newValue;
    emit valueChanged();
}

void BLEDataService::setDataType(DataType dataType)
{
    if (mDataType == dataType) {
        return;
    }

    mDataType = dataType;
    emit dataTypeChanged();
}

uint32_t BLEDataService::serviceUuid() const
{
    return mServiceUuid.toUInt32();
}

void BLEDataService::setServiceUuid(uint32_t newServiceUuid)
{
    QBluetoothUuid uuid(newServiceUuid);

    if (mServiceUuid == uuid) {
        return;
    }

    mServiceUuid = uuid;
    emit serviceUuidChanged();
}

uint32_t BLEDataService::characterUuid() const
{
    return mCharacterUuid.toUInt32();
}

void BLEDataService::setCharacterUuid(uint32_t newCharacterUuid)
{
    QBluetoothUuid uuid(newCharacterUuid);

    if (mCharacterUuid == uuid) {
        return;
    }

    mCharacterUuid = uuid;
    emit characterUuidChanged();
}

uint32_t BLEDataService::descriptorUuid() const
{
    return mDescriptorUuid.toUInt32();
}

void BLEDataService::setDescriptorUuid(uint32_t newDescriptorUuid)
{
    QBluetoothUuid uuid(newDescriptorUuid);

    if (mDescriptorUuid == uuid) {
        return;
    }

    mDescriptorUuid = uuid;
    emit descriptorUuidChanged();
}

quint8 BLEDataService::valueLength() const
{
    return mValueLength;
}

void BLEDataService::setValueLength(quint8 newValueLength)
{
    if (mValueLength == newValueLength) {
        return;
    }

    if (newValueLength < 1 || newValueLength > 31) {
        qWarning() << "Value length must be greater than 0 and less than 32";
        return;
    }

    mValueLength = newValueLength;
    emit valueLengthChanged();
}
