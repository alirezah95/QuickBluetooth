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
    , mPermission { Qt::PermissionStatus::Undetermined }
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    initialize();
#endif
}

void BLEController::initialize()
{
    requestPermission(std::bind(&BLEController::initialize, this));

    //! Note if bluetooth permission is not granted host info and bluetooth device will be null
    if (QBluetoothLocalDevice::allDevices().length() == 0) {
        qWarning() << "There is no bluetooth on the device.";
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
    //! If permission is Undetermined request for it
    setPermission(qApp->checkPermission(QBluetoothPermission()));

    switch (mPermission) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(QBluetoothPermission(), cb);
    case Qt::PermissionStatus::Denied:
        qWarning() << "Bluetooth permsission is denied";
        emit error("Bluetooth permsission is denied");
        return;
    case Qt::PermissionStatus::Granted:
        break; //! Continue this method
    }
}

void BLEController::powerOn()
{
    if (isReady()) {
        mDevice->powerOn();
    } else {
        qWarning() << "Bluetooth controller is not ready";
    }
}

void BLEController::powerOff()
{
    if (isReady()) {
        mDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    } else {
        qWarning() << "Bluetooth controller is not ready";
    }
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

    connect(mDevice, &QBluetoothLocalDevice::hostModeStateChanged, this,
            &BLEController::bluetoothModeChanged);
}
