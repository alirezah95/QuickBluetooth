#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothUuid>
#include <QLowEnergyController>

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
        Int,
        Float,
        String
    };
    Q_ENUM(DataType)

    explicit BLEDataService(QObject *parent = nullptr);

    /*!
     * \brief setup Set up this \ref BLEDataService so it can be used
     */
    bool setup(QLowEnergyController& leController);

    /*!
     * \brief value Returns the current value
     * \return
     */
    QVariant value() const;

    /*!
     * \brief setValue Sets the value of this \ref BLEDataService using the given \a QByteArray and
     * based on the data type of this service
     * \note This method does not send the value to the other end of a BLE connection
     * \param value
     */
    void setValue(QByteArray value);

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

signals:
    /*!
     * \brief valueChanged This signal is emitted when the value of this data is changed
     */
    void valueChanged();

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
};


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
