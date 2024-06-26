import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Flappy Bird")

    Item {
        id: rootItem

        anchors.fill: parent

        Rectangle {
            id: bird

            function jumpUp() {
                falling.stop()
                jump.restart()
            }

            width: 20
            height: 20
            color: "red"
            y: rootItem.height / 4
            x: 50


            NumberAnimation {
                id: falling
                target: bird
                property: "y"
                duration: {
                    Math.max(400 * ((rootItem.height - bird.height) - bird.y) / 100, 0)
                }

                easing.type: Easing.InCirc
                from: bird.y
                to: rootItem.height - bird.height
            }


            SequentialAnimation {
                id: jump

                NumberAnimation {
                    target: bird
                    property: "y"
                    duration: 500
                    easing.type: Easing.OutCirc
                    from: bird.y
                    to: bird.y - 70
                }

                onFinished: {
                    falling.start()
                }
            }

            Component.onCompleted: {
                falling.running = true
            }
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                bird.jumpUp()
            }
        }
    }
}
