#ifndef BSEVENTONDEACTIVATEMODIFIER_H
#define BSEVENTONDEACTIVATEMODIFIER_H

#include "hkbmodifier.h"

class BSEventOnDeactivateModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSEventOnDeactivateModifier(HkxFile *parent, long ref = 0);
    virtual ~BSEventOnDeactivateModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSEventOnDeactivateModifier& operator=(const BSEventOnDeactivateModifier&);
    BSEventOnDeactivateModifier(const BSEventOnDeactivateModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkEventPayload event;
};

#endif // BSEVENTONDEACTIVATEMODIFIER_H
