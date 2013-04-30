import QtQuick 2.0
import "components"

//import QtQuick 1.0

Rectangle {
    id: main
    width: 800
    height: 400
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#00000000"
        }

        GradientStop {
            position: 1
            color: "#000000"
        }
    }
    signal shuffle(int state)
    signal repeat(int state)
    signal nextTrack()
    signal play(int state)
    signal prevTrack()
    signal volUp()
    signal volDown()
    signal controlClicked(string s)

    function setControlViewListIndex(idx) {
        controlClicked("index has been set")
        controlListView.currentIndex=idx
    }

    Rectangle {
        id: playingRect
        color: "#00000000"
        anchors.fill: parent
        z: 2

        Rectangle {
            id: playWindow
            x: 491
            width: 400
            radius: 3
            border.color: "#85c4b7"
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#00c4a3"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 1
            Image {
                id: coverImage
                x: 18
                y: 5
                width: 300
                height: 300
                anchors.horizontalCenterOffset: -32
                anchors.horizontalCenter: parent.horizontalCenter
                source: "icons/noAlbumImage.png"
                anchors.top: parent.top
                anchors.topMargin: 5
            }

            Rectangle {
                id: rewindRect
                x: 5
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                MouseArea {
                    id: rewindMouseArea
                    anchors.fill: parent
                    onClicked: main.prevTrack()
                    onPressed: rewindImage.source="icons/rewind_pressed.png"
                    onReleased: rewindImage.source="icons/rewind.png"
                }

                Image {
                    id: rewindImage
                    source: "icons/rewind.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 345
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: playRect
                x: 65
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"

                MouseArea {
                    signal playClicked
                    signal playPressed
                    property int myState: 1 // start off paused
                    id: playMouseArea
                    anchors.fill: parent
                    onClicked: playClicked()
                    onPressed: playPressed()

                    onPlayPressed: {
                        if( myState==0 ) {  // currently playing, so paused icon displayed
                            playImage.source="icons/pause_pressed.png"
                        }
                        if( myState==1 ) {  // currently paused, so playing icon displayed
                            playImage.source="icons/play_pressed.png"
                        }
                    }

                    onPlayClicked: {
                        myState += 1
                        myState %= 2    // stay within the 2 states of "pause and play"
                        main.play(myState)
                        if( myState==0 ) {  // state 0 == play, so show pause Icon so person knows what to push for pause
                            playImage.source="icons/pause.png"
                        }
                        else if( myState==1 ) { // state 1 == pause, so show pause Icon so person knows what to push for play
                            playImage.source="icons/play.png"
                        }
                    }
                }

                Image {
                    id: playImage
                    source: "icons/play.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 285
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: forwardRect
                x: 125
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                MouseArea {
                    id: forwardMouseArea
                    anchors.fill: parent
                    onClicked: main.nextTrack()
                    onPressed: forwardImage.source="icons/forward_pressed.png"
                    onReleased: forwardImage.source="icons/forward.png"
                }

                Image {
                    id: forwardImage
                    source: "icons/forward.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 225
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
            Rectangle {
                id: shuffleRect
                x: 280
                y: 338
                width: 50
                height: 50
                MouseArea {
                    property int myState: 0
                    signal shuffleClicked
                    id: shuffleMouseArea
                    anchors.fill: parent
                    onClicked: shuffleClicked()
                    onShuffleClicked: {
                        myState += 1
                        myState %= 3    // stay within the 3 states of "no shuffle, shuffle by song, shuffle by album"
                        if( myState==0 ) {
                            shuffleImage.source="icons/no_shuffle.png"
                        }
                        else if( myState==1 ) {
                            shuffleImage.source="icons/shuffle_by_song.png"
                        }
                        else if( myState==2 ) {
                            shuffleImage.source="icons/shuffle_by_album.png"
                        }
                    }
                }

                Image {
                    id: shuffleImage
                    source: "icons/no_shuffle.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 70
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: repeatRect
                x: 220
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                MouseArea {
                    property int myState: 0
                    signal repeatClicked
                    id: repeatMouseArea
                    anchors.fill: parent
                    onClicked: repeatClicked()
                    onRepeatClicked: {
                        myState += 1
                        myState %= 3    // stay within the 3 states of "no repeat, repeat song, repeat album"
                        if( myState==0 ) {
                            repeatImage.source="icons/noRepeat.png"
                        }
                        else if( myState==1 ) {
                            repeatImage.source='icons/repeatSong.png'
                        }
                        else if( myState==2 ) {
                            repeatImage.source='icons/repeatAlbum.png'
                        }
                    }
                }

                Image {
                    id: repeatImage
                    source: "icons/noRepeat.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 130
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: volDownRect
                x: 348
                y: 355
                width: 40
                height: 40
                color: "#645a5a5a"
                gradient: Gradient {
                    GradientStop {
                        position: 0.00;
                        color: "#5a5a5a";
                    }
                    GradientStop {
                        position: 0.52;
                        color: "#ffffff";
                    }
                }
                z: 3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                MouseArea {
                    id: volDownMouseArea
                    anchors.fill: parent
                    onClicked: main.volDown()
                }

                Image {
                    id: volDownImage
                    source: "icons/downvol.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 15
                anchors.right: parent.right
            }

            Rectangle {
                id: volUpRect
                x: 348
                y: 5
                width: 40
                height: 40
                color: "#645a5a5a"
                gradient: Gradient {
                    GradientStop {
                        position: 0.00;
                        color: "#5a5a5a";
                    }
                    GradientStop {
                        position: 0.52;
                        color: "#ffffff";
                    }
                }
                z: 2
                MouseArea {
                    id: volUpMouseArea
                    anchors.fill: parent
                    onClicked: main.volUp()
                }

                Image {
                    id: volUpImage
                    source: "icons/upvol.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 15
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 5
            }

            Rectangle {
                id: volRect
                x: 358
                y: 150
                width: 20
                height: 200
                color: "#00000000"
                anchors.right: parent.right
                anchors.rightMargin: 25
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                z: 1

                Image {
                    id: volImage
                    anchors.fill: parent
                    source: "icons/volRect.png"
                }
            }

            Rectangle {
                id: durationRect
                x: 5
                y: 313
                width: 325
                height: 16
                color: "#00000000"
                border.color: "#7a7a7a"
                anchors.right: parent.right
                anchors.rightMargin: 70
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 71
            }
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 0
        }

        Rectangle {
            id: controlWindow
            width: 400
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#00c4a3"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Rectangle {
                id: controlBar
                height: 50
                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: "#000000"
                    }

                    GradientStop {
                        position: 0.44
                        color: "#00000000"
                    }

                    GradientStop {
                        position: 1
                        color: "#ffffff"
                    }
                }
                border.width: 0
                z: 3
                border.color: "#000000"
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0

                Rectangle {
                    id: homeBtn
                    width: 40
                    color: "#00000000"
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5

                    Image {
                        id: homeImage
                        anchors.fill: parent
                        source: "icons/home.png"
                    }

                    MouseArea {
                        id: homeMouseArea
                        x: 0
                        y: 0
                        anchors.fill: parent
                        onPressed: homeImage.source="icons/home_pressed.png"
                        onReleased: homeImage.source="icons/home.png"
                        onClicked: main.controlClicked("Home")
                    }
                }

                Rectangle {
                    id: returnBtn
                    x: 235
                    width: 40
                    color: "#00000000"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5

                    Image {
                        id: returnImage
                        anchors.fill: parent
                        source: "icons/leftArrow.png"
                    }

                    MouseArea {
                        id: returnMousearea
                        x: 0
                        y: 0
                        anchors.fill: parent
                    }
                }

                Rectangle {
                    id: nowPlayingBtn
                    x: 105
                    width: 200
                    color: "#00000000"
                    border.color: "#000000"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5

                    Image {
                        id: nowPlayingImage
                        z: 1
                        anchors.fill: parent
                        source: "icons/now_playing.png"
                    }

                    MouseArea {
                        id: nowPlayingMouseArea
                        anchors.fill: parent
                        onPressed: nowPlayingImage.source="icons/now_playing_pressed.png"
                        onReleased: nowPlayingImage.source="icons/now_playing.png"
                        onClicked: main.controlClicked("NowPlaying")
                    }
                }
            }

            //                Component {
            //                    id: highlight
            //                    Rectangle {
            //                        width: 180; height: 40
            //                        color: "lightsteelblue"; radius: 5
            //                        y: list_view1.currentItem.y
            //                        Behavior on y {
            //                            SpringAnimation {
            //                                spring: 3
            //                                damping: 0.2
            //                            }
            //                        }
            //                    }
            //                }

            //            }

            ListView {
                id: controlListView
                y: 333
                width: 400
                height: 350
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                highlight: highlight
                highlightFollowsCurrentItem: false
                focus: true

                model: controlListModel
                delegate: listDelegate

                //                delegate: ControlListDelegate {
                //                    Row {
                //                        id: row1
                //                        spacing: 10
                //                        Image {
                //                            width: 40
                //                            height: 40
                //                            source: image
                //                        }

                //                        Text {
                //                            text: name
                //                            anchors.verticalCenter: parent.verticalCenter
                //                            font {
                //                                pointSize: 10
                //                                bold: true
                //                            }
                //                            color: "white"
                //                        }
                //                    }
                //                    Component {
                //                        id: highlight
                //                        BorderImage {
                //                            anchors.fill: parent
                //                            source: "icons/controlHighlight.png"
                //                            Behavior on y {
                //                                SpringAnimation {
                //                                    spring: 3
                //                                    damping: 0.2
                //                                }
                //                            }
                //                        }
                //                    }
                //                }
                Component {
                    id: listDelegate
                    Item {
                        x: 5
                        height: 40
                        width: controlListView.width
                        Row {
                            id: row1
                            spacing: 10
                            Image {
                                width: 40
                                height: 40
                                source: image
                            }

                            Text {
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                                font {
                                    pointSize: 10
                                    bold: true
                                }
                                color: "white"
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                controlListView.currentIndex=index
                                main.controlClicked(index)
                            }

                            onPressAndHold: {
                                controlListView.controlPressAndHold(type)
                            }
                        }
                    }
                }

                Component {
                    id: highlight
                    Image {
                        source: "icons/controlHighlight.png"
                        width: controlListView.width; height: 40
                        y: controlListView.currentItem.y
                        Behavior on y {
                            SpringAnimation {
                                spring: 3
                                damping: 0.2
                            }
                        }
                    }
                }


            }
        }
    }
}
