#include "BLEPeripheral.h"

BLEPeripheral::BLEPeripheral(QObject *parent)
    : QObject{parent}
{}

ServicesListProperty BLEPeripheral::services()
{
    return QQmlListProperty<BLEDataService>(this, this,
                                            &BLEPeripheral::servicesListAppend,
                                            &BLEPeripheral::servicesListCount,
                                            &BLEPeripheral::servicesListAt,
                                            &BLEPeripheral::servicesListClear);
}

void BLEPeripheral::serviceAdd(BLEDataService* ble)
{

}

qsizetype BLEPeripheral::serviceCount() const
{
    return mServices.length();
}

BLEDataService* BLEPeripheral::serviceAt(qsizetype index)
{
    return index >= 0 && index < mServices.length() ? mServices.at(index) : nullptr;
}

void BLEPeripheral::serviceClear()
{
    qDeleteAll(mServices);
    mServices.clear();
}

void BLEPeripheral::servicesListAppend(ServicesListProperty* services, BLEDataService* service)
{
    reinterpret_cast<BLEPeripheral*>(services->object)->serviceAdd(service);
}

BLEDataService* BLEPeripheral::servicesListAt(ServicesListProperty* services, qsizetype index)
{
    return reinterpret_cast<BLEPeripheral*>(services->object)->serviceAt(index);
}

qsizetype BLEPeripheral::servicesListCount(ServicesListProperty* services)
{
    return reinterpret_cast<BLEPeripheral*>(services->object)->serviceCount();
}

void BLEPeripheral::servicesListClear(ServicesListProperty* services)
{
    reinterpret_cast<BLEPeripheral*>(services->object)->serviceClear();
}
