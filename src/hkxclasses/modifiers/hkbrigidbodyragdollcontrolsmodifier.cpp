#include "hkbrigidbodyragdollcontrolsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbRigidBodyRagdollControlsModifier
*/

uint hkbRigidBodyRagdollControlsModifier::refCount = 0;

QString hkbRigidBodyRagdollControlsModifier::classname = "hkbRigidBodyRagdollControlsModifier";

hkbRigidBodyRagdollControlsModifier::hkbRigidBodyRagdollControlsModifier(BehaviorFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      hierarchyGain(0),
      velocityDamping(0),
      accelerationGain(0),
      velocityGain(0),
      positionGain(0),
      positionMaxLinearVelocity(0),
      positionMaxAngularVelocity(0),
      snapGain(0),
      snapMaxLinearVelocity(0),
      snapMaxAngularVelocity(0),
      snapMaxLinearDistance(0),
      snapMaxAngularDistance(0),
      durationToBlend(0)
{
    setType(HKB_RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbRigidBodyRagdollControlsModifier"+QString::number(refCount);
}

QString hkbRigidBodyRagdollControlsModifier::getClassname(){
    return classname;
}

QString hkbRigidBodyRagdollControlsModifier::getName() const{
    return name;
}

bool hkbRigidBodyRagdollControlsModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "hierarchyGain"){
            hierarchyGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'hierarchyGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "velocityDamping"){
            velocityDamping = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'velocityDamping' data field!\nObject Reference: "+ref);
            }
        }else if (text == "accelerationGain"){
            accelerationGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'accelerationGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "velocityGain"){
            velocityGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'velocityGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "positionGain"){
            positionGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'positionGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "positionMaxLinearVelocity"){
            positionMaxLinearVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'positionMaxLinearVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "positionMaxAngularVelocity"){
            positionMaxAngularVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'positionMaxAngularVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapGain"){
            snapGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'snapGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxLinearVelocity"){
            snapMaxLinearVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'snapMaxLinearVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxAngularVelocity"){
            snapMaxAngularVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'snapMaxAngularVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxLinearDistance"){
            snapMaxLinearDistance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'snapMaxLinearDistance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "snapMaxAngularDistance"){
            snapMaxAngularDistance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'snapMaxAngularDistance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "durationToBlend"){
            durationToBlend = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'durationToBlend' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bones"){
            if (!bones.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bones' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbRigidBodyRagdollControlsModifier::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (variableBindingSet.data()){
            refString = variableBindingSet.data()->getReferenceString();
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableBindingSet"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userData"), QString::number(userData));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enable"), getBoolAsString(enable));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("controlData"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keyFrameHierarchyControlData"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("hierarchyGain"), getDoubleAsString(hierarchyGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("velocityDamping"), getDoubleAsString(velocityDamping));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("accelerationGain"), getDoubleAsString(accelerationGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("velocityGain"), getDoubleAsString(velocityGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("positionGain"), getDoubleAsString(positionGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("positionMaxLinearVelocity"), getDoubleAsString(positionMaxLinearVelocity));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("positionMaxAngularVelocity"), getDoubleAsString(positionMaxAngularVelocity));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapGain"), getDoubleAsString(snapGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxLinearVelocity"), getDoubleAsString(snapMaxLinearVelocity));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxAngularVelocity"), getDoubleAsString(snapMaxAngularVelocity));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxLinearDistance"), getDoubleAsString(snapMaxLinearDistance));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("snapMaxAngularDistance"), getDoubleAsString(snapMaxAngularDistance));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("durationToBlend"), getDoubleAsString(durationToBlend));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        if (bones.data()){
            refString = bones.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bones"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (bones.data() && !bones.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'bones'!!!", true);
        }
    }
    return true;
}

bool hkbRigidBodyRagdollControlsModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr = getParentFile()->findHkxObject(bones.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_INDEX_ARRAY){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'bones' is not a HKB_BONE_INDEX_ARRAY!");
            setDataValidity(false);
        }
        bones = *ptr;
    }
    return true;
}

void hkbRigidBodyRagdollControlsModifier::unlink(){
    HkDynamicObject::unlink();
    bones = HkxObjectExpSharedPtr();
}

bool hkbRigidBodyRagdollControlsModifier::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!bones.data()){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbRigidBodyRagdollControlsModifier::~hkbRigidBodyRagdollControlsModifier(){
    refCount--;
}
