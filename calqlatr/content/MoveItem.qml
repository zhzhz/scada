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
import "dragItem"//导入拖拉框目录

Rectangle {
    property alias text: btnText.text
    property var id
    property var deleteRender
    //property alias containsMouse: mouseArea.containsMouse
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


    //在要显示拖拉框的item中引入导入CusTemplateDragBorder控件
    //这个控件要放在MouseArea之后，不然会被MouseArea覆盖
    CusTemplateDragBorder {
        id_num:parent.id
        width: parent.width + borderMargin * 2
        height: parent.height + borderMargin * 2
        anchors.centerIn: parent
        visible: true
    }


    function render()
    {
        var state = mainStateControl.store.getState();
        console.log("render()被调用了1");

        x = state.item1.present[id].x;
        y = state.item1.present[id].y;
        //mouseResized增加宽和高，则创建时的state就要有width和height
        width = state.item1.present[id].width;
        height = state.item1.present[id].height;
        rotation = state.item1.present[id].rotation;


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
