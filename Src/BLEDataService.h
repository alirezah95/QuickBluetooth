#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothUuid>
#include <QLowEnergyController>
#include <QLowEnergyService>

/*!
 * \brief The BLEDataService class describes a service in a BLE connection that can be read or write
 */
class BLEDataService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(quint8 valueLength READ valueLength WRITE setValueLength NOTIFY valueLengthChanged FINAL)
    Q_PROPERTY(DataType dataType READ dataType WRITE setDataType NOTIFY dataTypeChanged)
    Q_PROPERTY(uint32_t serviceUuid READ serviceUuid WRITE setServiceUuid NOTIFY serviceUuidChanged FINAL)
    Q_PROPERTY(uint32_t characterUuid READ characterUuid WRITE setCharacterUuid NOTIFY characterUuidChanged FINAL)
    Q_PROPERTY(uint32_t descriptorUuid READ descriptorUuid WRITE setDescriptorUuid NOTIFY descriptorUuidChanged FINAL)

public:
    /*!
     * \brief The DataType enum represents the type of data this service is holding
     */
    enum DataType {
        Int,    //! 16 bit number
        Float,
        String
    };
    Q_ENUM(DataType)

    explicit BLEDataService(QObject *parent = nullptr);

    /*!
     * \brief isValid
     * \return
     */
    bool isValid() const;

    /*!
     * \brief setup Set up this \ref BLEDataService so it can be used
     * \return The \a QLowEnergyService of this
     */
    QLowEnergyService* setup(QLowEnergyController& leController);

    /*!
     * \brief writeValue Send the value to the other end of connection
     * \param value
     */
    Q_INVOKABLE void writeValue(const QVariant& value);

    /*!
     * \brief value Returns the current value
     * \return
     */
    QVariant value() const;

    /*!
     * \brief setValue Sets the value to \a value without any conversion required
     * \param value
     */
    void setValue(QVariant value);

    /*!
     * \brief setValue Sets the value of this \ref BLEDataService using the given \a QByteArray and
     * based on the data type of this service
     * \note This method does not send the value to the other end of a BLE connection
     * \param byteArray
     */
    void setValue(QByteArray byteArray);

    /*!
     * \brief dataType
     * \return
     */
    DataType dataType() const;
    /*!
     * \brief setDataType
     * \param dataType
     */
    void setDataType(DataType dataType);

    /*!
     * \brief serviceUuid Service uuid getter for QML
     * \return The uint32 form of service uuid
     */
    uint32_t serviceUuid() const;
    /*!
     * \brief serviceBluetoothUuid Returns the \a QBluetoothUuid of service
     */
    QBluetoothUuid serviceBluetoothUuid() const;
    /*!
     * \brief setServiceUuid Service Uuid setter
     * \param newServiceUuid
     */
    void setServiceUuid(uint32_t newServiceUuid);

    /*!
     * \brief characterUuid Character Uuid getterfor QML
     * \return The uint32 form of character uuid
     */
    uint32_t characterUuid() const;
    /*!
     * \brief characterBluetoothUuid Returns the \a QBluetoothUuid of character
     */
    QBluetoothUuid characterBluetoothUuid() const;
    /*!
     * \brief setCharacterUuid Character uuid setter
     * \param newCharacterUuid
     */
    void setCharacterUuid(uint32_t newCharacterUuid);

    /*!
     * \brief descriptorUuid Descriptor uuid getterfor QML
     * \return The uint32 form of descriptor uuid
     */
    uint32_t descriptorUuid() const;
    /*!
     * \brief descriptorBluetoothUuid Returns the \a QBluetoothUuid of descriptor
     */
    QBluetoothUuid descriptorBluetoothUuid() const;
    /*!
     * \brief setDescriptorUuid Descriptor uuid setter
     * \param newDescriptorUuid
     */
    void setDescriptorUuid(uint32_t newDescriptorUuid);

    /*!
     * \brief valueLength Getter for value length
     * \return
     */
    quint8 valueLength() const;
    /*!
     * \brief setValueLength Setter for value length
     * \param newValueLength
     */
    void setValueLength(quint8 newValueLength);

private slots:
    /*!
     * \brief onValueWritten This slot is connected to \a QLowEnergyService::characteristicWritten()
     * signal
     * \param characteristic
     * \param value
     */
    void onValueWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);

    /*!
     * \brief serviceStateChanged This slot is connected to \a QLowEnergyService::stateChanged()
     * signal if this \ref BLEDataService is used in a Cental
     * \param st
     */
    void serviceStateChanged(QLowEnergyService::ServiceState st);

private:
    /*!
     * \brief valueToByteArray
     * \param value
     * \return
     */
    QByteArray valueToByteArray(const QVariant& value);

    /*!
     * \brief byteArrayToValue
     * \param byteArray
     * \return
     */
    QVariant byteArrayToValue(const QByteArray& byteArray);

signals:
    /*!
     * \brief valueChanged This signal is emitted when the value of this data is changed
     */
    void valueChanged();

    /*!
     * \brief valueUpdated This signal is emitted when a new value is set for this charactristic
     * data from other end of the connection. This is a private signal and can't be emitted from
     * outside
     */
    void valueUpdated(QVariant value, QPrivateSignal);

    /*!
     * \brief serviceDataModified This signal is emmitted when
     * \param service
     * \param value
     */
    void serviceDataModified(BLEDataService* service, QByteArray value, QPrivateSignal);

    void dataTypeChanged();
    void serviceUuidChanged();
    void characterUuidChanged();
    void descriptorUuidChanged();

    void valueLengthChanged();

protected:
    //! \brief mServiceUuid Service class uuid for \a QLowEnergyServiceData
    QBluetoothUuid mServiceUuid;

    //! \brief mCharacterUuid Characteristic uuid for \a QLowEnergyCharacteristicData
    QBluetoothUuid mCharacterUuid;

    //! \brief mDescriptorUuid Descriptor uuid in \a QLowEnergyDescriptorData
    QBluetoothUuid mDescriptorUuid;

    //! \brief mValue Value of this service data
    QVariant mValue;

    //! \brief mValueLength Holds the length of the value for this service
    quint8 mValueLength;

    //! \brief mDataType Holds the data type of this service
    DataType mDataType;

    //! \brief mService The \a QLowEenergyService responsible for reading and writing for this \ref
    //! BLEDataService
    QLowEnergyService* mService;
};


inline bool BLEDataService::isValid() const
{
    return !mServiceUuid.isNull()
           && !mCharacterUuid.isNull()
           && !mDescriptorUuid.isNull()
           && mService;
}

inline QVariant BLEDataService::value() const
{
    return mValue;
}

inline BLEDataService::DataType BLEDataService::dataType() const
{
    return mDataType;
}

inline QBluetoothUuid BLEDataService::serviceBluetoothUuid() const
{
    return mServiceUuid;
}

inline QBluetoothUuid BLEDataService::characterBluetoothUuid() const
{
    return mCharacterUuid;
}

inline QBluetoothUuid BLEDataService::descriptorBluetoothUuid() const
{
    return mDescriptorUuid;
}
