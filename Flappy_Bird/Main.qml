import QtQuick
import Flappy_Bird

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
                if (!Driver.started) {
                    Driver.started = true
                }

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
                    Math.max(300 * ((rootItem.height - bird.height) - bird.y) / 100, 0)
                }

                easing.type: Easing.InCirc
                from: bird.y
                to: rootItem.height - bird.height

                onFinished: {
                    console.log("game over")
                }
            }


            SequentialAnimation {
                id: jump

                NumberAnimation {
                    target: bird
                    property: "y"
                    duration: 500 * Math.min(70, bird.y - 0) / 70
                    easing.type: Easing.OutCirc
                    from: bird.y
                    to: Math.max(bird.y - 70, 0)
                }

                onFinished: {
                    falling.start()
                }
            }

            Component.onCompleted: {
                // falling.running = Driver.started
            }
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                bird.jumpUp()
            }
        }

        Repeater {
            id: obstacles

            model: Driver.model
            delegate: Item {
                Rectangle {
                    width: 20
                    height: model.gapY

                    color: "blue"
                    y: 0
                    x: model.X
                }

                Rectangle {
                    width: 20
                    height: Driver.windowHeight - y

                    color: "blue"
                    y: model.gapY + Driver.gapHeight
                    x: model.X
                }
            }
        }

        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Space) {
                bird.jumpUp()
            }
        }

        onWidthChanged: {
            Driver.windowRightmost = width
        }

        onHeightChanged: {
            Driver.windowHeight = height
        }

        Component.onCompleted: {
            Driver.maxNumberOfObstacles = 5
            Driver.gapHeight = 100
            forceActiveFocus()
        }
    }
}
