#pragma execution_character_set("utf-8")

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QUrl>
#include <QFileDialog>
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
    
private slots:
    void onBtnClickedStart(bool);
    void onBtnClickedPause(bool);
    void onBtnClickedEnd(bool);
    void onBtnClickedSaveFile(bool);
    
private:
    Ui::Widget *ui;
    
    DownEngine::CDownWrapper*       m_pDownEngine;
    BOOL                            m_bIsEngineInitFinish;
    
};

#endif // WIDGET_H
