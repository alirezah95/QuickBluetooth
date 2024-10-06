#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <QLowEnergyController>

class BluetoothDeviceInfo;
class BLEDataService;

using ServicesListProperty = QQmlListProperty<BLEDataService>;

/*!
 * \brief The BLEPeripheral class sets up services, advertise data, etc to provide the role of
 * peripheral of a BLE connection
 */
class BLEPeripheral : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_CLASSINFO("DefaultProperty", "services")
    Q_PROPERTY(ServicesListProperty services READ services NOTIFY servicesChanged FINAL)
    Q_PROPERTY(QString localName READ localName WRITE setLocalName NOTIFY localNameChanged FINAL)

public:
    explicit BLEPeripheral(QObject *parent = nullptr);

    /*!
     * \brief initialize Instances the BLE cotroller
     * \note This method must be called after \ref BluetoothController::isReady is true
     */
    void initialize();

    /*!
     * \brief startAdvertising Starts the advertising process.
     */
    void startAdvertising();

    /*!
     * \brief device Gettero for \ref BluetoothDeviceInfo of currently connected device
     * \return
     */
    BluetoothDeviceInfo* device() const;

    /*!
     * \brief services Returns a \a QQmlListProperty instance for services property
     * \return
     */
    ServicesListProperty services();

    QString localName() const;
    void setLocalName(const QString& localName);

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

    //! ServicesListProperty methods
    static void servicesListAppend(ServicesListProperty* services, BLEDataService* service);
    static BLEDataService* servicesListAt(ServicesListProperty* services, qsizetype index);
    static qsizetype servicesListCount(ServicesListProperty* services);
    static void servicesListClear(ServicesListProperty* services);

signals:
    void deviceChanged();
    void servicesChanged();

    void localNameChanged();

private:
    //! \brief mController Holds the \a QLowEnergyController instance
    QLowEnergyController* mController;

    //! \brief mServices All the services for this \ref BLEPeripheral
    QList<BLEDataService*> mServices;

    //! \brief Connected device
    BluetoothDeviceInfo* mDevice;

    //! \brief mLocalName A name for advertising service
    QString mLocalName;
};


inline BluetoothDeviceInfo* BLEPeripheral::device() const
{
    return mDevice;
}
