import QtQuick
import QtQuick.Layouts
import Flappy_Bird

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Flappy Bird")

    Item {
        id: rootItem

        function gameOver() {
            console.log("game over")

            bird.stop()
            needsReset = true

            Driver.started = false
        }

        function resetGame() {
            Driver.resetGame()
            bird.reset()
        }

        property int obstacleWidth: 20
        property bool needsReset: false

        anchors.fill: parent

        Text {
            id: startGameText

            anchors.centerIn: parent
            visible: !Driver.started && !rootItem.needsReset
            text: qsTr("Press space or click the mouse to start")
            font.pixelSize: 15
            font.bold: true
        }

        ColumnLayout {
            id: gameOverBanner

            anchors.centerIn: parent
            z: 1
            spacing: 10
            visible: rootItem.needsReset

            Item {
                Layout.preferredWidth: background.width
                Layout.preferredHeight: background.height

                Rectangle {
                    id: background

                    color: "lightblue"
                    width: gameOverText.implicitWidth + 30
                    height: gameOverText.implicitHeight + 30
                    border.width: 10
                    border.color: "black"
                }

                Text {
                    id: gameOverText

                    anchors.centerIn: parent
                    text: qsTr("GAME OVER")
                    font.pixelSize: 60
                    font.bold: true
                }
            }
            Text {
                id: restartGameText

                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Press space or click the mouse to continue")
            }
        }

        Rectangle {
            id: bird

            function jumpUp() {
                falling.stop()
                jump.restart()
            }

            function stop() {
                jump.stop()
                falling.stop()
            }

            function reset() {
                y = rootItem.height / 4
                birdImage.state = "will_start"
            }

            width: 20
            height: 20
            color: "red"
            x: 50
            // y: rootItem.height / 4
            z: 1

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

                onStarted: {
                    birdImage.state = "falling"
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

                onStarted: {
                    birdImage.state = "jumping"
                }

                onFinished: {
                    falling.start()
                }
            }

            onXChanged: {
                Driver.birdX = x;
            }

            onYChanged: {
                Driver.birdY = y;
                if (y + height === rootItem.height) {
                    rootItem.gameOver()
                }
            }

            Component.onCompleted: {            
                reset()

                Driver.birdWidth = width
                Driver.birdHeight = height
                Driver.birdX = x;
                Driver.birdY = y;
            }

            Image {
                id: birdImage

                property string prefix: "assets/sprites/"
                property var sources: ["yellowbird-downflap.png", "yellowbird-midflap.png", "yellowbird-upflap.png"]

                function getFrame(wing) {
                    var ret = prefix
                    if (wing === "up")
                        ret += "yellowbird-upflap.png"
                    else if (wing === "mid") {
                        ret += "yellowbird-midflap.png"
                    }
                    else {
                        ret += "yellowbird-downflap.png"
                    }

                    return ret
                }

                anchors.fill: parent
                source: prefix + sources[1]
                fillMode: Image.PreserveAspectFit

                state: "will_start"
                states: [
                    State {
                        name: "will_start"
                        PropertyChanges {
                            target: birdImage
                            rotation: 0
                        }
                    },
                    State {
                        name: "jumping"
                        PropertyChanges {
                            target: birdImage
                            rotation: -45
                        }
                    },
                    State {
                        name: "falling"
                    }
                ]

                onStateChanged: {
                    if (state === "falling") {
                        rotateDownwards.start()
                    }
                    else {
                        rotateDownwards.stop()
                    }
                }

                RotationAnimation{
                    id: rotateDownwards

                    target: birdImage
                    duration: 500
                    from: birdImage.rotation
                    to: 90
                }

                property int pausingBetweenFrames: 100

                SequentialAnimation { // todo use SpriteSequence?
                    running: birdImage.rotation < 70
                    loops: Animation.Infinite

                    ScriptAction {
                        script: {
                            birdImage.source = birdImage.getFrame("mid")
                        }
                    }

                    PauseAnimation { duration: birdImage.pausingBetweenFrames }

                    ScriptAction {
                        script: {
                            birdImage.source = birdImage.getFrame("down")
                        }
                    }

                    PauseAnimation { duration: birdImage.pausingBetweenFrames }

                    ScriptAction {
                        script: {
                            birdImage.source = birdImage.getFrame("mid")
                        }
                    }

                    PauseAnimation { duration: birdImage.pausingBetweenFrames }

                    ScriptAction {
                        script: {
                            birdImage.source = birdImage.getFrame("up")
                        }
                    }

                    PauseAnimation { duration: birdImage.pausingBetweenFrames }
                }
            }
        }

        function clickAction() {
            if (needsReset) {
                needsReset = false
                resetGame()
                return
            }

            if (!Driver.started) {
                Driver.started = true
            }
            bird.jumpUp()
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                rootItem.clickAction()
            }
        }

        Repeater {
            id: obstacles

            model: Driver.model
            delegate: Item {
                Rectangle {
                    width: rootItem.obstacleWidth
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
                clickAction()
            }
        }

        onWidthChanged: {
            Driver.windowRightmost = width
        }

        onHeightChanged: {
            Driver.windowHeight = height
        }

        Component.onCompleted: {
            Driver.maxNumberOfObstacles = 10
            Driver.gapHeight = 100
            Driver.obstableWidth = rootItem.obstacleWidth
            forceActiveFocus()
        }

        Connections {
            target: Driver

            function onGameOver() {
                rootItem.gameOver();
            }
        }
    }
}
