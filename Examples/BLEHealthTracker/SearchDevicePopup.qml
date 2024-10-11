import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import QuickBluetooth 1.0

Popup {
    id: root

    //* Property Declarations
    //* ************************************/

    property BLECentral centeral

    //* Object Properties
    //* ************************************/

    Material.roundedScale: Material.MediumScale
    modal: true
    closePolicy: "NoAutoClose"

    //* Children
    //* ************************************/

    BluetoothDiscovery {
        id: bluDiscovery
    }

    ColumnLayout {
        id: mainLay
        anchors.fill: parent

        spacing: 16

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: bluDiscovery.devices
            delegate: ItemDelegate {
                required property BluetoothDeviceInfo modelData
                required property int index

                contentItem: ColumnLayout {
                    Label {
                        font.bold: true
                        text: modelData?.name ?? ""
                    }

                    Label {
                        opacity: 0.75
                        text: modelData?.address ?? ""
                    }
                }

                onClicked: {
                    if (centeral) {
                        centeral.device = modelData;
                    }
                }
            }
        }

        RowLayout {
            Layout.leftMargin: 24
            Layout.rightMargin: 24

            spacing: 24

            Button {
                Layout.fillWidth: true
                Material.roundedScale: Material.SmallScale

                enabled: !bluDiscovery.isActive && centeral
                text: "Scan"

                onClicked: {
                    bluDiscovery.start();
                }
            }

            Button {
                Layout.fillWidth: true
                Material.roundedScale: Material.SmallScale

                enabled: bluDiscovery.isActive && centeral
                text: "Stop"

                onClicked: {
                    bluDiscovery.start();
                }
            }
        }
    }
}
