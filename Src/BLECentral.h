#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QLowEnergyService>
#include <QLowEnergyController>

#include "BLERole.h"

/*!
 * \brief The BLECentral class handles the Central role functionality in a BLE connection
 */
class BLECentral : public BLERole
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(BluetoothDeviceInfo* device READ device WRITE setDevice NOTIFY deviceChanged)

public:
    explicit BLECentral(QObject *parent = nullptr);

    /*!
     * \brief setDevice Sets the device that this \ref BLECentral should be connected to as
     * peripheral
     * \param device
     */
    void setDevice(BluetoothDeviceInfo* device);

    /*!
     * \brief disconnect Disconnect from current peripheral if any
     */
    void disconnect();

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
    /*!
     * \brief serviceDiscovered This is called when a new service is found in the peripheral. The
     * discovered service is added to controller if it exists in the \ref mServices
     * \param uuid The \a QBluetoothUuid of the discovered service
     */
    void serviceDiscovered(const QBluetoothUuid& uuid);
};
