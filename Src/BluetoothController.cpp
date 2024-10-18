#include "BluetoothController.hpp"

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
{
    //! If permission is Undetermined request for it
    mPermission = qApp->checkPermission(QBluetoothPermission());
}

void BluetoothController::initialize(QVariant callback)
{
    //! Update permission first
    setPermission(qApp->checkPermission(QBluetoothPermission()));

    if (mPermission == Qt::PermissionStatus::Undetermined) {
        return requestPermission(std::bind(&BluetoothController::initialize, this, callback));
    }

    if (mPermission == Qt::PermissionStatus::Granted) {
        //! Note if bluetooth permission is not granted host info and bluetooth device will be null
        if (QBluetoothLocalDevice::allDevices().length() == 0) {
            qWarning() << "There is no bluetooth on the device.";
        } else {
            setHostInfo(QBluetoothLocalDevice::allDevices().at(0));
            getDefaultDevice();
        }
    }

    callVariantCallback(callback);
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
    switch (mPermission) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(QBluetoothPermission(), cb);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning() << "Bluetooth permsission is denied";
        emit error("Bluetooth permsission is denied");
        break;
    case Qt::PermissionStatus::Granted:
        break; //! Continue this method
    }
}

void BluetoothController::powerOn(QVariant callback)
{
    if (isReady()) {
        //! If device is already powered on, return
        if (bluetoothMode() != BluetoothController::HostPoweredOff) {
            //! Already on
            qDebug() << "Already on";
            callVariantCallback(callback);
            return;
        }

        //! Make sure the callback is called when devices turns on
        setupCallbackForPowerOn(callback);
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

void BluetoothController::setupCallbackForPowerOn(QVariant callback)
{
    connect(mDevice, &QBluetoothLocalDevice::hostModeStateChanged, this,
            std::bind(&BluetoothController::callVariantCallback, this, callback),
            Qt::SingleShotConnection);
}

void BluetoothController::callVariantCallback(QVariant callback)
{
    if (callback.canConvert<std::function<void ()>>()) {
        callback.value<std::function<void ()>>()();
    } else if (callback.canConvert<QJSValue>()) {
        callJsCallback(callback.value<QJSValue>());
    }
}

void BluetoothController::callJsCallback(QJSValue cb)
{
    if (!cb.isCallable()) {
        return;
    }

    QJSValue result = cb.call();

    if (result.isError()) {
        qDebug("%s:%s: %s",
               qPrintable(result.property("fileName").toString()),
               qPrintable(result.property("lineNumber").toString()),
               qPrintable(result.toString().toStdString().c_str()));
    }
}
