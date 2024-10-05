#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>

class BluetoothDeviceInfo;
class BLEDataService;

/*!
 * \brief The BLEPeripheral class sets up services, advertise data, etc to provide the role of
 * peripheral of a BLE connection
 */
class BLEPeripheral : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit BLEPeripheral(QObject *parent = nullptr);

    /*!
     * \brief device Gettero for \ref BluetoothDeviceInfo of currently connected device
     * \return
     */
    BluetoothDeviceInfo* device() const;

private:
    /*!
     * \brief serviceAdd Adds the \ref BLEDataService instance to the list of servcies for
     * this peripheral
     * \param ble
     */
    void serviceAdd(BLEDataService* ble);

    /*!
     * \brief serviceCount Returns the number of services for this \ref BLEPeripheral
     * \return
     */
    qsizetype serviceCount() const;

    /*!
     * \brief serviceAt Returns the BLEDataService at the given index
     * \param index
     * \return
     */
    BLEDataService* serviceAt(qsizetype index);

    /*!
     * \brief serviceClear Removes all services
     */
    void serviceClear();

signals:
    void deviceChanged();

private:
    //! \brief mServices All the services for this \ref BLEPeripheral
    QList<BLEDataService*> mServices;

    //! \brief Connected device
    BluetoothDeviceInfo* mDevice;
};


inline BluetoothDeviceInfo* BLEPeripheral::device() const
{
    return mDevice;
}
