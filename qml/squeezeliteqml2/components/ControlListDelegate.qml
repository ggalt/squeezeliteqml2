import QtQuick 2.0

Item {
    id: listItem

//    property alias mainText: lblImage.mainText
//    property alias subText: lblImage.subText
//    property alias image: lblImage.image
//    property alias drillDown: lblImage.drilldownVisisble
//    property alias iconAnnotated: lblImage.annotated
//    property alias iconVisible: lblImage.iconVisible
//    property variant myModel: model

    signal clicked
    signal pressAndHold

//    clicked.connect(main.controlClicked)

    height: 40
    width: parent.width

    SelectionBackground {
        id: background
        visible: mouseArea.pressed
    }

//    LabelImage {
//        id: lblImage
//        anchors.left: parent.left
//    }

    MouseArea {
        id: mouseArea
        anchors.fill: background
        onClicked: {
            main.controlClicked()
            listItem.clicked()
        }

        onPressAndHold: {
            listItem.pressAndHold()
        }
    }
}
