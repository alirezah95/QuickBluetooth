#include "BLERole.h"
#include "BLEDataService.h"
#include "BluetoothDeviceInfo.h"

BLERole::BLERole(QObject *parent)
    : QObject{ parent }
    , mController { nullptr }
    , mDevice { nullptr }
{}

void BLERole::serviceAdd(BLEDataService* ble)
{
    if (!ble) {
        return;
    }

    mServices.append(ble);
    emit servicesChanged();
}

qsizetype BLERole::serviceCount() const
{
    return mServices.length();
}

BLEDataService* BLERole::serviceAt(qsizetype index)
{
    return index >= 0 && index < mServices.length() ? mServices.at(index) : nullptr;
}

void BLERole::serviceClear()
{
    qDeleteAll(mServices);
    mServices.clear();
}

ServicesListProperty BLERole::services()
{
    return QQmlListProperty<BLEDataService>(this, this,
                                            &BLERole::servicesListAppend,
                                            &BLERole::servicesListCount,
                                            &BLERole::servicesListAt,
                                            &BLERole::servicesListClear);
}

void BLERole::servicesListAppend(ServicesListProperty* services, BLEDataService* service)
{
    reinterpret_cast<BLERole*>(services->object)->serviceAdd(service);
}

BLEDataService* BLERole::servicesListAt(ServicesListProperty* services, qsizetype index)
{
    return reinterpret_cast<BLERole*>(services->object)->serviceAt(index);
}

qsizetype BLERole::servicesListCount(ServicesListProperty* services)
{
    return reinterpret_cast<BLERole*>(services->object)->serviceCount();
}

void BLERole::servicesListClear(ServicesListProperty* services)
{
    reinterpret_cast<BLERole*>(services->object)->serviceClear();
}
