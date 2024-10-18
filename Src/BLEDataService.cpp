#include "BLEDataService.hpp"

#include <QtEndian>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyDescriptorData>
#include <QLowEnergyServiceData>

BLEDataService::BLEDataService(QObject *parent)
    : QObject{ parent }
    , mService { nullptr }
    , mValueLength { 2 }
    , mDataType { DataType::Int }
    , mValue { uint16_t(0) }
{}

QLowEnergyService* BLEDataService::setup(QLowEnergyController& leController)
{
    if (mServiceUuid.isNull() || mCharacterUuid.isNull()) {
        return nullptr;
    }

    if (leController.role() == QLowEnergyController::PeripheralRole) {
        //! This service is used in a Peripheral

        //! First set up characteristic data
        QLowEnergyCharacteristicData charData;
        charData.setUuid(mCharacterUuid);
        charData.setValue(QByteArray(mValueLength, 0));
        charData.setProperties(QLowEnergyCharacteristic::Read
                               | QLowEnergyCharacteristic::Write
                               | QLowEnergyCharacteristic::Notify);

        //! Add Descriptor data
        QLowEnergyDescriptorData des(
            QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration,
            QLowEnergyCharacteristic::CCCDEnableNotification);

        charData.addDescriptor(des);

        //! Create a service data
        QLowEnergyServiceData serviceData;
        serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
        serviceData.setUuid(mServiceUuid);
        serviceData.addCharacteristic(charData);

        mService = leController.addService(serviceData);
    } else {
        //! This service is used in a Central
        mService = leController.createServiceObject(mServiceUuid, this);
        if (mService) {
            connect(mService, &QLowEnergyService::stateChanged, this,
                    &BLEDataService::serviceStateChanged);
        }
    }

    if (mService) {
        connect(mService, &QLowEnergyService::characteristicChanged, this,
                &BLEDataService::onValueWritten);

        return mService;
    }

    return nullptr;
}

void BLEDataService::writeValue(const QVariant& value)
{
    if (!isValid() || !value.isValid()) {
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
    setValue(value);
}

void BLEDataService::setValue(QVariant value)
{
    if (!value.isValid()) {
        return;
    }

    mValue = value;
    emit valueChanged();
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

    //! When data type is changed mValue should also be changed to a QVariant with this type
    switch (mDataType) {
    case DataType::Int:
        setValue(QVariant::fromValue<uint16_t>(0));
        break;
    case DataType::Float:
        setValue(QVariant::fromValue<float>(0));
        break;
    case DataType::String:
        setValue(QVariant::fromValue<QString>(""));
        break;
    }
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
    if (!newValue.isValid()) {
        qWarning() << "Invalid value recieved: " << mDataType << value << newValue;
        return;
    }

    setValue(newValue);
    emit valueUpdated(newValue, QPrivateSignal());
}

void BLEDataService::serviceStateChanged(QLowEnergyService::ServiceState st)
{
    if (st == QLowEnergyService::RemoteServiceDiscovered) {
        //! Enable notification for the descriptor of the service characteristics
        QLowEnergyCharacteristic charData = mService->characteristic(mCharacterUuid);
        if (charData.isValid()) {
            QLowEnergyDescriptor srvDescriptor = charData.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
            if (srvDescriptor.isValid()) {
                mService->writeDescriptor(srvDescriptor,
                                          QLowEnergyCharacteristic::CCCDEnableNotification);
            }
        }
    }
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
        newValue = byteArray.toFloat(&ok);
        break;
    case DataType::String:
        newValue = QString(byteArray);
        break;
    }

    return ok ? newValue : QVariant();
}
