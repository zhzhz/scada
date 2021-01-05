import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "../.."

Text {
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.family: cusConfig.fontFamily
    font.pixelSize: cusConfig.fontPixel
    elide: Text.ElideRight
    color: cusConfig.textColor
//    renderType: Text.NativeRendering
}
