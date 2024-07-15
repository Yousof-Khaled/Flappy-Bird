import QtQuick
import QtQuick.Layouts
import Flappy_Bird

Window {
    width: 320
    height: 480
    visible: true
    title: qsTr("Flappy Bird")

    Item {
        id: rootItem

        Image {
            source: "assets/sprites/background-day.png"
            anchors.fill: parent
        }

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

        property int obstacleWidth: 40
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
            z: 2
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
                    font.pixelSize: 40
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
                wingsAnimation.stop()
                rotateDownwards.stop()
            }

            function reset() {
                y = rootItem.height / 4
                birdImage.state = "will_start"
            }

            width: 40
            height: 40
            color: "transparent"
            x: 50
            // y: rootItem.height / 4
            z: 1

            NumberAnimation {
                id: falling

                property alias birdY: bird.y
                property int start_y: bird.y

                target: bird
                property: "y"
                duration: {
                    Math.max(300 * ((rootItem.height - bird.height) - bird.y) / 100, 0)
                }

                easing.type: Easing.InCirc
                from: bird.y
                to: rootItem.height - bird.height - base.height

                onStarted: {
                    birdImage.state = "falling"
                }

                onBirdYChanged: {
                    if (!falling.running)
                        return

                    if (birdY >= start_y + 70 && !rotateDownwards.running && rotateDownwards.shouldRun) {
                        rotateDownwards.start()
                    }
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
                    falling.start_y = bird.y
                    falling.start()
                }
            }

            onXChanged: {
                Driver.birdX = x;
            }

            onYChanged: {
                Driver.birdY = y;
                if (y + height === rootItem.height - base.height) {
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
                    switch (wing) {
                    case "up":
                        ret += "yellowbird-upflap.png"
                        break
                    case "mid":
                        ret += "yellowbird-midflap.png"
                        break
                    case "down":
                        ret += "yellowbird-downflap.png"
                        break
                    default:
                        break
                    }

                    return ret
                }

                anchors.fill: parent
                source: prefix + sources[1]
                fillMode: Image.PreserveAspectFit
                rotation: -45

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
                    if (state === "will_start") {
                        rotation = -45
                    }

                    if (state !== "falling") {
                        rotateDownwards.stop()
                        if (state === "jumping")
                            rotation = -45
                    }
                }

                RotationAnimation{
                    id: rotateDownwards

                    property bool shouldRun: birdImage.rotation < to

                    target: birdImage
                    duration: 200
                    from: birdImage.rotation
                    to: 90
                }

                property int pausingBetweenFrames: 100

                SequentialAnimation { // todo use SpriteSequence?
                    id: wingsAnimation

                    property int selectedFrame: 0
                    property var animationWingPlacements: ["mid", "down", "mid", "up"]

                    running: birdImage.rotation < 70
                    loops: Animation.Infinite

                    ScriptAction {
                        script: {
                            wingsAnimation.selectedFrame = (wingsAnimation.selectedFrame + 1) % 4
                            birdImage.source = birdImage.getFrame(wingsAnimation.animationWingPlacements[wingsAnimation.selectedFrame])
                        }
                    }

                    PauseAnimation { duration: birdImage.pausingBetweenFrames }

                    onStopped: {
                        birdImage.source = birdImage.getFrame("mid")
                    }
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

                    color: "transparent"
                    y: 0
                    x: model.X

                    Image {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        height: implicitHeight
                        rotation: 180
                        source: "assets/sprites/pipe-green.png"
                    }
                }

                Rectangle {
                    width: rootItem.obstacleWidth
                    height: Driver.windowHeight - y

                    color: "transparent"
                    y: model.gapY + Driver.gapHeight
                    x: model.X

                    Image {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        source: "assets/sprites/pipe-green.png"
                    }
                }
            }
        }

        Item {
            id: base

            ListModel {
                id: x_positions
            }

            function moveLeft() {
                for (var i = 0 ; i < x_positions.count ; ++i) {
                    x_positions.setProperty(i, "X", x_positions.get(i).X - 1)
                }

                if (x_positions.get(0).X + width == 0) {
                    x_positions.remove(0, 1)
                    x_positions.append({"X": base.width})
                }
            }

            width: parent.width
            height: 70
            anchors.bottom: parent.bottom

            Component.onCompleted: {
                x_positions.append({"X": 0})
                x_positions.append({"X": base.width})
            }

            Repeater {
                id: movingFloor

                model: x_positions
                delegate: Image {
                    id: baseImage

                    x: model.X
                    anchors.bottom: base.bottom
                    height: base.height
                    width: base.width
                    source: "assets/sprites/base.png"
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
            Driver.windowHeight = height - base.height
        }

        Component.onCompleted: {
            Driver.maxNumberOfObstacles = 10
            Driver.gapHeight = 130
            Driver.obstableWidth = rootItem.obstacleWidth
            forceActiveFocus()
        }

        Connections {
            target: Driver

            function onGameOver() {
                rootItem.gameOver();
            }

            function onBaseMoveLeft() {
                base.moveLeft()
            }
        }
    }
}
