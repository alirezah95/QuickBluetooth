#pragma once

#include <QObject>
#include <QQmlEngine>

#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>

//! \brief Aliasing Qt::PermissionStatus
using Permission = Qt::PermissionStatus;

/*!
 * \brief The BLEController class provides some hardware functionalities related to bluetooth
 * including getting permission, turning bluetooth on/off, etc
 */
class BLEController : public QObject
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
    static BLEController* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);

    /*!
     * \brief instance Returns the singleton instance of this class. This is here for use in C++
     * \return
     */
    static BLEController& instance();

    /*!
     * \brief initialize Initializes bluetooth related properties, fetches required permissions.
     * This method must be called before accessing any bluetooth permission on Android.
     */
    void initialize();

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
     * \brief powerOn
     */
    Q_INVOKABLE void powerOn();

    /*!
     * \brief powerOff
     */
    Q_INVOKABLE void powerOff();

private:
    explicit BLEController(QObject *parent = nullptr);

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

signals:
    void error(const QString& error);

    void bluetoothAvailableChanged();
    void nameChanged();
    void isReadyChanged();
    void permissionChanged();
    void bluetoothModeChanged();

private:
    //! \brief The single instance of this class
    static BLEController* sInstance;

    //! \brief mDevice The instance related to this local device
    QBluetoothLocalDevice* mDevice;

    //! \brief The info of the host bluetooth device
    QBluetoothHostInfo mHostInfo;

    //! \brief mPermission The permission permission of the bluetooth
    Permission mPermission;
};


inline bool BLEController::bluetoothAvailable() const
{
    return !mHostInfo.address().isNull();
}

inline Permission BLEController::permission() const
{
    return mPermission;
}

inline bool BLEController::isReady() const
{
    return bluetoothAvailable() && mPermission == Permission::Granted && mDevice;
}

inline BLEController::HostMode BLEController::bluetoothMode() const
{
    return HostMode(mDevice ? mDevice->hostMode()
                            : QBluetoothLocalDevice::HostMode::HostPoweredOff);
}
