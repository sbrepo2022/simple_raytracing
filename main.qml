import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("SB ray tracer")

    Image {
        anchors.fill: parent
        source: "file:///geometry_output.png"
        smooth: true
        fillMode: Image.PreserveAspectFit
    }
}
