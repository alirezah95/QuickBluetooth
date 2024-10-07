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

    mService = leController.addService(serviceData);
    if (mService) {
        connect(mService, &QLowEnergyService::characteristicChanged, this,
                &BLEDataService::onValueWritten);

        return true;
    }

    return false;
}

void BLEDataService::writeValue(const QVariant& value)
{
    if (!isValid() || value.isNull() || !value.isValid()) {
        return;
    }

    QByteArray data = valueToByteArray(value);
    if (data.isEmpty()) {
        return;
    }

    QLowEnergyCharacteristic charac = mService->characteristic(mCharacterUuid);
    if (!charac.isValid()) {
        return;
    }
    mService->writeCharacteristic(charac, data);
}

void BLEDataService::setValue(QByteArray byteArray)
{
    if (byteArray.length() == 0) {
        return;
    }

    QVariant newValue = byteArrayToValue(byteArray);
    if (mValue == newValue || !newValue.isValid()) {
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

void BLEDataService::onValueWritten(const QLowEnergyCharacteristic& characteristic,
                                    const QByteArray& value)
{
    if (characteristic.uuid() != mCharacterUuid) {
        return;
    }

    QVariant newValue = byteArrayToValue(value);

    setValue(value);
    emit valueUpdated(value, QPrivateSignal());
}

QByteArray BLEDataService::valueToByteArray(const QVariant& value)
{
    QByteArray data;

    switch (mDataType) {
    case DataType::Int: { //! This is 16 bit
        if (!value.canConvert<int>()) {
            return QByteArray();
        }
        data.setNum(static_cast<uint16_t>(value.value<int>()));
        break;
    }
    case DataType::Float:
        if (!value.canConvert<float>()) {
            return QByteArray();
        }
        data.setNum(static_cast<float>(value.value<float>()));
        break;
    case DataType::String:
        if (!value.canConvert<QString>()) {
            return QByteArray();
        }
        data.append(value.value<QString>().toUtf8());
        break;
    }

    return data;
}

QVariant BLEDataService::byteArrayToValue(const QByteArray& byteArray)
{
    QVariant newValue;

    bool ok = true;

    switch (mDataType) {
    case DataType::Int:
        newValue = byteArray.toShort(&ok);
        break;
    case DataType::Float:
        if (byteArray.length() < sizeof(float)) {
            return QVariant();
        }
        newValue = byteArray.toFloat(&ok);
        break;
    case DataType::String:
        newValue = QString(byteArray);
        break;
    }

    return ok ? newValue : QVariant();
}
