#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQmlListProperty>
#include <QLowEnergyController>

#include "BLERole.hpp"


/*!
 * \brief The BLEPeripheral class sets up services, advertise data, etc to provide the role of
 * peripheral of a BLE connection
 */
class BLEPeripheral : public BLERole
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString localName READ localName WRITE setLocalName NOTIFY localNameChanged FINAL)
    Q_PROPERTY(BluetoothDeviceInfo* device READ device WRITE setDevice NOTIFY deviceChanged)

public:
    void setDevice(BluetoothDeviceInfo* d)  { }
    explicit BLEPeripheral(QObject *parent = nullptr);

    /*!
     * \brief initialize Instances the BLE cotroller
     * \note This method must be called after \ref BluetoothController::isReady is true
     */
    Q_INVOKABLE void initialize();

    /*!
     * \brief startAdvertising Starts the advertising process.
     */
    Q_INVOKABLE void startAdvertising();

    QString localName() const;
    void setLocalName(const QString& localName);

protected:
    /*!
     * \brief Override \ref BLERole::readData() to read data from other end
     * \param uuid
     */
    virtual void readData(const QBluetoothUuid& uuid) override;

    /*!
     * \brief Override \ref BLERole::writeData() to write data to other end
     * \param uuid
     * \param value
     */
    virtual void writeData(const QBluetoothUuid& uuid, const QVariant& value) override;

private slots:
    void onErrorOccured(QLowEnergyController::Error error);

signals:
    void localNameChanged();

private:

    //! \brief mLocalName A name for advertising service
    QString mLocalName;
};
