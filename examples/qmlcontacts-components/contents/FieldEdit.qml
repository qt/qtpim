import QtQuick 2.0
import com.nokia.components 1.0
import QtAddOn.contacts 2.0

Rectangle {
    id: fieldEdit
    property string field
    property ContactDetail detail
    property string detailDefinitionName
    property string text
    signal valueChanged(string newValue)
    height: childrenRect.height
    color: "black"
    Column {
        height: childrenRect.height
        width: parent.width
        spacing: 3
        Text {
            id: labelText
            text: detailDefinitionName + ": " + field
            height: 30
            color: "white"
            font.pointSize: 16
            width: parent.width

        }
        TextField {
            id: fieldValue
            height: 30
            text:  fieldEdit.text//detail.value(field)
            font.pointSize: 16
            width: parent.width
            onTextChanged: {
                console.log ("Sending valueChange with arg " + text)
                fieldEdit.valueChanged(text)
                }
        }
    }
}
