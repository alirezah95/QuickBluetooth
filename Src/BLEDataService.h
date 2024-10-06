#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QBluetoothUuid>

/*!
 * \brief The BLEDataService class describes a service in a BLE connection that can be read or write
 */
class BLEDataService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(DataType dataType READ dataType WRITE setDataType NOTIFY dataTypeChanged)
    Q_PROPERTY(QBluetoothUuid serviceUuid READ serviceUuid WRITE setServiceUuid NOTIFY serviceUuidChanged FINAL)
    Q_PROPERTY(QBluetoothUuid characterUuid READ characterUuid WRITE setCharacterUuid NOTIFY characterUuidChanged FINAL)
    Q_PROPERTY(QBluetoothUuid descriptorUuid READ descriptorUuid WRITE setDescriptorUuid NOTIFY descriptorUuidChanged FINAL)

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
     * \brief serviceUuid Service uuid getter
     * \return
     */
    QBluetoothUuid serviceUuid() const;
    /*!
     * \brief setServiceUuid Service Uuid setter
     * \param newServiceUuid
     */
    void setServiceUuid(const QBluetoothUuid& newServiceUuid);

    /*!
     * \brief characterUuid Character Uuid getter
     * \return
     */
    QBluetoothUuid characterUuid() const;
    /*!
     * \brief setCharacterUuid Character uuid setter
     * \param newCharacterUuid
     */
    void setCharacterUuid(const QBluetoothUuid& newCharacterUuid);

    /*!
     * \brief descriptorUuid Descriptor uuid getter
     * \return
     */
    QBluetoothUuid descriptorUuid() const;
    /*!
     * \brief setDescriptorUuid Descriptor uuid setter
     * \param newDescriptorUuid
     */
    void setDescriptorUuid(const QBluetoothUuid& newDescriptorUuid);

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

protected:
    //! \brief mServiceUuid
    QBluetoothUuid mServiceUuid;

    //! \brief mCharacterUuid
    QBluetoothUuid mCharacterUuid;

    //! \brief mDescriptorUuid
    QBluetoothUuid mDescriptorUuid;

    //! \brief mValue Value of this service data
    QVariant mValue;

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
