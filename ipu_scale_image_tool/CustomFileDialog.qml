import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls 1.2
import Qt.labs.folderlistmodel 2.1
Rectangle {
    id: custom_file_dialog
    visible: true
    width: 640
    height: 480

    property bool b_file_custom_dialog_result: true
    signal send_select_file_path( var s_select_file )
    Rectangle {
        id: return_top
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 20
        border.width: 1
        width: 120
        height: 30
        radius: 3
        Text {
            id: return_top_txt
            anchors.centerIn: parent
            text: "返回到上一级"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if( file_custom_dialog.show_upper_level() ) {
                    load_folder_data();
                }
            }
            onPressed: {
                return_top.color = "lightgray"
            }
            onReleased: {
                return_top.color = "white"
            }
        }

    }
    Rectangle {
        id: home_dir
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 60
        width: 80
        height: 80
        border.width: 1
        radius: 3

        Text {
            id: home_dir_name
            text:"主文件夹"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                file_custom_dialog.show_home_dir();
                load_folder_data();
            }
            onPressed: {
                home_dir.color = "lightgray"
            }
            onReleased: {
                home_dir.color = "white"
            }
        }


    }
    Rectangle {
        id: sd_dir
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: home_dir.bottom
        anchors.topMargin: 30
        width: 80
        height: 80
        border.width: 1
        radius: 3

        Text {
            id: sd_dir_name
            text: "SD卡目录"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                file_custom_dialog.show_sd_dir();
                load_folder_data();
            }
            onPressed: {
                sd_dir.color = "lightgray"
            }
            onReleased: {
                sd_dir.color = "white"
            }
        }


    }
    Rectangle {
        id: file_system_dir
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: sd_dir.bottom
        anchors.topMargin: 30
        width: 80
        height: 80
        border.width: 1
        radius: 3

        Text {
            id: file_system_dir_name
            text: "文件系统"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                file_custom_dialog.show_file_system_dir();
                load_folder_data();
            }
            onPressed: {
                file_system_dir.color = "lightgray"
            }
            onReleased: {
                file_system_dir.color = "white"
            }
        }

    }

    Rectangle {
        id: directory_file_list
        anchors.top: parent.top
        anchors.topMargin: 60
        anchors.left: home_dir.right
        anchors.leftMargin: 20
        width: 515
        height: 300
        border.color: "black"
        radius: 3

        TableView {
            id: listView
            width: parent.width
            height: parent.height

            focus: true
            alternatingRowColors: false
            selectionMode: SelectionMode.ExtendedSelection
            model: petsModel
            headerVisible: false
            TableViewColumn {
                id: list_name_col
                role: "name"
            }
            onClicked: {
                file_edit.text = ""
                listView.selection.forEach( function( row_index ) {
                    file_edit.text = file_edit.text + " " + petsModel.get( row_index ).name
                } )
            }
            onDoubleClicked: {
                if( file_custom_dialog.show_to_folder( petsModel.get( row ).name ) ) {
                    load_folder_data();
                }
            }
        }

        ListModel {
            id: petsModel
        }
        Text{
            id: get_text
            visible: false
            text: ( listView.currentRow>=0&&listView.currentRow<petsModel.count)?petsModel.get( listView.currentRow ).name:""
            elide: Text.ElideNone
            clip: false
        }

        Component.onCompleted: {
            console.log( "Importshoot.qml onCompleted" )
            file_custom_dialog.show_default_name( custom_file_dialog.b_file_custom_dialog_result );
            load_folder_data();
        }
    }
    //---------------------------------------------------//文件选择条目显示
    Row {
        anchors.left: home_dir.right
        anchors.leftMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
        spacing: 8
        Text {
            id: file_name
            width: 76
            height: 28
            anchors.top: parent.top
            anchors.topMargin: 4
            text: "文件名:"
        }
        TextField {
            id: file_edit
            width: 350
            height: 28
            readOnly: true
        }
        Rectangle {
            id: ok_btn
            border.width: 1
            radius: 3
            width: 80
            height: 28
            Text {
                id: ok_txt
                anchors.centerIn: parent
                text:  "确定"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var sel_indexs = new Array();
                    listView.selection.forEach( function( row_index ) {
                        sel_indexs.push( row_index );
                    } )
                    var s_file = ""
                    for( var i=0; i<sel_indexs.length; i++ ) {
                        s_file += file_custom_dialog.get_current_path() + "/" + petsModel.get( sel_indexs[i] ).name
                        s_file += ","
                        console.log( s_file )
                        //                excel_enter.append_excel_path_into_list( file_custom_dialog.get_current_path() + "/" + petsModel.get( sel_indexs[i] ).name )
                    }
                    send_select_file_path( s_file.substring(0, s_file.lastIndexOf(",")) )
                    //            file_custom_dialog.set_excel_file_path_into_config( )
                    custom_file_dialog.destroy()
                }
                onPressed: {
                    ok_btn.color = "lightgray"
                }
                onReleased: {
                    ok_btn.color = "white"
                }
            }
        }
    }


    Row {
        anchors.left: home_dir.right
        anchors.leftMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 45
        spacing: 8
        Text {
            id: file_type
            height: 28
            anchors.top: parent.top
            anchors.topMargin: 4
            text: "文件类型:"

        }
        TextField {
            id: file_type_combox
            width: 350
            height: 28
            readOnly: true
            enabled: false
            text: "*.jpg *.bmp *.png"
        }
        Rectangle {
            id: cancel_btn
            width: 80
            height: 28
            border.width: 1
            radius: 3

            Text {
                id: cancle_txt
                anchors.centerIn: parent
                text: "取消"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    custom_file_dialog.destroy()
                }
                onPressed: {
                    cancel_btn.color = "lightgray"
                }
                onReleased: {
                    cancel_btn.color = "white"
                }
            }

    }
}
    function load_folder_data() {
        petsModel.clear();
        listView.selection.clear();
        file_edit.text = "";

        var n_count = file_custom_dialog.get_file_folder_number();
        var n_max_len = 0;
        for( var i=0; i<n_count; ++i ) {
            var s_name = file_custom_dialog.get_file_folder_name( i );
            petsModel.append( { "name": s_name } );

            var n_len = s_name.length;
            if( n_len > n_max_len ) {
                n_max_len = n_len;
            }
        }

        //listView.resizeColumnsToContents();
        n_max_len = n_max_len * 16;
        console.log( "Importshoot.qml n_max_len=", n_max_len, " list_name_col.width=", list_name_col.width )
        if( n_max_len > list_name_col.width ) {
            list_name_col.width = n_max_len;
        }
    }

}


