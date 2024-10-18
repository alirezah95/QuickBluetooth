import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import QuickBluetooth 1.0

Page {
    id: root

    //* Object Properties
    //* ************************************/

    header: Label {
        leftPadding: 16
        rightPadding: 16
        font {
            weight: Font.ExtraBold
            pointSize: 28
        }
        text: "Health Tracker Device"
        wrapMode: "Wrap"
    }
    //* Children
    //* ************************************/

    CustomBLEPeripheral {
        id: peripheral
        localName: "QuickBluetooth"
    }

    QtObject {
        id: internal

        property bool random: true

        property real heartRate: 60
        property real bodyTemperature: 37
        property real bloodPressure: 64
        property real stepsCount: 2400
        property real distanceTravelled: 10.5
        property real caloriesBurnt: 1200
    }

    ListModel {
        id: cardModel

        ListElement {
            title: "Heart\nRate"
            value: () => internal.heartRate
            setValue: (value) => {
                          peripheral.setHeartRate(value);
                          internal.heartRate = value
                      }
            step: 1; min: 50; max: 100
        }
        ListElement {
            title: "Body\nTemp"
            value: () => internal.bodyTemperature
            setValue: (value) => {
                          peripheral.setBodyTemp(value);
                          internal.bodyTemperature = value
                      }
            step: .5; min: 35; max: 45
        }
        ListElement {
            title: "Blood\nPressure"
            value: () => internal.bloodPressure
            setValue: (value) => {
                          peripheral.setBloodPressure(value);
                          internal.bloodPressure = value
                      }
            step: 1; min: 50; max: 100
        }
        ListElement {
            title: "Steps\nCount"
            value: () => internal.stepsCount
            setValue: (value) => {
                          peripheral.setStepsCount(value);
                          internal.stepsCount = value
                      }
            step: 120; min: 500; max: 10000
        }
        ListElement {
            title: "Distance\nTravelled"
            value: () => internal.distanceTravelled
            setValue: (value) => {
                          peripheral.setDistTravelled(value);
                          internal.distanceTravelled = value
                      }
            step: 6; min: 5; max: 100
        }
        ListElement {
            title: "Calories\nBurnt"
            value: () => internal.caloriesBurnt
            setValue: (value) => {
                          peripheral.setCaloriesBurnt(value);
                          internal.caloriesBurnt = value
                      }
            step: 25; min: 500; max: 3000
        }
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView

            property real titleWidth: 0

            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            spacing: 16
            model: cardModel

            delegate: Pane {
                id: cardDelegate

                required property var model
                required property int index

                Material.elevation: 4
                width: ListView.view.width

                RowLayout {
                    anchors.fill: parent
                    spacing: 24

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: listView.titleWidth
                        font.bold: true
                        text: model.title
                        horizontalAlignment: "AlignHCenter"
                        verticalAlignment: "AlignVCenter"

                        onWidthChanged: if (width > listView.titleWidth) listView.titleWidth = width
                    }

                    TextField {
                        id: valueTf
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        implicitHeight: Material.textFieldHeight * 0.8
                        topPadding: 8; bottomPadding: 8
                        text: model.value()
                        horizontalAlignment: "AlignHCenter"
                        validator: DoubleValidator {
                            bottom: 0; top: 30000
                        }
                    }

                    RowLayout {
                        spacing: 12

                        Button {
                            Material.roundedScale: Material.ExtraSmallScale
                            implicitWidth: plusBtn.implicitWidth
                            implicitHeight: plusBtn.implicitHeight
                            font: plusBtn.font
                            topInset: 2; bottomInset: 2; rightInset: 2; leftInset: 2
                            leftPadding: 8; rightPadding: leftPadding
                            topPadding: 4; bottomPadding: topPadding
                            highlighted: true
                            text: "-"
                            autoRepeat: true
                            autoRepeatDelay: 1000
                            autoRepeatInterval: 500

                            onClicked: {
                                model.setValue(Math.min(model.max, Math.max(model.value() - model.step, model.min)))
                            }
                        }

                        Button {
                            id: plusBtn
                            Material.roundedScale: Material.ExtraSmallScale
                            implicitWidth: implicitContentWidth + leftPadding + rightPadding
                            implicitHeight: implicitContentHeight + topPadding + bottomPadding
                            font {
                                bold: true
                                pointSize: Application.font.pointSize * 1.5
                            }
                            topInset: 2; bottomInset: 2; rightInset: 2; leftInset: 2
                            leftPadding: 10; rightPadding: leftPadding
                            topPadding: 6; bottomPadding: topPadding
                            highlighted: true
                            text: "+"
                            autoRepeat: true
                            autoRepeatDelay: 1000
                            autoRepeatInterval: 500

                            onClicked: {
                                model.setValue(Math.min(model.max, Math.max(model.value() + model.step, model.min)))
                            }
                        }
                    }
                }
            }
        }

        Button {
            Material.roundedScale: Material.SmallScale
            Layout.alignment: Qt.AlignHCenter
            leftPadding: 48; rightPadding: leftPadding
            highlighted: true
            text: "Start Advertising"

            onClicked: {
                BluetoothController.initialize();
                peripheral.initialize();
                peripheral.startAdvertising();
            }
        }
    }
}
