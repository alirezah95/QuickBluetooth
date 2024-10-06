#include "BLEDataService.h"

#include <QtEndian>

BLEDataService::BLEDataService(QObject *parent)
    : QObject{parent}
{}

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

QBluetoothUuid BLEDataService::serviceUuid() const
{
    return mServiceUuid;
}

void BLEDataService::setServiceUuid(const QBluetoothUuid& newServiceUuid)
{
    if (mServiceUuid == newServiceUuid) {
        return;
    }

    mServiceUuid = newServiceUuid;
    emit serviceUuidChanged();
}

QBluetoothUuid BLEDataService::characterUuid() const
{
    return mCharacterUuid;
}

void BLEDataService::setCharacterUuid(const QBluetoothUuid& newCharacterUuid)
{
    if (mCharacterUuid == newCharacterUuid) {
        return;
    }
    mCharacterUuid = newCharacterUuid;
    emit characterUuidChanged();
}

QBluetoothUuid BLEDataService::descriptorUuid() const
{
    return mDescriptorUuid;
}

void BLEDataService::setDescriptorUuid(const QBluetoothUuid& newDescriptorUuid)
{
    if (mDescriptorUuid == newDescriptorUuid) {
        return;
    }

    mDescriptorUuid = newDescriptorUuid;
    emit descriptorUuidChanged();
}
