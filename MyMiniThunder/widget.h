#pragma execution_character_set("utf-8")

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QUrl>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QMessageBox>
#include <QDebug>
#include <QSize>
#include <QResizeEvent>
#include <QDebug>
#include <QApplication>
#include <QCheckBox>
#include <QTimer>
#include <QTime>
#include <QThread>

#include "./DownWrapper.h"


#ifdef UNICODE
    #define QStringToTCHAR(x)     (wchar_t*) x.utf16()
    #define PQStringToTCHAR(x)    (wchar_t*) x->utf16()
    #define TCHARToQString(x)     QString::fromUtf16((x))
    #define TCHARToQStringN(x,y)  QString::fromUtf16((x),(y))
#else
    #define QStringToTCHAR(x)     x.local8Bit().constData()
    #define PQStringToTCHAR(x)    x->local8Bit().constData()
    #define TCHARToQString(x)     QString::fromLocal8Bit((x))
    #define TCHARToQStringN(x,y)  QString::fromLocal8Bit((x),(y))
#endif


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    virtual void resizeEvent(QResizeEvent *event);
    ~Widget();
    
private:
    BOOL InitDownLoad(QString& );
    
    
    BOOL CreateBtTask(QString& );
    BOOL CreateNomalTask(QString& );
    BOOL CreateThunderTask(QString& );
    
    WSTRING convertTaskInfo(DownEngine::DOWN_TASK_STATUS Status, DownEngine::DownTaskInfo info);
    
private slots:
    void onBtnClickedStart(bool);
    void onBtnClickedPause(bool);
    void onBtnClickedEnd(bool);
    void onCheckBoxState(int);
    
    void onTimerQueryTaskInfo();
    
private:
    Ui::Widget *ui;
    
    HANDLE                          m_hTask;
    QTimer *                        m_pTimerGetTaskInfo;
    DownEngine::CDownWrapper*       m_pDownEngine;
    BOOL                            m_bIsEngineInitFinish;
    DownEngine::DownTaskParam       m_objTaskParam;
    BOOL                            m_bIsDownloading;
    BOOL                            m_bIsDownCancel;
    QString                         m_strBtFilePath;
};

#endif // WIDGET_H
