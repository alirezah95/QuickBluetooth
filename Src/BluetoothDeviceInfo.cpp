#include "BluetoothDeviceInfo.hpp"

BluetoothDeviceInfo::BluetoothDeviceInfo(const QBluetoothDeviceInfo& device, QObject *parent)
    : QObject{parent}
    , mDevice { device }
{}

void BluetoothDeviceInfo::setDevice(const QBluetoothDeviceInfo& other)
{
    if (mDevice == other) {
        return;
    }

    mDevice = other;
    emit deviceChanged();
}
