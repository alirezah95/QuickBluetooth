import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material


Pane {
    id: root

    //* Property Declarations
    //* ************************************/

    property string icon: ""

    property string title: ""

    property string value: "0"

    //* Object Properties
    //* ************************************/

    Material.elevation: 8
    Material.roundedScale: Material.SmallScale
    horizontalPadding: 8
    verticalPadding: 12
    font.pointSize: Application.font.pointSize * 1.15

    //* Children
    //* ************************************/

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        Label {
            Layout.fillWidth: true
            font.bold: true
            horizontalAlignment: "AlignHCenter"
            wrapMode: "Wrap"
            text: title
        }

        Image {
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: root.icon
            fillMode: Image.PreserveAspectFit
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
        }

        Label {
            Layout.fillWidth: true
            text: value
            wrapMode: "Wrap"
            horizontalAlignment: "AlignHCenter"
        }
    }
}
