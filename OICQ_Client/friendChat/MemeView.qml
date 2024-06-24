import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components" as MyComponents
import Qt.labs.folderlistmodel

// 表情组件

Popup {
    id: popup
    width: 438
    height: 300
    visible: false
     property string memepath: ""
     property int currentIndex: 1
    background: Rectangle{
        anchors.fill: parent
        radius: 5
        border.color: "#ccc"
        color: "white"
    }

    signal emojiInsert(var type)


    ListModel {
        id:buttonModel1
        ListElement{
            path:"qrc:/icon/biaoqing_black.png"
        }
        ListElement{
            path:"qrc:/icon/shoucang.png"
        }
        ListElement{
            path:"qrc:/icon/tianjia.png"
        }
    }

    Rectangle{
        id: rect1
        x:8
        width: 424
        height: 254
        GridView {
            id: gridview
            anchors.fill: parent

            model: imageModel1
            clip: true
            height: 200
            delegate: imageDelegate1
            cellHeight: 64
            cellWidth: 70
        }

    }
    Rectangle{
        id: line
        height: 1
        width: parent.width
        anchors.top: rect1.bottom
        color: "#eee"
    }
    Rectangle{
        //width: parent.width
        id: rect3
        width: 430
        anchors.top: line.bottom
        height: 36
        Row{
            spacing: 2
            Repeater{
                id:repeater1
                model: buttonModel1
                //这边的currentIndex只是用来对当前选择的项进行ui切换
                MyComponents.IconButton{
                    width: 34
                    height: 34
                    imagePath: path
                    backColor: currentIndex === index ? "#0099ff" : "white"
                    onClicked: {
                        currentIndex = index
                        if(currentIndex === 0){
                            gridview.delegate = imageDelegate2
                            gridview.model= smallmemes1
                            gridview.cellHeight = 42
                            gridview.cellWidth = 42
                        }
                        if(currentIndex === 1){
                            gridview.delegate = imageDelegate1
                            gridview.model= imageModel1
                            gridview.cellHeight = 64
                            gridview.cellWidth = 70
                        }
                        if(currentIndex === 3){

                        }
                    }
                }
            }
        }
    }

    Component {
        id: imageDelegate1  //收藏的表情
        Item {
            width: 56
            height: 56

            Image {
                id: image
                anchors.fill: parent
                source: model.imageUrl // 绑定到模型的 imageUrl 属性
                TapHandler{
                    onTapped: {
                        memepath = parent.source
                        emojiInsert(0)
                    }
                }
            }
        }
    }
    Component {
        id: imageDelegate2  //默认表情
        Item {
            width: 32
            // 或者根据需要进行设置
            height: 32
            Image {
                id: image
                anchors.fill: parent
                source: model.imageUrl // 绑定到模型的 imageUrl 属性
                TapHandler{
                    onTapped: {
                        memepath = parent.source
                        emojiInsert(1)
                    }
                }
            }
        }
    }

    // function setFilesModel(){
    //     list1.clear()
    //     for(let i = 0; i< arguments[0].length; i++){
    //         //数据项的格式
    //         let data = { "imageUrl": arguments[0][i]} ;
    //         list1.append(data)
    //     }
    //     gridview.model = null   // 重置model
    //    // gridview.openType = 0
    //     gridview.model = list1;
    //     gridview.currentIndex = 0;
    // }

    // function setFolderModel(){
    //     folderList1.folder =  arguments[0];
    //     console.log("得到的folder路径:" ,folderList1.folder)
    //     gridview.model = null

    //     //gridview.model = folderList1;

    // }

    // ListModel{
    //     id: list1
    // }
    // FolderListModel{
    //     id: folderList1
    //     nameFilters: ["*.jpg"]

    //     folder: "memeImages"
    //     showFiles: true
    // }


    ListModel {
        id: imageModel1
        // 假设有一个图片列表的 URL 或路径
        ListElement { imageUrl: "qrc:/memeImages/mimage1.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage2.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage3.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage4.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage5.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage6.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage7.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage8.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage9.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage10.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage11.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage12.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage13.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage14.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage15.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage16.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage17.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage18.gif" }
        ListElement { imageUrl: "qrc:/memeImages/mimage19.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage20.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage21.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage22.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage23.jpg" }
        ListElement { imageUrl: "qrc:/memeImages/mimage24.jpg" }
    }

    ListModel{
        id: smallmemes1
        ListElement{ imageUrl: "qrc:/smallMemes/0.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/1.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/2.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/3.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/4.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/5.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/6.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/7.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/8.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/9.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/10.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/11.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/12.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/13.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/14.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/21.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/23.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/25.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/26.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/27.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/29.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/32.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/33.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/34.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/36.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/37.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/38.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/39.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/42.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/45.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/46.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/47.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/50.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/51.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/53.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/54.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/55.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/56.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/57.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/58.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/59.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/62.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/63.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/64.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/71.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/72.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/73.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/74.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/74.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/75.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/76.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/77.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/78.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/79.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/80.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/81.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/82.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/83.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/84.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/85.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/86.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/87.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/88.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/91.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/93.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/95.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/96.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/97.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/98.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/99.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/100.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/101.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/102.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/103.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/104.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/105.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/106.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/107.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/108.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/109.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/110.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/111.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/112.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/113.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/114.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/115.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/116.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/117.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/118.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/119.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/120.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/121.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/122.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/123.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/124.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/125.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/126.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/127.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/128.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/129.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/130.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/131.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/132.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/133.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/134.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/135.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/136.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/137.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/138.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/139.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/140.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/141.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/142.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/143.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/144.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/145.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/146.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/147.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/148.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/149.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/150.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/151.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/152.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/153.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/154.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/155.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/156.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/157.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/158.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/159.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/160.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/161.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/162.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/163.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/164.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/165.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/166.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/167.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/168.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/169.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/170.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/171.png" }
        ListElement{ imageUrl: "qrc:/smallMemes/172.png" }

    }
}
