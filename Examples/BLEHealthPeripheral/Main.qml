import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import QuickBluetooth

//!
//! NOTE: Peripheral role is not supported on Windows
//!

ApplicationWindow {
    id: rootWindow

    readonly property bool isMobile: Qt.platform.os.match(/(android|ios)/)

    //* Object Properties
    //* ************************************/

    Material.theme: Material.Light
    Material.accent: Material.Pink

    width: isMobile ? Screen.desktopAvailableWidth : 390
    height: isMobile ? Screen.desktopAvailableHeight : 770
    visible: true
    title: qsTr("Health Device")

    //* Children
    //* ************************************/

    Dashboard {
        anchors.fill: parent
    }
}
