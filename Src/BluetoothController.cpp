#include "BluetoothController.h"

#include <QCoreApplication>
#include <QBluetoothLocalDevice>
#include <QBluetoothPermission>

//! \brief Initialize the single instance
BluetoothController* BluetoothController::sInstance = nullptr;

BluetoothController* BluetoothController::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    return &instance();
}

BluetoothController &BluetoothController::instance()
{
    if (!sInstance) {
        sInstance = new BluetoothController;
    }

    return *sInstance;
}

BluetoothController::BluetoothController(QObject *parent)
    : QObject{ parent }
    , mDevice { nullptr }
    , mPermission { Qt::PermissionStatus::Undetermined }
{ }

void BluetoothController::initialize()
{
    requestPermission(std::bind(&BluetoothController::initialize, this));

    //! Note if bluetooth permission is not granted host info and bluetooth device will be null
    if (QBluetoothLocalDevice::allDevices().length() == 0) {
        qWarning() << "There is no bluetooth on the device.";
    } else {
        setHostInfo(QBluetoothLocalDevice::allDevices().at(0));
        getDefaultDevice();
    }
}

QString BluetoothController::name() const
{
    return mHostInfo.name();
}

void BluetoothController::setPermission(Permission permission)
{
    if (mPermission == permission) {
        return;
    }

    mPermission = permission;
    emit permissionChanged();
}

void BluetoothController::setBluetoothMode(HostMode mode)
{
    if (bluetoothMode() == mode) {
        return;
    }

    mDevice->setHostMode(QBluetoothLocalDevice::HostMode(mode));
    emit bluetoothModeChanged();
}

void BluetoothController::requestPermission(std::function<void ()> cb)
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

void BluetoothController::powerOn()
{
    if (isReady()) {
        mDevice->powerOn();
    } else {
        qWarning() << "Bluetooth controller is not ready";
    }
}

void BluetoothController::powerOff()
{
    if (isReady()) {
        mDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    } else {
        qWarning() << "Bluetooth controller is not ready";
    }
}

void BluetoothController::setHostInfo(const QBluetoothHostInfo& bh)
{
    if (mHostInfo == bh) {
        return;
    }

    mHostInfo = bh;

    emit nameChanged();
    emit bluetoothAvailableChanged();
}

void BluetoothController::getDefaultDevice()
{
    if (mDevice) {
        delete mDevice;
    }

    mDevice = new QBluetoothLocalDevice();
    emit bluetoothModeChanged();

    connect(mDevice, &QBluetoothLocalDevice::hostModeStateChanged, this,
            &BluetoothController::bluetoothModeChanged);
}
