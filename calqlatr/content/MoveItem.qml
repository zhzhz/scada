/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

Rectangle {
    property alias text: btnText.text
    property var id
    property var deleteRender
    //注意拖动目标不要使用锚布局或者Layout，而是使用相对坐标
    x: 100
    y: 100
    width: 100
    height: 100
    //id: key
    property var objectName:"key"
    property var nameId


    signal deleteThis(var obj)

    Text {
        id: btnText
        anchors.centerIn: parent
        //text: qsTr("x加")
    }

    color: "lightblue"
    MouseArea {
        anchors.fill: parent
        property real lastX: 0
        property real lastY: 0
        property real rectangleXOld: 0
        property real rectangleYOld: 0

        onPressed: {
            //鼠标按下时，记录鼠标初始位置
            lastX = mouseX
            lastY = mouseY

            //root.activeItemId = id;
            root.activeItem = parent;
            console.log("鼠标按下，记录方块位置");
            var state = mainStateControl.store.getState();
            rectangleXOld = state.item1.present[id].x;
            rectangleYOld = state.item1.present[id].y;
        }

        onReleased:{
            //如果方块位置变化了，则调用这个函数，反之不调用
            var state = mainStateControl.store.getState();
            var x = state.item1.present[id].x;
            var y = state.item1.present[id].y;

            if (rectangleXOld === x && rectangleYOld === y)
            {
                //鼠标位置没变化，不在redux-undo中记录
            }
            else
            {

                mainStateControl.mouseReleased();
            }
        }

        onPositionChanged: {
            if (pressed) {
                //鼠标按住的前提下，坐标改变时，计算偏移量，应用到目标item的坐标上即可
                //moveItem.x += mouseX - lastX
                //moveItem.y += mouseY - lastY
                console.log("鼠标移动---------------" + lastX + ' ' + lastY+ ' ' + mouseX+ ' ' + mouseY);
                mainStateControl.mousePositionChanged(id,mouseX - lastX, mouseY - lastY)
                //mouseX在onReleased中等于lastX，所以在这记录下mouse的x，y
                //mouseXTrue = mouseX;
                //mouseYTrue = mouseY;
            }
        }
    }

    function render()
    {
        var state = mainStateControl.store.getState();
        console.log("render()被调用了1");
        //if (state.item1.present[id] !== undefined)
        //{
            x = state.item1.present[id].x;
            y = state.item1.present[id].y;
        //}
        console.log("render()被调用了2")
    }

    Component.onCompleted:{

        deleteRender = mainStateControl.store.subscribe(render);
        render();
    }

    Component.onDestruction:{
        console.log("删除item，没调用deleteRender（）");
        //deleteRender();
    }


}
