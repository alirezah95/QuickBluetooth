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
