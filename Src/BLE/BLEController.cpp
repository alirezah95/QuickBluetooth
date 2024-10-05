#include "BLEController.h"

#include <QCoreApplication>
#include <QBluetoothLocalDevice>
#include <QBluetoothPermission>

//! \brief Initialize the single instance
BLEController* BLEController::sInstance = new BLEController();

BLEController *BLEController::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    return sInstance;
}

BLEController &BLEController::instance()
{
    return *sInstance;
}

BLEController::BLEController(QObject *parent)
    : QObject{ parent }
    , mDevice { nullptr }
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    initialize();
#endif
}

void BLEController::initialize()
{
    //! If permission is Undetermined request for it
    switch (qApp->checkPermission(QBluetoothPermission())) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(QBluetoothPermission(), [&](const QPermission& permission) {
            setPermission(permission.status());

            //! Recall this method
            initialize();
        });
    case Qt::PermissionStatus::Denied:
        qWarning() << "BLE: Bluetooth permsission is denied";
        emit error("BLE: Bluetooth permsission is denied");
        return;
    case Qt::PermissionStatus::Granted:
        break; //! Continue this method
    }

    //! Note if bluetooth permission is not granted host info and bluetooth device will be null
    if (QBluetoothLocalDevice::allDevices().length() == 0) {
        qWarning() << "BLE: There is no bluetooth on the device.";
    } else {
        setHostInfo(QBluetoothLocalDevice::allDevices().at(0));
        getDefaultDevice();
    }
}

QString BLEController::name() const
{
    return mHostInfo.name();
}

void BLEController::setPermission(Permission permission)
{
    if (mPermission == permission) {
        return;
    }

    mPermission = permission;
    emit permissionChanged();
}

void BLEController::requestPermission(std::function<void ()> cb)
{
#ifdef Q_OS_ANDROID
    switch (qApp->checkPermission(QBluetoothPermission())) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(QBluetoothPermission(), [&, cb](const QPermission &permission) {
            setPermission(permission.status());
        });
        break;
    case Qt::PermissionStatus::Denied:
        qWarning() << "BLE: Bluetooth permission is denied";
        return;
    case Qt::PermissionStatus::Granted:
        return;
    }
#endif
}

void BLEController::setHostInfo(const QBluetoothHostInfo& bh)
{
    if (mHostInfo == bh) {
        return;
    }

    mHostInfo = bh;

    emit nameChanged();
    emit bluetoothAvailableChanged();
}

void BLEController::getDefaultDevice()
{
    if (mDevice) {
        delete mDevice;
    }

    mDevice = new QBluetoothLocalDevice();
    emit bluetoothModeChanged();
}
