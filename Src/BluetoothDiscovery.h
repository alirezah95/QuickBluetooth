#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothDeviceDiscoveryAgent>

class BluetoothDeviceInfo;

using DiscoveryMethods = QBluetoothDeviceDiscoveryAgent::DiscoveryMethods;
using DiscoveryMethod = QBluetoothDeviceDiscoveryAgent::DiscoveryMethod;

/*!
 * \brief The BluetoothDiscovery class provides functionality to search and view nearby bluetooth
 * devices
 */
class BluetoothDiscovery : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<BluetoothDeviceInfo*> devices READ devices NOTIFY devicesChanged)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(int timeOut READ timeOut WRITE setTimeOut NOTIFY timeOutChanged)
    Q_PROPERTY(DiscoveryMethods methods READ methods WRITE setMethods NOTIFY methodsChanged)

public:
    explicit BluetoothDiscovery(QObject *parent = nullptr);

    /*!
     * \brief start Starts the start operation
     */
    Q_INVOKABLE void start();

    /*!
     * \brief stop Stops current search for bluetooth devices
     */
    Q_INVOKABLE void stop();

    /*!
     * \brief method Returns the method of discovery
     * \return
     */
    DiscoveryMethods methods() const;

    /*!
     * \brief setMethod Sets the discovery method (only applied on next search)s
     * \param method
     */
    void setMethods(DiscoveryMethods methods);

    /*!
     * \brief isActive Returns true if scanner is searching for devices
     * \return
     */
    bool isActive() const;

    /*!
     * \brief setIsActive
     * \param isActive
     * \return
     */
    void setIsActive(bool isActive);

    /*!
     * \brief timeOut Getter for discovery timeout
     * \return
     */
    int timeOut() const;

    /*!
     * \brief setTimeOut Setter for recovery timeout
     * \param timeout
     */
    void setTimeOut(int timeout);

    /*!
     * \brief devices Getter for the list of devices
     * \return
     */
    const QList<BluetoothDeviceInfo*>& devices();

signals:
    void devicesChanged();
    void isActiveChanged();
    void timeOutChanged();
    void methodsChanged();

private slots:
    /*!
     * \brief addDevice Add a new bluetooth device to the list of devices
     * \param dev
     */
    void addDevice(const QBluetoothDeviceInfo& dev);

    /*!
     * \brief errorOccurred
     * \param error
     */
    void errorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);

private:
    //! \brief The discovery agent
    QBluetoothDeviceDiscoveryAgent* mDevDiscovery;

    //! \brief A list of available devices
    QList<BluetoothDeviceInfo*> mDevices;

    //! \brief Is scanner running
    bool mIsActive;

    //! \brief mDiscoveryMethods
    DiscoveryMethods mDiscoveryMethods;

    //! \brief mDeviceCoreConfig Determines what kind of devices should be added to the list when
    //! discoverd. This depends on \ref mDiscoverMethods
    int mDeviceCoreConfig;
};


inline DiscoveryMethods BluetoothDiscovery::methods() const
{
    return mDiscoveryMethods;
}

inline bool BluetoothDiscovery::isActive() const
{
    return mDevDiscovery && mDevDiscovery->isActive();
}

inline int BluetoothDiscovery::timeOut() const
{
    return mDevDiscovery ? mDevDiscovery->lowEnergyDiscoveryTimeout() : 0;
}

inline const QList<BluetoothDeviceInfo*>& BluetoothDiscovery::devices()
{
    return mDevices;
}
