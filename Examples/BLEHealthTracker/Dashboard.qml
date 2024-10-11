import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import QuickBluetooth


Page {
    id: root

    //* Object Properites
    //* ************************************/

    implicitWidth: 390
    implicitHeight: 770
    verticalPadding: 24
    horizontalPadding: 20
    bottomPadding: 12

    header: RowLayout {
        Label {
            Layout.fillWidth: true
            leftPadding: 16
            rightPadding: 16
            font {
                weight: Font.ExtraBold
                pointSize: 28
            }
            text: "Health Tracker Example"
            wrapMode: "Wrap"
        }
    }

    //* Children
    //* ************************************/

    CustomBLECentral {
        id: bleCentral

        property bool isConnected: state === BLERole.ConnectedState

        onHeartRateUpdated: (value) => internal.heartRate = value;
        onBodyTempUpdated: (value) => internal.bodyTemperature = value;
        onBloodPressureUpdated: (value) => internal.bloodPressure = value;
        onStepsCountUpdated: (value) => internal.stepsCount = value
        onDistTravelledUpdated: (value) => internal.distanceTravelled = value
        onCaloriesBurntUpdated: (value) => internal.caloriesBurnt = value
    }

    SearchDevicePopup {
        y: connectStatusRow.height + mainLay.spacing + (cardsGrid.height - height) / 2
        x: (parent.width - width) / 2
        width: parent.width * 0.85
        height: cardsGrid.height * 0.9
        visible: !bleCentral.isConnected
        centeral: bleCentral
    }

    QtObject {
        id: internal

        property real heartRate: 0.0
        property real bodyTemperature: 0.0
        property real bloodPressure: 0.0
        property real stepsCount: 0.0
        property real distanceTravelled: 0.0
        property real caloriesBurnt: 0.0
    }

    ListModel {
        id: cardModel

        ListElement {
            icon: "heart-rate-svgrepo"
            title: "Heart\nRate"
            value: () => internal.heartRate
        }
        ListElement {
            icon: "temperature"
            title: "Body\nTemperature"
            value: () => internal.bodyTemperature
        }
        ListElement {
            icon: "blood-pressure-icon"
            title: "Blood\nPressure"
            value: () => internal.bloodPressure
        }
        ListElement {
            icon: "leg-footprints-icon"
            title: "Steps\nCount"
            value: () => internal.stepsCount
        }
        ListElement {
            icon: "distance-svgrepo"
            title: "Distance\nTravelled"
            value: () => internal.distanceTravelled
        }
        ListElement {
            icon: "calories-vecteezy"
            title: "Calories\nBurnt"
            value: () => internal.caloriesBurnt
        }
    }

    ColumnLayout {
        id: mainLay
        anchors.fill: parent
        spacing: 24

        //! Status label
        RowLayout {
            id: connectStatusRow

            ColorImage {
                Layout.preferredHeight: connectStateLbl.height * 1.2
                Layout.preferredWidth: height * implicitWidth / implicitHeight
                source: "qrc:/BLEHealthTracker/Res/bluetooth-icon.png"
                color: bleCentral.isConnected ? undefined : Material.hintTextColor
            }

            Label {
                id: connectStateLbl
                font.pointSize: Application.font.pointSize
                text: bleCentral.isConnected ? "Connected" : "Disconnected"
                color: bleCentral.isConnected ? Material.accent : Material.foreground
            }
        }

        GridLayout {
            id: cardsGrid
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            rowSpacing: 16
            columnSpacing: 24
            columns: 2

            Repeater {
                model: cardModel

                delegate: DataCard {
                    id: delegate

                    required property var modelData
                    required property int index

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    implicitWidth: 1;
                    implicitHeight: 1
                    icon: "qrc:/BLEHealthTracker/Res/%1".arg(modelData.icon)
                    title: modelData.title
                    value: modelData.value()
                }
            }
        }

        RowLayout {
            Layout.leftMargin: 32
            Layout.rightMargin: 32
            spacing: 24

            Button {
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Material.roundedScale: Material.SmallScale
                highlighted: true
                text: "Connect"
            }

            Button {
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                Material.roundedScale: Material.SmallScale
                text: "Disconnect"
            }
        }
    }
}
