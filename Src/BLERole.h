#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <QLowEnergyController>

#include "BluetoothDeviceInfo.h"

class BluetoothDeviceInfo;
class BLEDataService;

using ServicesListProperty = QQmlListProperty<BLEDataService>;

/*!
 * \brief The BLERole class is the base class of BLE peripherals and centrals which provides the
 * common functionalities in both
 */
class BLERole : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("BLERole is an interface")

    Q_CLASSINFO("DefaultProperty", "services")
    Q_PROPERTY(ServicesListProperty services READ services NOTIFY servicesChanged FINAL)
    Q_PROPERTY(BluetoothDeviceInfo* device READ device NOTIFY deviceChanged)

public:
    explicit BLERole(QObject *parent = nullptr);

    /*!
     * \brief services Returns a \a QQmlListProperty instance for services property
     * \return
     */
    ServicesListProperty services();

    /*!
     * \brief device Getter for the device info of this \ref BLECentral
     * \return
     */
    BluetoothDeviceInfo* device() const;

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

protected:
    /*!
     * \brief Subclasses should implment this method to read data from othe end of BLE connection
     * \param uuid
     */
    virtual void readData(const QBluetoothUuid& uuid) = 0;

    /*!
     * \brief Subclasses should implment this method to write data to other end of BLE connection
     * \param uuid
     * \param value
     */
    virtual void writeData(const QBluetoothUuid& uuid, const QVariant& value) = 0;

protected:
    //! ServicesListProperty methods
    static void servicesListAppend(ServicesListProperty* services, BLEDataService* service);
    static BLEDataService* servicesListAt(ServicesListProperty* services, qsizetype index);
    static qsizetype servicesListCount(ServicesListProperty* services);
    static void servicesListClear(ServicesListProperty* services);

signals:
    void deviceChanged();
    void servicesChanged();

protected:
    //! \brief mController Holds the \a QLowEnergyController instance
    QLowEnergyController* mController;

    //! \brief Connected device
    BluetoothDeviceInfo* mDevice;

    //! \brief mServices All the services for this \ref BLEPeripheral
    QList<BLEDataService*> mServices;
};


inline BluetoothDeviceInfo* BLERole::device() const
{
    return mDevice;
}
