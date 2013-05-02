import QtQuick 2.0

Rectangle {
    width: 40
    height: 40
    color: "#00000000"

    Image {
        id: buttonImage
        anchors.fill: parent
        source: "qrc:/qtquickplugin/images/template_image.png"
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
    }
    states: [
        State {
            name: "pauseState"
        }
    ]
}
