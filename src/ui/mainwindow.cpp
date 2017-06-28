#include "mainwindow.h"
#include "src/filetypes/projectfile.h"
#include "src/filetypes/characterfile.h"
#include "src/filetypes/skeletonfile.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/hkbcharacterdata.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/hkxclassesui/hkdataui.h"
#include "src/ui/hkxclassesui/behaviorui/behaviorvariablesui.h"
#include "src/ui/hkxclassesui/behaviorui/characterpropertiesui.h"
#include "src/ui/hkxclassesui/behaviorui/eventsui.h"
#include "src/ui/hkxclassesui/projectui.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorgraph.h"
#include "src/ui/genericdatawidgets.h"

#include <QtWidgets>

#define MAX_REFERENCED_BEHAVIOR_FILES 30
#define CONFIRM_CLOSE_PROJECT_WITHOUT_SAVING "WARNING: There are unsaved changes to the project, character or behavior files currently open!\nAre you sure you want to close them without saving?"
#define CONFIRM_CLOSE_FILE_WITHOUT_SAVING "WARNING: There are unsaved changes to the behavior file currently open!\nAre you sure you want to close it without saving?"

MainWindow::MainWindow()
    :
      topLyt(new QGridLayout(this)),
      topMB(new QMenuBar(this)),
      debugLog(new PlainTextEdit(this)),
      openProjectA(new QAction("Open Project", this)),
      exportToSkyrimDirA(new QAction("Export Project To Skyrim Directory", this)),
      fileM(new QMenu("File", this)),
      saveA(new QAction("Save Viewed Behavior File", this)),
      saveProjectA(new QAction("Save Project", this)),
      exitA(new QAction("Exit", this)),
      viewM(new QMenu("View", this)),
      expandA(new QAction("Expand All", this)),
      collapseA(new QAction("Collapse All", this)),
      refocusA(new QAction("Refocus On Selected Item", this)),
      tabs(new QTabWidget),
      projectFile(NULL),
      characterFile(NULL),
      skeletonFile(NULL),
      projectUI(new ProjectUI(NULL)),
      iconGBLyt(new QVBoxLayout(this)),
      behaviorGraphViewGB(new QGroupBox("Behavior Graph")),
      objectDataSA(new QScrollArea),
      eventsWid(new EventsUI("Events")),
      variablesWid(new BehaviorVariablesUI("Behavior Variables")),
      objectDataWid(new HkDataUI("Object Data")),
      logGB(new QGroupBox("Debug Log")),
      logGBLyt(new QVBoxLayout(this)),
      lastFileSelected("C:/")
{
    //setStyleSheet("QComboBox {background: yellow};QWidget {background: darkGray}");
    projectUI->setDisabled(true);

    //Change this for release!!!
    hkxcmdPath = "c:/users/wayne/desktop/hkxcmd.exe";
    //hkxcmdPath = QDir::current()+"/hkxcmd.exe";
    //Change this for release!!!

    openProjectA->setStatusTip("Open a hkx project file!");
    openProjectA->setShortcut(QKeySequence::Open);
    exportToSkyrimDirA->setStatusTip("Pack and export the current project to the working Skyrim directory!");
    exportToSkyrimDirA->setShortcut(QKeySequence::Copy);
    saveA->setStatusTip("Save file!");
    saveA->setShortcut(QKeySequence::Save);
    saveProjectA->setStatusTip("Save project!");
    saveProjectA->setShortcut(QKeySequence::SaveAs);
    exitA->setStatusTip("Exit!");
    exitA->setShortcut(QKeySequence::Close);
    fileM->addAction(openProjectA);
    fileM->addAction(exportToSkyrimDirA);
    fileM->addAction(saveA);
    fileM->addAction(saveProjectA);
    fileM->addAction(exitA);
    expandA->setStatusTip("Expand all branches!");
    expandA->setShortcut(QKeySequence::ZoomIn);
    collapseA->setStatusTip("Collapse all branches!");
    collapseA->setShortcut(QKeySequence::ZoomOut);
    refocusA->setStatusTip("Centers the behavior view on the selected item!");
    refocusA->setShortcut(QKeySequence::Back);
    viewM->addAction(expandA);
    viewM->addAction(collapseA);
    viewM->addAction(refocusA);
    topMB->setMaximumHeight(50);
    topMB->addMenu(fileM);
    topMB->addMenu(viewM);
    logGBLyt->addWidget(debugLog);
    logGB->setLayout(logGBLyt);
    eventsWid->setHkDataUI(objectDataWid);
    variablesWid->setHkDataUI(objectDataWid);
    objectDataWid->setEventsVariablesAnimationsUI(eventsWid, variablesWid, projectUI->animations);
    iconGBLyt->addWidget(tabs);
    behaviorGraphViewGB->setLayout(iconGBLyt);
    objectDataSA->setWidgetResizable(true);
    topLyt->addWidget(topMB, 0, 0, 1, 10);
    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 6);
    topLyt->addWidget(objectDataSA, 1, 6, 6, 4);
    topLyt->addWidget(eventsWid, 7, 6, 4, 2);
    topLyt->addWidget(variablesWid, 7, 8, 4, 2);
    topLyt->addWidget(logGB, 10, 0, 1, 6);
    tabs->setTabsClosable(true);
    readSettings();
    setLayout(topLyt);
    objectDataSA->setWidget(objectDataWid);
    //objectDataSA->setStyleSheet("QScrollArea {background-color:cyan}");
    //variablesWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    //eventsWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    logGB->setMaximumSize(size().width()*0.6, size().height()*0.1);
    if (!findSkyrimDirectory()){
        WARNING_MESSAGE(QString("The TESV executable was not found!"));
    }
    /*if (!findHKXCMD()){
        WARNING_MESSAGE(QString("The hkxcmd executable was not found!"));
    }*/
    connect(openProjectA, SIGNAL(triggered(bool)), this, SLOT(openProject()));
    connect(saveA, SIGNAL(triggered(bool)), this, SLOT(save()));
    connect(exportToSkyrimDirA, SIGNAL(triggered(bool)), this, SLOT(packAndExportToSkyrimDirectory()));
    connect(saveProjectA, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
    connect(expandA, SIGNAL(triggered(bool)), this, SLOT(expandBranches()));
    connect(collapseA, SIGNAL(triggered(bool)), this, SLOT(collapseBranches()));
    connect(refocusA, SIGNAL(triggered(bool)), this, SLOT(refocus()));
    connect(exitA, SIGNAL(triggered(bool)), this, SLOT(exit()));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(changedTabs(int)));
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(projectUI, SIGNAL(openFile(QModelIndex)), this, SLOT(openBehaviorFile(QModelIndex)));
}

