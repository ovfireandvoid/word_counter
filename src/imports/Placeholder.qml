import QtQuick 2.15
import QtQuick.Controls 2.15

Control {
    id: root

    property color buttonAccent: '#212121'
    property alias buttonEnabled: button.enabled
    property alias buttonText: button.text
    property alias labelText: label.text

    signal buttonClicked()

    font {
        pixelSize: 18
    }

    implicitWidth: Math.max(label.implicitWidth, button.implicitWidth) + leftPadding + rightPadding
    implicitHeight: label.implicitHeight + button.implicitHeight + spacing + topPadding + bottomPadding

    padding: 24
    spacing: 12

    background: Rectangle {
        color: '#FAFAFA'
    }

    Label {
        id: label

        anchors {
            baseline: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        color: '#9E9E9E'
        elide: Label.ElideNone
        horizontalAlignment: Label.AlignHCenter
        width: Math.min(implicitWidth, parent.width)
        wrapMode: Label.WordWrap
    }

    Button {
        id: button

        anchors {
            horizontalCenter: label.horizontalCenter
            top: label.bottom
            topMargin: root.spacing
        }

        background: Rectangle {
            color: root.buttonAccent
            opacity: button.hovered || button.visualFocus || !button.enabled ? 0.2 : 0
            radius: 4
        }

        contentItem: Text {
            color: root.buttonAccent
            elide: Text.ElideNone
            font: button.font
            opacity: button.enabled ? 1 : 0.5
            text: button.text
            wrapMode: Text.WordWrap
        }

        hoverEnabled: true

        horizontalPadding: 12
        verticalPadding: 8

        transform: Translate {
            y: button.down ? 1 : 0
        }

        transformOrigin: Item.Center

        onClicked: root.buttonClicked()
    }
}
