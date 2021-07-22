import QtQuick 2.15
import QtQuick.Controls 2.15

import QtGraphicalEffects 1.0

ToolBar {
    id: root

    property alias progressMin: progress.from
    property alias progressMax: progress.to
    property alias progressVal: progress.value

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

    contentItem: ProgressBar {
        id: progress

        from: 0
        to: 100
        value: 40

        background: null

        contentItem: Item {
            implicitWidth: 200
            implicitHeight: 4

            Rectangle {
                color: '#64B5F6'

                width: parent.width * progress.visualPosition
                height: parent.height
            }
        }

        padding: 8
        spacing: 0

    }
}
