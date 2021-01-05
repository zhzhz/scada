import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "../.."
ToolTip {
    id: basicTooltip
    contentItem: BasicText {
        text: basicTooltip.text
    }
    background: Rectangle {
        color: cusConfig.tipBackgroundColor
        border.color: cusConfig.tipBorderColor
    }
}
