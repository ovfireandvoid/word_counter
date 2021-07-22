import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import WordCounter 1.0

Control {
    id: root

    required property WordCounterModel model

    readonly property var colors: [
        '#F44336',
        '#E91E63',
        '#9C27B0',
        '#673AB7',
        '#3F51B5',
        '#2196F3',
        '#03A9F4',
        '#00BCD4',
        '#009688',
        '#4CAF50',
        '#8BC34A',
        '#CDDC39',
        '#FFEB3B',
        '#FFC107',
        '#FF9800',
        '#FF5722',
        '#607D8B',
    ]

    component LabelDelegate: Label {
        required property int index
        required property string word

        elide: Label.ElideLeft
        font {
            pixelSize: 14
        }
        text: word
        wrapMode: Label.NoWrap
    }

    component BarDelegate: Item {
        id: barDelegate

        required property color color
        required property int index
        required property int wordCount
        required property real wordCountFraction

        implicitWidth: barDelegateCaption.implicitWidth
        implicitHeight: Math.max(32, barDelegateCaption.implicitHeight)

        Rectangle {
            id: barDelegateFill

            width: barDelegate.width * barDelegate.wordCountFraction
            height: barDelegate.height

            color: barDelegate.color
            opacity: 0.3
            radius: 2
        }

        Label {
            id: barDelegateCaption

            x: Math.max(0, barDelegateFill.width - width)
            y: (parent.height - height) / 2

            leftPadding: 12
            rightPadding: 12

            elide: Label.ElideNone
            font {
                pixelSize: 12
                weight: Font.DemiBold
            }
            text: Number(barDelegate.wordCount).toLocaleString()
            wrapMode: Label.NoWrap
        }
    }

    padding: 0
    spacing: 0

    background: Rectangle {
        color: '#FFFFFF'
    }

    contentItem: Flickable {
        id: scroll

        boundsBehavior: Flickable.StopAtBounds
        clip: true

        contentWidth: layout.width
        contentHeight: layout.height

        topMargin: 8
        rightMargin: 12
        bottomMargin: 8
        leftMargin: 12

        ScrollBar.vertical: ScrollBar { }

        GridLayout {
            id: layout

            rowSpacing: 8
            columnSpacing: 12

            width: scroll.width - scroll.leftMargin - scroll.rightMargin

            Repeater {
                model: root.model

                LabelDelegate {
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.row: index
                    Layout.column: 0
                }
            }

            Repeater {
                model: root.model

                BarDelegate {
                    color: index !== -1 ? root.colors[index % root.colors.length] : '#000'

                    Layout.alignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    Layout.row: index
                    Layout.column: 1
                }
            }
        }
    }
}
