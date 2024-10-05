#include "BLEPeripheral.h"

BLEPeripheral::BLEPeripheral(QObject *parent)
    : QObject{parent}
{}

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
