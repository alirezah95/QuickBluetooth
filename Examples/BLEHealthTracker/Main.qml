import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material


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
    title: qsTr("Health Tracker")

    //* Children
    //* ************************************/

    Dashboard {
        anchors.fill: parent
    }
}
