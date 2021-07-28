import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15

import WordCounter 1.0

ApplicationWindow {
    id: root

    function openFile(fileUrl) {
        counter.process(fileUrl);
    }

    function closeFile() {
        counter.reset();
    }

    font {
        pixelSize: 14
    }

    width: 600
    height: 500

    title: Qt.application.displayName

    visible: true

    background: Rectangle {
        color: '#FAFAFA'
    }

    header: MainWindowHeader {
        id: header

        buttonEnabled: counter.status !== WordCounter.None
        buttonText: qsTr('Close')
        labelText: counter.fileName

        visible: false

        onButtonClicked: root.closeFile()
    }

    footer: MainWindowFooter {
        id: footer

        progressMin: counter.progressMin
        progressMax: counter.progressMax
        progressVal: counter.progressVal

        visible: false
    }

    WordCounterModel {
        id: model

        counter: WordCounter {
            id: counter
        }
    }

    Connections {
        enabled: !timer.running
        target: counter

        function onProgressValChanged() {
            model.update();
            timer.start();
        }
    }

    Timer {
        id: timer

        interval: 16
    }

    FileDialog {
        id: fileDialog

        folder: fileDialog.shortcuts.desktop
        nameFilters: [qsTr('Text Documents (*.txt)'), qsTr('All Files (*)')]
        selectExisting: true
        selectFolder: false
        selectMultiple: false

        onAccepted: root.openFile(fileDialog.fileUrl)
    }

    StackLayout {
        id: layout

        anchors {
            fill: parent
        }

        currentIndex: 0

        Placeholder {
            buttonAccent: '#1E88E5'
            buttonEnabled: counter.status === WordCounter.None
            buttonText: qsTr('Browse Files')
            labelText: qsTr('Drag and Drop text document here or')

            onButtonClicked: fileDialog.open()

            DropArea {
                anchors {
                    fill: parent
                }

                onEntered: (drag) => {
                               drag.accepted = false;

                               const { hasUrls, urls } = drag;
                               if (!hasUrls || urls.length !== 1) {
                                   return;
                               }

                               const urlStr = String(urls[0]);
                               if (!urlStr.startsWith('file://') || !urlStr.endsWith('.txt')) {
                                   return;
                               }

                               drag.acceptProposedAction();
                           }

                onDropped: ({ urls }) => root.openFile(urls[0])
            }
        }

        Placeholder {
            buttonAccent: '#E53935'
            buttonEnabled: counter.status === WordCounter.Error
            buttonText: qsTr('Go Back')
            labelText: qsTr('Something went wrong')

            onButtonClicked: root.closeFile()
        }

        ChartView {
            model: model
        }
    }

    Item {
        states: [
            State {
                name: 'empty'
                when: counter.status === WordCounter.None

                PropertyChanges { target: layout; currentIndex: 0 }
            },

            State {
                name: 'error'
                when: counter.status === WordCounter.Error

                PropertyChanges { target: layout; currentIndex: 1 }
            },

            State {
                name: 'ready'
                when: (counter.status === WordCounter.Canceled) ||
                      (counter.status === WordCounter.Finished)

                PropertyChanges { target: layout; currentIndex: 2 }
                PropertyChanges { target: header; visible: true }
            },

            State {
                extend: 'ready'
                name: 'running'
                when: counter.status === WordCounter.Running

                PropertyChanges { target: footer; visible: true }
            }
        ]

        visible: false
    }
}
