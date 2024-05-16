import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

Item {
    width: 1000
    height: 800

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(36.90121, 10.190200)
        zoomLevel: 16

        MapItemView {
            model: ListModel {
                ListElement {
                    latitude: 36.90121
                    longitude: 10.190200
                }
            }
            delegate: MapQuickItem {
                anchorPoint.x: image.width / 2
                anchorPoint.y: image.height
                sourceItem: Item {
                    width: 100
                    height: 80
                    Image {
                        id: image
                        source: "/image/marqueur.png"
                        width: parent.width
                        height: parent.height
                    }
                    Text {
                        text: "Esprit bloc I,J,K Cebalat Ben Ammar"
                        font.bold: true
                        color: "red"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.bottom
                    }
                }
                coordinate: QtPositioning.coordinate(model.latitude, model.longitude)
            }
        }
    }
}