MainWindow::~MainWindow(){
    //
}

QMessageBox::StandardButton MainWindow::yesNoDialogue(const QString & message){
    return QMessageBox::warning(0, "Skyrim Behavior Tool", message, QMessageBox::Yes | QMessageBox::No);
}

QMessageBox::StandardButton MainWindow::closeAllDialogue(){
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "Skyrim Behavior Tool", CONFIRM_CLOSE_PROJECT_WITHOUT_SAVING, QMessageBox::Yes | QMessageBox::SaveAll | QMessageBox::Cancel);
    if (ret == QMessageBox::SaveAll){
        saveProject();
    }
    return ret;
}

QMessageBox::StandardButton MainWindow::closeFileDialogue(){
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "Skyrim Behavior Tool", CONFIRM_CLOSE_FILE_WITHOUT_SAVING, QMessageBox::Yes | QMessageBox::Save | QMessageBox::Cancel);
    if (ret == QMessageBox::Save){
        save();
    }
    return ret;
}

void MainWindow::changedTabs(int index){
    //ProgressDialog dialog("Changing viewed behavior...", "", 0, 100, this);
    index--;
    if (projectFile){
        if (!behaviorFiles.isEmpty()){
            if (index < behaviorFiles.size() && index < behaviorGraphs.size()){
                objectDataWid->changeCurrentDataWidget(NULL);
                if (index == -1){
                    variablesWid->hide();
                    eventsWid->hide();
                    objectDataSA->hide();
                    topLyt->removeWidget(objectDataSA);
                    topLyt->removeWidget(eventsWid);
                    topLyt->removeWidget(variablesWid);
                    topLyt->removeWidget(behaviorGraphViewGB);
                    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 10);
                }else if (index >= 0){
                    objectDataWid->loadBehaviorView(behaviorGraphs.at(index));
                    variablesWid->clear();
                    variablesWid->loadData(behaviorFiles.at(index)->getBehaviorGraphData());
                    eventsWid->clear();
                    eventsWid->loadData(behaviorFiles.at(index)->getBehaviorGraphData());
                    topLyt->removeWidget(behaviorGraphViewGB);
                    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 6);
                    topLyt->addWidget(objectDataSA, 1, 6, 6, 4);
                    topLyt->addWidget(eventsWid, 7, 6, 4, 2);
                    topLyt->addWidget(variablesWid, 7, 8, 4, 2);
                    variablesWid->show();
                    eventsWid->show();
                    objectDataSA->show();
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("MainWindow::changedTabs(): The tab index is out of sync with the behavior files or behavior graphs!"));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("MainWindow::changedTabs(): No project opened!"));
    }
    //dialog.setValue(//dialog.maximum());
    //dialog.done(0);
}

