#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothDeviceInfo>
#include <QBluetoothAddress>

/*!
 * \brief The BluetoothDeviceInfo class represents a nearby bluetooth device
 */
class BluetoothDeviceInfo : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("BluetoothDeviceInfo can only be created from C++")

    Q_PROPERTY(QString name READ name NOTIFY deviceChanged);
    Q_PROPERTY(QString address READ address NOTIFY deviceChanged);

public:
    BluetoothDeviceInfo(const QBluetoothDeviceInfo& device, QObject *parent = nullptr);

    /*!
     * \brief name Getter for this bluetooth device name
     * \return
     */
    QString name() const;

    /*!
     * \brief address Getter for this bluetooth device address
     * \return
     */
    QString address() const;

    /*!
     * \brief setDevice Set the \a QBluetoothDeviceInfo for this \ref BluetoothDeviceInfo
     * \param other
     */
    void setDevice(const QBluetoothDeviceInfo& other);

    /*!
     * \brief Getter for the \a QBluetoothDeviceInfo of this instance
     */
    const QBluetoothDeviceInfo& device() const;

signals:
    void deviceChanged();

private:
    //! \brief The \a QBluetoothDeviceInfo related to this instance
    QBluetoothDeviceInfo mDevice;
};


inline QString BluetoothDeviceInfo::name() const
{
    return mDevice.name();
}

inline QString BluetoothDeviceInfo::address() const
{
    return mDevice.address().toString();
}

inline const QBluetoothDeviceInfo& BluetoothDeviceInfo::device() const
{
    return mDevice;
}
