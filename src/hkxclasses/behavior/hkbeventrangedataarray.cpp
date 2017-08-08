#include "hkbeventrangedataarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/**
 * hkbEventRangeDataArray
 */

uint hkbEventRangeDataArray::refCount = 0;

QString hkbEventRangeDataArray::classname = "hkbEventRangeDataArray";

const QStringList hkbEventRangeDataArray::hkbEventRangeData::EventRangeMode = {"EVENT_MODE_SEND_ON_ENTER_RANGE", "EVENT_MODE_SEND_WHEN_IN_RANGE"};

hkbEventRangeDataArray::hkbEventRangeDataArray(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_EVENT_RANGE_DATA_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbEventRangeDataArray::getClassname(){
    return classname;
}

void hkbEventRangeDataArray::addEventData(const hkbEventRangeData & data){
    eventData.append(data);
}

void hkbEventRangeDataArray::setEventDataId(int index, int id){
    if (eventData.size() > index){
        eventData[index].event.id = id;
    }
}

void hkbEventRangeDataArray::removeEventData(int index){
    if (eventData.size() > index){
        eventData.removeAt(index);
    }
}

int hkbEventRangeDataArray::getLastEventDataIndex() const{
    return eventData.size() - 1;
}

bool hkbEventRangeDataArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "eventData"){
            int numtriggers = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtriggers; j++){
                eventData.append(hkbEventRangeData());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "upperBound"){
                        eventData.last().upperBound = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "id"){
                        eventData.last().event.id = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "payload"){
                        if (!eventData.last().event.payload.readReference(index, reader)){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "eventMode"){
                        eventData.last().eventMode = reader.getElementValueAt(index);
                        if (!eventData.last().EventRangeMode.contains(eventData.last().eventMode)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventMode' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
            return true;
        }
        index++;
    }
    return true;
}

bool hkbEventRangeDataArray::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"eventData", QString::number(eventData.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < eventData.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("upperBound"), QString::number(eventData.at(i).upperBound, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("event"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventData.at(i).event.id));
            if (eventData.at(i).event.payload.data()){
                refString = eventData.at(i).event.payload.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
            writer->writeLine(writer->object, false);
            writer->writeLine(writer->parameter, false);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventMode"), eventData.at(i).eventMode);
            writer->writeLine(writer->object, false);
        }
        if (eventData.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (int i = 0; i < eventData.size(); i++){
            if (eventData.at(i).event.payload.data() && !eventData.at(i).event.payload.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload' at"+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

int hkbEventRangeDataArray::getNumberOfRanges() const{
    return eventData.size();
}

bool hkbEventRangeDataArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < eventData.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventData.at(i).event.payload.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            eventData[i].event.payload = *ptr;
        }
    }
    return true;
}

bool hkbEventRangeDataArray::evaulateDataValidity(){
    for (int i = 0; i < eventData.size(); i++){
        if (eventData.at(i).event.payload.data() && eventData.at(i).event.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkbEventRangeDataArray::~hkbEventRangeDataArray(){
    refCount--;
}