void MainWindow::expandBranches(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
            behaviorGraphs.at(index)->expandAllBranches();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("MainWindow::expandBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!"));
        }
    }else{
        WARNING_MESSAGE(QString("MainWindow::expandBranches(): No project opened!"));
    }
}

void MainWindow::collapseBranches(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
            behaviorGraphs.at(index)->contractAllBranches();
            behaviorGraphs.at(index)->selectRoot();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("MainWindow: collapseBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!"));
        }
    }else{
        WARNING_MESSAGE(QString("MainWindow::collapseBranches(): No project opened!"));
    }
}

void MainWindow::refocus(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index != -1){
            if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
                if (!behaviorGraphs.at(index)->refocus()){
                    WARNING_MESSAGE(QString("MainWindow::refocus(): No behavior graph item is currently selected!"));
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("MainWindow::refocus() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!"));
            }
        }else{
            WARNING_MESSAGE(QString("MainWindow::refocus(): No behavior graph is currently viewed!"));
        }
    }else{
        WARNING_MESSAGE(QString("MainWindow::refocus(): No project opened!"));
    }
}

void MainWindow::save(){
    saveFile(tabs->currentIndex() - 1);
}

void MainWindow::saveFile(int index){
    if (projectFile){
        QString backupPath;
        if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
            if (behaviorFiles.at(index)->getIsChanged()){
                ProgressDialog dialog("Saving behavior: "+behaviorFiles.at(index)->fileName()+"...", "", 0, 100, this);
                backupPath = QDir::currentPath()+"/Backup";
                QDir dir(backupPath);
                if (!dir.exists()) {
                    dir.mkpath(backupPath);
                }
                dialog.setProgress("Creating backup file...", 10);
                QString filename = backupPath+"\\"+behaviorFiles.at(index)->fileName().section("/",-1,-1);
                QFile backup(filename);
                if (backup.exists()){
                    if (!backup.remove()){
                        CRITICAL_ERROR_MESSAGE(QString("Failed to remove to old backup file "+backupPath.section("/",-1,-1)+"!"));
                    }
                }
                if (!behaviorFiles.at(index)->copy(behaviorFiles.at(index)->fileName(), filename)){
                    CRITICAL_ERROR_MESSAGE(QString("Backup failed!"));
                }else{
                    if (!behaviorFiles.at(index)->remove()){
                        CRITICAL_ERROR_MESSAGE(QString("Failed to remove to old file "+backupPath.section("/",-1,-1)+"!"));
                    }
                    dialog.setProgress("Backup finished...", 20);
                }
                dialog.setProgress("Writing to file...", 30);
                behaviorFiles.at(index)->write();
                behaviorFiles.at(index)->toggleChanged(false);
                dialog.setProgress("Behavior file saved!!!", dialog.maximum());
            }
        }else{
            WARNING_MESSAGE(QString("MainWindow::saveFile(): No behavior file is currently viewed!"));
        }
    }else{
        WARNING_MESSAGE(QString("MainWindow::saveFile(): No project opened!"));
    }
}

