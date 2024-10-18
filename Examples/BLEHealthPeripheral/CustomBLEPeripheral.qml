import QtQuick

import QuickBluetooth 1.0


BLEPeripheral {
    id: peripheral

    //* Property Declarations
    //* ************************************/

    signal heartRateUpdated(var value)
    signal bodyTempUpdated(var value)
    signal bloodPressureUpdated(var value)
    signal stepsCountUpdated(var value)
    signal distTravelledUpdated(var value)
    signal caloriesBurntUpdated(var value)

    readonly property int heartRateUuid:        0xFFFF
    readonly property int bodyTempUuid:         0xFFFE
    readonly property int bloodPressureUuid:    0xFFF0
    readonly property int stepsCountUuid:       0xFFFC
    readonly property int distTravelledUuid:    0xFFFB
    readonly property int caloriesBurntUuid:    0xFFFA

    //* Children
    //* ************************************/

    BLEDataService {
        id: heartRate
        serviceUuid: heartRateUuid
        characterUuid: heartRateUuid
        dataType: BLEDataService.Int

        onValueUpdated: (value) => heartRateUpdated(value)
    }

    BLEDataService {
        id: bodyTemp
        serviceUuid: bodyTempUuid
        characterUuid: bodyTempUuid
        dataType: BLEDataService.Float

        onValueUpdated: (value) => bodyTempUpdated(value)
    }

    BLEDataService {
        id: bloodPressure
        serviceUuid: bloodPressureUuid
        characterUuid: bloodPressureUuid
        dataType: BLEDataService.Int

        onValueUpdated: (value) => bloodPressureUpdated(value)
    }

    BLEDataService {
        id: stepsCount
        serviceUuid: stepsCountUuid
        characterUuid: stepsCountUuid
        dataType: BLEDataService.Int

        onValueUpdated: (value) => stepsCountUpdated(value)
    }

    BLEDataService {
        id: distTravelled
        serviceUuid: distTravelledUuid
        characterUuid: distTravelledUuid
        dataType: BLEDataService.Float

        onValueUpdated: (value) => distTravelledUpdated(value)
    }

    BLEDataService {
        id: caloriesBurnt
        serviceUuid: caloriesBurntUuid
        characterUuid: caloriesBurntUuid
        dataType: BLEDataService.Int

        onValueUpdated: (value) => caloriesBurntUpdated(value)
    }

    //* Methods
    //* ************************************/

    function setHeartRate(value)
    {
        heartRate.writeValue(value);
    }

    function setBodyTemp(value)
    {
        bodyTemp.writeValue(value);
    }

    function setBloodPressure(value)
    {
        bloodPressure.writeValue(value);
    }

    function setStepsCount(value)
    {
        stepsCount.writeValue(value);
    }

    function setDistTravelled(value)
    {
        distTravelled.writeValue(value);
    }

    function setCaloriesBurnt(value)
    {
        caloriesBurnt.writeValue(value);
    }
}
