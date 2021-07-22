import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtGraphicalEffects 1.0

ToolBar {
    id: root

    property alias buttonEnabled: button.enabled
    property alias buttonText: button.text
    property alias labelText: label.text

    signal buttonClicked()

    font {
        weight: Font.DemiBold
    }

    padding: 0
    spacing: 8

    background: Rectangle {
        color: '#263238'

        layer {
            effect: DropShadow {
                color: '#212121'
                radius: 4
                samples: 8
            }

            enabled: true
        }
    }

    contentItem: RowLayout {
        spacing: root.spacing

        Label {
            id: label

            color: '#FFFFFF'
            elide: Label.ElideLeft
            padding: 8
            wrapMode: Label.NoWrap

            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: false
        }

        ToolButton {
            id: button

            background: Rectangle {
                color: '#FFFFFF'
                opacity: button.hovered || button.visualFocus ? 0.2 : 0
            }

            contentItem: Text {
                color: '#FFFFFF'
                elide: Text.ElideRight
                font: button.font
                opacity: button.enabled ? 1 : 0.5
                text: button.text
                wrapMode: Text.NoWrap
            }

            hoverEnabled: true

            padding: 8
            spacing: 0

            transform: Translate {
                y: button.down ? 1 : 0
            }

            transformOrigin: Item.Center

            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: false
            Layout.fillHeight: false

            onClicked: root.buttonClicked()
        }
    }
}