void MainWindow::saveProject(){
    if (projectFile){
        ProgressDialog dialog("Saving project "+projectFile->fileName().section("/", -1, -1)+"...", "", 0, 100, this);
        if (projectFile->getIsChanged()){
            projectFile->write();
            projectFile->toggleChanged(false);
        }
        dialog.setProgress(projectFile->fileName()+" saved...", 10);
        if (characterFile){
            if (characterFile->getIsChanged()){
                characterFile->write();
                characterFile->toggleChanged(false);
            }
            dialog.setProgress(characterFile->fileName().section("/", -1, -1)+" saved...", 20);
            if (skeletonFile){
                /*if (skeletonFile->getIsChanged()){
                    skeletonFile->write();
                    skeletonFile->toggleChanged(false);
                }*/
                dialog.setProgress("Saving all unsaved behavior files...", 30);
                for (int i = 0; i < behaviorFiles.size(); i++){
                    if (behaviorFiles.at(i) && i < behaviorGraphs.size() && behaviorGraphs.at(i)){
                        if (behaviorFiles.at(i)->getIsChanged()){
                            saveFile(i);
                        }
                    }else{
                        CRITICAL_ERROR_MESSAGE(QString("MainWindow::saveProject(): The tab index is out of sync with the behavior files or a NULL pointer in either the behavior or behavior graph list was encountered!!"));
                        return;
                    }
                }
                dialog.setProgress("Project saved!!!", dialog.maximum());
            }else{
                CRITICAL_ERROR_MESSAGE(QString("Project is missing a skeleton file!"));
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("Project is missing a character file!"));
        }
    }else{
        WARNING_MESSAGE(QString("No project open!"));
    }
}

