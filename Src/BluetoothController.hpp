#pragma once

#include <QObject>
#include <QQmlEngine>

#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>

//! \brief Aliasing Qt::PermissionStatus
using Permission = Qt::PermissionStatus;

/*!
 * \brief The BluetoothController class provides some hardware functionalities related to bluetooth
 * including getting permission, turning bluetooth on/off, etc
 */
class BluetoothController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool bluetoothAvailable READ bluetoothAvailable NOTIFY bluetoothAvailableChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

    Q_PROPERTY(HostMode bluetoothMode READ bluetoothMode NOTIFY bluetoothModeChanged)
    Q_PROPERTY(Permission permission READ permission NOTIFY permissionChanged)
    Q_PROPERTY(bool isReady READ isReady NOTIFY isReadyChanged)

public:
    /*!
     * \brief The HostMode enum is the mode of bluetooth device. Must match \a
     * QBluetoothLocalDevice::HostMode
     */
    enum HostMode {
        HostPoweredOff,
        HostConnectable,
        HostDiscoverable,
        HostDiscoverableLimitedInquiry
    };
    Q_ENUM(HostMode);

    /*!
     * \brief create Custom factory method for QML to make sure both QML and C++ accesses the same
     * instance
     * \param qmlEngine
     * \param jsEngine
     * \return
     */
    static BluetoothController* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);

    /*!
     * \brief instance Returns the singleton instance of this class. This is here for use in C++
     * \return
     */
    static BluetoothController& instance();

    /*!
     * \brief initialize Initializes bluetooth related properties, fetches required permissions.
     * This method must be called before accessing any bluetooth permission on Android.
     * \param Callback Can hold a callable (either JS callable or C++ std::function) to be called
     * when initialization is finished
     */
    Q_INVOKABLE void initialize(QVariant callback = QVariant());

    /*!
     * \brief name Returns the name of the host bluetooth device if any.
     * \return
     */
    QString name() const;

    /*!
     * \brief Getter to check if host has bluetooth
     * \return
     */
    bool bluetoothAvailable() const;

    /*!
     * \brief permission Returns the current permission of bluetooth permission
     * \return
     */
    Permission permission() const;

    /*!
     * \brief setPermission Sets the
     * \param permission
     */
    void setPermission(Permission permission);

    /*!
     * \brief isReady If the controller is set up, ie there is a bluetooth device on host and the
     * required permission is granted.
     * \return This is a shortcut for \a bluetoothAvailable() && permission() == Permission::Granted
     */
    bool isReady() const;

    /*!
     * \brief bluetoothMode Getter for the mode of the bluetooth device on the host
     * \return
     */
    HostMode bluetoothMode() const;

    /*!
     * \brief setHostMode
     * \param mode
     */
    void setBluetoothMode(HostMode mode);

    /*!
     * \brief powerOn
     * \param callback Can hold a callable to be called after bluetooth is successfully turned on
     */
    Q_INVOKABLE void powerOn(QVariant callback = QVariant());

    /*!
     * \brief powerOff
     */
    Q_INVOKABLE void powerOff();

private:
    explicit BluetoothController(QObject *parent = nullptr);

    /*!
     * \brief requestPermission Request bluetooth permission on Android/iOS
     */
    void requestPermission(std::function<void ()> cb);

    /*!
     * \brief setHostInfo Sets the info of host bluetooth device
     * \param bh
     */
    void setHostInfo(const QBluetoothHostInfo& bh);

    /*!
     * \brief getDefaultDevice
     * \param bld
     */
    void getDefaultDevice();

    /*!
     * \brief setupCallbackForPowerOn
     * \param callback
     */
    void setupCallbackForPowerOn(QVariant callback);

    /*!
     * \brief callVariantCallback
     * \param callback
     */
    void callVariantCallback(QVariant callback);

    /*!
     * \brief callJsCallback
     */
    void callJsCallback(QJSValue cb);

signals:
    void error(const QString& error);

    void bluetoothAvailableChanged();
    void nameChanged();
    void isReadyChanged();
    void permissionChanged();
    void bluetoothModeChanged();

private:
    //! \brief The single instance of this class
    static BluetoothController* sInstance;

    //! \brief mDevice The instance related to this local device
    QBluetoothLocalDevice* mDevice;

    //! \brief The info of the host bluetooth device
    QBluetoothHostInfo mHostInfo;

    //! \brief mPermission The permission permission of the bluetooth
    Permission mPermission;
};


inline bool BluetoothController::bluetoothAvailable() const
{
    return !mHostInfo.address().isNull();
}

inline Permission BluetoothController::permission() const
{
    return mPermission;
}

inline bool BluetoothController::isReady() const
{
    return bluetoothAvailable() && mPermission == Permission::Granted && mDevice;
}

inline BluetoothController::HostMode BluetoothController::bluetoothMode() const
{
    return HostMode(mDevice ? mDevice->hostMode()
                            : QBluetoothLocalDevice::HostMode::HostPoweredOff);
}
