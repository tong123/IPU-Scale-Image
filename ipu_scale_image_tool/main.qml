import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    id: main_page
    width: 800
    height: 480
    visible: true
    color: "blue"
    property string s_img_path: "/media/sd-mmcblk2p1/ipu_main/images/img.irp.jpg"
    Image {
        id: img
//        width: 640
//        height: 480
//        source: "qrc:///img.irp.jpg"
//        source: "file:///home/fish/qtample/ipu_scale_image_tool/img_320_240.irp.jpg"
    }
    Rectangle {
        id: open_btn
        width: 100
        height: 50
        border.width: 1
        color: "transparent"
        radius: 3
        Text {
            id: open_txt
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: "打开"
        }
        MouseArea {
            id: mouse_area
            anchors.fill: parent
            onPressed: {
                open_btn.color = "lightgray"
            }
            onReleased: {
                open_btn.color = "transparent"
            }
            onClicked: {
                create_component( "./CustomFileDialog.qml", true )
            }
        }
    }
    Rectangle {
        id: zoom_in
        anchors.right: parent.right
        anchors.rightMargin: 5
        width: 100
        height: 50
        border.width: 1
//        color: "transparent"
        radius: 3
        Text {
            id: zoom_in_txt
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: "+"
        }
        MouseArea {
            anchors.fill: parent
            onPressed: {
                zoom_in.color = "lightgray"
            }
            onReleased: {
                zoom_in.color = "white"
            }
            onClicked: {
                ipu_scale.zoom_in( s_img_path )

            }
        }
    }
    Rectangle {
        id: zoom_out
        anchors.top: zoom_in.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        width: 100
        height: 50
        border.width: 1
//        color: "transparent"
        radius: 3
        Text {
            id: zoom_out_txt
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: "-"
        }
        MouseArea {
            anchors.fill: parent
            onPressed: {
                zoom_out.color = "lightgray"
            }
            onReleased: {
                zoom_out.color = "white"
            }
            onClicked: {
                ipu_scale.zoom_out( s_img_path )
            }
        }
    }
    Connections {
        target: ipu_scale
        onChanged_image_show: {
            img.source = s_string
        }
    }

    function create_component( fn, status ) {
        var component = Qt.createComponent( fn )
        var obj = null
        if( component.status === Component.Ready ) {
            obj = component.createObject( main_page, { } )
            if( obj == null ) {
                console.log( "fail create", fn, "object" )
            } else {
                console.log( "create success" )
                if( fn === "./CustomFileDialog.qml" ) {
                    obj.send_select_file_path.connect( update_image_file_path )
                }
            }
        }
    }

    function update_image_file_path( s_string ) {
        img.source = "file://"+s_string
        s_img_path = s_string
        ipu_scale.reset_zoom_size()
    }

    Keys.onPressed: {
        if( event.key === Qt.Key_F7 ) {
            console.log("evevt F7")
        }
        console.log("evevt: ")

    }

    Component.onCompleted: {
//        create_component( "./CustomFileDialog.qml", true )
    }
}
