#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "src/filetypes/hkxfile.h"

class ProjectFile: public HkxFile
{
    friend class hkbProjectData;
public:
    ProjectFile(MainWindow *window, const QString & name);
    virtual ~ProjectFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    HkxObjectExpSharedPtr * findProjectData(long ref);
protected:
    bool parse();
    bool link();
private:
    HkxObjectExpSharedPtr stringData;
    HkxObjectExpSharedPtr projectData;
    long largestRef;
};

#endif // PROJECTFILE_H