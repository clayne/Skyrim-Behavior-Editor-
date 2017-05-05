#ifndef HKBEVENTRANGEDATAARRAY_H
#define HKBEVENTRANGEDATAARRAY_H

#include "src/hkxclasses/hkxobject.h"

class hkbEventRangeDataArray: public HkxObject
{
public:
    hkbEventRangeDataArray(HkxFile *parent, long ref = -1);
    virtual ~hkbEventRangeDataArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
    int getLastEventDataIndex() const;
    bool write(HkxXMLWriter *writer);
protected:
private:
    struct hkbEventRangeData
    {
        static const QStringList EventRangeMode;    //(EVENT_MODE_SEND_ON_ENTER_RANGE=0;EVENT_MODE_SEND_WHEN_IN_RANGE=1)
        hkbEventRangeData()
            : upperBound(0),
              eventMode(EventRangeMode.first())
        {
            //
        }
        qreal upperBound;
        hkEventPayload event;
        QString eventMode;
    };
    void addEventData(const hkbEventRangeData & data = hkbEventRangeData());
    void setEventDataId(int index, int id);
    void removeEventData(int index);
private:
    static uint refCount;
    static QString classname;
    QList <hkbEventRangeData> eventData;
};

#endif // HKBEVENTRANGEDATAARRAY_H
