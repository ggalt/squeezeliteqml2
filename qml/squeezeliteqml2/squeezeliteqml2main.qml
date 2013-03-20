import QtQuick 2.0


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

    Rectangle {
        id: playingRect
        color: "#00000000"
        anchors.fill: parent
        z: 2

        Rectangle {
            id: playWindow
            x: 491
            width: 400
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#000000"
                }

                GradientStop {
                    position: 1
                    color: "#ffffff"
                }
            }
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            z: 1
            Image {
                id: coverImage
                x: 63
                y: 5
                width: 300
                height: 300
                anchors.horizontalCenterOffset: 13
                anchors.horizontalCenter: parent.horizontalCenter
                source: "icons/noAlbumImage.png"
                anchors.top: parent.top
                anchors.topMargin: 5
            }

            Rectangle {
                id: rewindRect
                x: 50
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                MouseArea {
                    id: rewindMouseArea
                    anchors.fill: parent
                    onClicked: main.prevTrack()
                }

                Image {
                    id: rewindImage
                    source: "icons/rewind.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 300
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: playRect
                x: 110
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                states: [
                    State {
                        name:"pause"
                        PropertyChanges {
                            target: playImage;source:"icons/pause.png"
                        }
                    },
                    State {
                        name:"play"
                        PropertyChanges {
                            target: playImage;source:"icons/play.png"
                        }
                    },
                    State {
                        name:"stop"
                        PropertyChanges {
                            target: playImage;source:"icons/play.png"
                        }
                    }
                ]

                MouseArea {
                    signal playClicked
                    property int myState: 0
                    id: playMouseArea
                    anchors.fill: parent
                    onClicked: playClicked()

                    onPlayClicked: {
                        myState += 1
                        myState %= 2    // stay within the 2 states of "pause and play"
                        main.play(myState)
                        if( myState==0 ) {
                            playRect.state="pause"
                        }
                        else if( myState==1 ) {
                            playRect.state="play"
                        }
                    }
                }

                Image {
                    id: playImage
                    source: "icons/pause.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 240
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
//                signal forwardClicked
                id: forwardRect
                x: 170
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                MouseArea {
                    id: forwardMouseArea
                    anchors.fill: parent
                    onClicked: main.nextTrack()
                }

                Image {
                    id: forwardImage
                    source: "icons/forward.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 180
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
            Rectangle {
                id: shuffleRect
                x: 330
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                states: [
                    State {
                        name:"noshuffle"
                        PropertyChanges {
                            target: shuffleImage;source:"icons/no_shuffle.png"
                        }
                    },
                    State {
                        name:"shuffle_by_song"
                        PropertyChanges {
                            target: shuffleImage;source:"icons/shuffle_by_song.png"
                        }
                    },
                    State {
                        name:"shuffle_by_album"
                        PropertyChanges {
                            target: shuffleImage;source:"icons/shuffle_by_album.png"
                        }
                    }
                ]
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
                            shuffleRect.state="noshuffle"
                        }
                        else if( myState==1 ) {
                            shuffleRect.state="shuffle_by_song"
                        }
                        else if( myState==2 ) {
                            shuffleRect.state="shuffle_by_album"
                        }
                    }
                }

                Image {
                    id: shuffleImage
                    source: "icons/no_shuffle.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 25
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: repeatRect
                x: 270
                y: 338
                width: 50
                height: 50
                color: "#645a5a5a"
                states: [
                    State {
                        name:"noRepeat"
                        PropertyChanges {
                            target: repeatImage; source: "icons/noRepeat.png"
                        }
                    },
                    State {
                        name:"repeatSong"
                        PropertyChanges {
                            target: repeatImage; source: "icons/repeatSong.png"
                        }
                    },
                    State {
                        name:"repeatAlbum"
                        PropertyChanges {
                            target: repeatImage; source: "icons/repeatAlbum.png"
                        }
                    }

                ]
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
                            repeatRect.state="noRepeat"
                        }
                        else if( myState==1 ) {
                            repeatRect.state="repeatSong"
                        }
                        else if( myState==2 ) {
                            repeatRect.state="repeatAlbum"
                        }
                    }
                }

                Image {
                    id: repeatImage
                    source: "icons/noRepeat.png"
                    anchors.fill: parent
                }
                anchors.rightMargin: 85
                anchors.bottomMargin: 12
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: volDownRect
                x: 0
                y: 355
                width: 40
                height: 40
                color: "#645a5a5a"
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
                anchors.rightMargin: 360
                anchors.right: parent.right
            }

            Rectangle {
                id: volUpRect
                x: 7
                y: 261
                width: 40
                height: 40
                color: "#645a5a5a"
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
                anchors.rightMargin: 360
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 5
            }

            Rectangle {
                id: volRect
                x: 0
                width: 40
                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: "#f95909"
                    }

                    GradientStop {
                        position: 1
                        color: "#02d9f9"
                    }
                }
                anchors.right: parent.right
                anchors.rightMargin: 360
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                z: 1
            }

            Rectangle {
                id: durationRect
                x: 50
                y: 313
                width: 325
                height: 16
                color: "#00000000"
                border.color: "#7a7a7a"
                anchors.right: parent.right
                anchors.rightMargin: 25
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 71
            }
            anchors.rightMargin: 0
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 0
        }

        Rectangle {
            id: controlRect
            x: 0
            width: 2000
            color: "#ffffff"
            clip: false
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#51cfa7"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            z: -1
        }
    }
}