void MainWindow::packAndExportToSkyrimDirectory(){
    if (projectFile){
        QTime t;
        t.start();
        writeToLog("\n-------------------------\nExporting the current project to the Skyrim game directory...");
        ProgressDialog dialog("Exporting the current project to the Skyrim game directory...", "", 0, 100, this);
        QDir dir(skyrimDirectory);
        QString path = skyrimDirectory+"/data/meshes/actors";
        QString projectFolder = path+"/"+lastFileSelectedPath.section("/", -1, -1);
        QString projectName = lastFileSelectedPath.section("/", -1, -1);
        QString tempPath;
        if (dir.exists()){
            dir.setPath(path);
            if (!dir.exists()){
                //The current project does not exist in the game directory... Export the whole project as is...
                if (!dir.mkpath(path)){
                    WARNING_MESSAGE(QString("Failed to make the correct folders in the Skyrim directory!"));
                    return;
                }
                dialog.setProgress("Converting entire project...This may take a few seconds...", 20);
                if (packHKX(lastFileSelected, projectFolder+"/"+projectName) != HKXCMD_SUCCESS){
                    WARNING_MESSAGE(QString("Failed to convert and export the files to the correct folders in the Skyrim directory!"));
                    return;
                }
                dialog.setProgress("Project export sucessful!!", dialog.maximum());
            }else{
                dir.setPath(projectFolder);
                if (dir.exists()){
                    //An older version of the project exists, overwrite the relevant files starting with the project file...
                    dialog.setProgress("Replacing existing project file: "+projectFile->fileName().section("/", -1, -1), 5);
                    tempPath = path+"/"+projectName+"/"+lastFileSelected.section("/", -1, -1);
                    dir.setPath(tempPath);
                    if (QFile::exists(tempPath)){
                        if (!dir.remove(tempPath)){//Might not be necessary since hkxcmd.exe seems to overwrite automatically...
                            WARNING_MESSAGE(QString("Failed to overwrite:"+tempPath+"!"));
                        }
                    }
                    dialog.setProgress("Converting project file...", 8);
                    if (packHKX(lastFileSelected, tempPath) != HKXCMD_SUCCESS){
                        WARNING_MESSAGE(QString("Failed to convert and export the project file to the Skyrim directory!"));
                        return;
                    }
                    dialog.setProgress("Project file exported sucessfully!!!", 10);
                    //Overwrite the character file...
                    tempPath = projectFolder+"/"+projectFile->getCharacterFilePathAt(0);
                    dir.setPath(tempPath);
                    dialog.setProgress("Replacing existing character file: "+characterFile->fileName().section("/", -1, -1), 15);
                    if (QFile::exists(tempPath)){
                        if (!dir.remove(tempPath)){
                            WARNING_MESSAGE(QString("Failed to overwrite:"+tempPath+"!"));
                        }
                    }
                    dialog.setProgress("Converting character file...", 20);
                    if (packHKX(lastFileSelectedPath+"/"+projectFile->getCharacterFilePathAt(0), tempPath) != HKXCMD_SUCCESS){
                        WARNING_MESSAGE(QString("Failed to convert and export the character file to the correct folder in the Skyrim directory!"));
                    }
                    dialog.setProgress("Character file exported sucessfully!", 23);
                    tempPath = projectFolder+"/"+characterFile->getRigName();
                    dir.setPath(tempPath);
                    //Ensure a skeleton file exists in the target game directory...
                    dialog.setProgress("Searching for skeleton file...", 25);
                    if (!QFile::exists(tempPath)){
                        dialog.setProgress("Skeleton file not found! Exporting...", 28);
                        if (packHKX(lastFileSelectedPath+"/"+characterFile->getRigName(), tempPath) != HKXCMD_SUCCESS){
                            WARNING_MESSAGE(QString("Failed to convert and export the skeleton file to the correct folder in the Skyrim directory!"));
                            return;
                        }
                        dialog.setProgress("Skeleton file exported sucessfully!", 30);
                    }
                    //Overwrite the behavior files...
                    tempPath = projectFolder+"/"+characterFile->getBehaviorDirectoryName();
                    dir.setPath(tempPath);
                    dialog.setProgress("Searching for behavior directory...", 35);
                    if (!QFile::exists(tempPath)){
                        dialog.setProgress("Behavior directory not found! Creating one...", 40);
                        dialog.setProgress("Converting behavior files...This may take a few seconds...", 50);
                        if (packHKX(lastFileSelectedPath+"/"+characterFile->getBehaviorDirectoryName(), tempPath) != HKXCMD_SUCCESS){
                            WARNING_MESSAGE(QString("Failed to convert and export the behavior directory to the correct folder in the Skyrim directory!"));
                            return;
                        }
                        dialog.setProgress("Behavior files exported sucessfully!", dialog.maximum());
                    }else{
                        //remove directory and export files...
                        tempPath = projectFolder+"/"+characterFile->getBehaviorDirectoryName();
                        dir.setPath(tempPath);
                        dialog.setProgress("Replacing existing behavior directory...", 35);
                        if (QFile::exists(tempPath)){
                            if (!dir.removeRecursively()){
                                WARNING_MESSAGE(QString("Failed to overwrite:"+tempPath+"!"));
                            }
                        }
                        dialog.setProgress("Converting behavior files...This may take a few seconds...", 40);
                        if (packHKX(lastFileSelectedPath+"/"+characterFile->getBehaviorDirectoryName(), tempPath) != HKXCMD_SUCCESS){
                            WARNING_MESSAGE(QString("Failed to convert and export the behavior directory to the correct folder in the Skyrim directory!"));
                            return;
                        }
                        dialog.setProgress("Behavior files exported sucessfully!", dialog.maximum());
                    }
                }else{
                    dialog.setProgress("Converting entire project...This may take a few seconds...", 20);
                    if (packHKX(lastFileSelectedPath, projectFolder) != HKXCMD_SUCCESS){
                        WARNING_MESSAGE(QString("Failed to convert and export the files to the correct folders in the Skyrim directory!"));
                        return;
                    }
                    dialog.setProgress("Behavior files exported sucessfully!", dialog.maximum());
                }
            }
        }else{
            WARNING_MESSAGE(QString("Skyrim Directory not found!"));
        }
        writeToLog("\n-------------------------\nTime taken to export the project is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    }else{
        WARNING_MESSAGE(QString("No project open!"));
    }
}

void MainWindow::exit(){
    if (exitProgram()) {
        writeSettings();
    }
    QApplication::exit(0);
}

void MainWindow::closeTab(int index){
    objectDataWid->unloadDataWidget();
    if (projectFile){
        if (index == 0 && (behaviorFiles.isEmpty() || (!behaviorFiles.at(index)->getIsChanged() || closeAllDialogue() != QMessageBox::Cancel))){
            closeAll();
        }else{
            index--;
            if (index >= 0 && index < behaviorGraphs.size() && (!behaviorFiles.at(index)->getIsChanged() || closeFileDialogue() != QMessageBox::Cancel)){
                if (index < behaviorFiles.size()){
                    if (behaviorGraphs.at(index)){
                        delete behaviorGraphs.at(index);
                    }
                    behaviorGraphs.removeAt(index);
                    if (behaviorFiles.at(index)){
                        delete behaviorFiles.at(index);
                    }
                    behaviorFiles.removeAt(index);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("MainWindow::closeTab(): The tab index is out of sync with the behavior files or behavior graphs!"));
            }
        }
    }else{
        WARNING_MESSAGE(QString("No project open!"));
    }
}

bool MainWindow::closeAll(){
    bool unsavedChanges = false;
    for (int i = 0; i < behaviorGraphs.size(); i++){
        if (behaviorFiles.at(i)->getIsChanged()){
            unsavedChanges = true;
        }
    }
    if (characterFile->getIsChanged()){
        unsavedChanges = true;
    }
    if (!unsavedChanges || closeAllDialogue() != QMessageBox::Cancel){
        projectUI->setDisabled(true);
        objectDataWid->unloadDataWidget();
        tabs->clear();
        for (int j = 0; j < behaviorGraphs.size(); j++){
            if (behaviorGraphs.at(j)){
                delete behaviorGraphs.at(j);
            }
        }
        behaviorGraphs.clear();
        for (int j = 0; j < behaviorFiles.size(); j++){
            if (behaviorFiles.at(j)){
                delete behaviorFiles.at(j);
            }
        }
        behaviorFiles.clear();
        if (projectFile){
            delete projectFile;
            projectFile = NULL;
        }
        if (characterFile){
            delete characterFile;
            characterFile = NULL;
        }
        if (skeletonFile){
            delete skeletonFile;
            skeletonFile = NULL;
        }
        return true;
    }else{
        return false;
    }
}

void MainWindow::writeToLog(const QString &message, bool isError){
    QString str;
    QString temp = "\n#########################################";
    if (!isError){
        //str = "WARNING!!!-->";
    }else{
        str = "ERROR!!!-->";
    }
    debugLog->appendPlainText(temp+"\n"+str+message+temp);
    QTextCursor tempTC(debugLog->document());
    tempTC.movePosition(QTextCursor::End);
    debugLog->setTextCursor(tempTC);
}

void MainWindow::openProject(){
    if (tabs->count() > 0){
        closeAll();
    }
    QString filename = QFileDialog::getOpenFileName(this, tr("Open hkx project file..."), lastFileSelected, tr("hkx Files (*.hkx)"));
    if (filename == ""){
        return;
    }
    lastFileSelected = filename;
    QTime t;
    t.start();
    objectDataWid->changeCurrentDataWidget(NULL);
    int time = t.elapsed();
    projectFile = new ProjectFile(this, filename);
    ProgressDialog dialog("Opening project..."+projectFile->fileName().section("/", -1, -1), "", 0, 100, this);
    if (!projectFile->parse()){
        CRITICAL_ERROR_MESSAGE(QString("MainWindow::openProject(): The project file "+filename+" could not be parsed!!!"));
        delete projectFile;
        projectFile = NULL;
        return;
    }
    writeToLog("\n-------------------------\nTime taken to open file \""+filename+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    dialog.setProgress("Loading character data...", 10);
    int index = filename.lastIndexOf("\\", -1);
    if (index > -1){
        lastFileSelectedPath = filename.remove(index, filename.size() - index);
    }else{
        index = filename.lastIndexOf("/", -1);
        if (index > -1){
            lastFileSelectedPath = filename.remove(index, filename.size() - index);
        }else{
            lastFileSelectedPath = filename;
        }
    }
    projectUI->setFilePath(lastFileSelectedPath);
    time = t.elapsed();
    characterFile = new CharacterFile(this, lastFileSelectedPath+"/"+projectFile->getCharacterFilePathAt(0));
    if (!characterFile->parse()){
        CRITICAL_ERROR_MESSAGE(QString("MainWindow::openProject(): The character file "+projectFile->getCharacterFilePathAt(0)+" could not be parsed!!!"));
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        return;
    }
    dialog.setProgress("Character data loaded sucessfully!!!", 40);
    projectFile->setCharacterFile(characterFile);
    writeToLog("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"/"+projectFile->getCharacterFilePathAt(0)+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    time = t.elapsed();
    dialog.setProgress("Loading skeleton data...", 50);
    skeletonFile = new SkeletonFile(this, lastFileSelectedPath+"/"+characterFile->getRigName());
    if (!skeletonFile->parse()){
        CRITICAL_ERROR_MESSAGE(QString("MainWindow::openProject(): The skeleton file "+characterFile->getRigName()+" could not be parsed!!!"));
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        delete skeletonFile;
        skeletonFile = NULL;
        return;
    }
    dialog.setProgress("Skeleton data loaded sucessfully!!!", 90);
    characterFile->setSkeletonFile(skeletonFile);
    tabs->addTab(projectUI, "Character Data");
    projectUI->setProject(projectFile);
    projectUI->loadData();
    writeToLog("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"/"+characterFile->getRigName()+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    writeToLog("\n-------------------------\nTime taken to open project \""+filename+
                              "\" is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    projectUI->setDisabled(false);
    dialog.setProgress("Project loaded sucessfully!!!", dialog.maximum());
}

bool MainWindow::openBehavior(const QString & filename){
    if (filename != ""){
        if (projectFile){
            ProgressDialog dialog("Opening "+filename, "", 0, 100, this);
            objectDataWid->changeCurrentDataWidget(NULL);
            dialog.setProgress("Beginning XML parse...", 5);
            behaviorFiles.append(new BehaviorFile(this, characterFile, filename));
            if (!behaviorFiles.last()->parse()){
                delete behaviorFiles.last();
                behaviorFiles.removeLast();
                return false;
            }
            dialog.setProgress("Behavior file parsed and objects linked successfully!!!", 50);
            behaviorGraphs.append(new BehaviorGraphView(objectDataWid, behaviorFiles.last()));
            tabs->addTab(behaviorGraphs.last(), filename.section("/", -1, -1));
            dialog.setProgress("Drawing Behavior Graph...", 60);
            if (!behaviorGraphs.last()->drawGraph(static_cast<DataIconManager *>(behaviorFiles.last()->getBehaviorGraph()))){
                CRITICAL_ERROR_MESSAGE(QString("MainWindow::drawBehaviorGraph(): The behavior graph was drawn incorrectly!"));
            }
            tabs->setCurrentIndex(tabs->count() - 1);
            dialog.setProgress("Behavior graph successfully drawn!!!", dialog.maximum());
            return true;
        }else{
            CRITICAL_ERROR_MESSAGE(QString("MainWindow::openBehavior(): No project is opened!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("MainWindow::openBehavior(): Filename is empty!"));
    }
    return false;
}

void MainWindow::openBehaviorFile(const QModelIndex & index){
    QString fileName = index.data().toString();
    QTime t;
    t.start();
    if (openBehavior(lastFileSelectedPath+"/"+projectFile->character->getBehaviorDirectoryName()+"/"+fileName)){
        writeToLog("\n-------------------------\nTime taken to open behavior \""+fileName+
                              "\" is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    }else{
        CRITICAL_ERROR_MESSAGE(QString("MainWindow::openBehaviorFile(): The behavior file "+fileName+" failded to open correctly!"));
    }
}

void MainWindow::readSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    QPoint pos = settings.value("pos", QPoint(500, 500)).toPoint();
    QSize size = settings.value("size", QSize(900, 900)).toSize();
    resize(size);
    move(pos);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

bool MainWindow::findSkyrimDirectory(){
    QFileInfoList drives = QDir::drives();
    QString driveName;
    QString path;
    QDir dir;
    for (int i = 0; i < drives.size(); i++){
        driveName = drives.at(i).absolutePath();
        dir.setPath(driveName);
        path = driveName+"Steam Games/steamapps/common/Skyrim";
        if (dir.exists(path)){
            skyrimDirectory = path;
            return true;
        }
    }
    return false;
}

MainWindow::HKXCMD_RETURN MainWindow::packHKX(const QString &filepath, const QString &outputDirectory){
    QString command;
    command = "\"\""+hkxcmdPath+"\"\" convert \""+filepath+"\" \""+outputDirectory+"\" -f SAVE_CONCISE";
    command.replace("/", "\\");
    command = "cmd /c "+command;
    QProcess processHKXCMD;
    HKXCMD_RETURN value = (HKXCMD_RETURN)processHKXCMD.execute(command);
    if (value != HKXCMD_SUCCESS){
        writeToLog("MainWindow: packHKX() failed!\nThe command \""+command+"\" failed!", true);
    }
    return value;
}

bool MainWindow::exitProgram()
{
    QMessageBox::StandardButton ret;
    if (closeAll()){
        ret = QMessageBox::warning(this, tr("Application"), "Close the application?", QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok){
            return true;
        }else if (ret == QMessageBox::Cancel){
            return false;
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *){
    writeSettings();
    /*if (exitProgram()) {
        writeSettings();
        event->accept();
    }else {
        event->ignore();
    }*/
}

void MainWindow::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
