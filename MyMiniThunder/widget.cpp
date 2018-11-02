#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // QMessageBox::warning(this, "testTitle", "TestMsg");
    
    this->setLayout(ui->verticalLayoutMain );
    this->resize(QSize(680, 280));
    
    
    m_pDownEngine = NULL;
    m_bIsEngineInitFinish = FALSE;
    
    m_pDownEngine = new DownEngine::CDownWrapper();
    if(m_pDownEngine == NULL)
    {
        // QMessageBox::warning(this, "初始化", "空间申请失败, 请联系管理员...");
        QMessageBox::warning(this, "Init", "Alloc Mem Error, Please Contact Admin...");
        qApp->quit();
        return;
    }
    try
    {
        BOOL bRet = m_pDownEngine->InitEngine(TEXT("./xldl.dll"));
        if (bRet)
        {
            
        }
    }
    catch(TSTRING str)
    {
        QMessageBox::warning(this, "Init", TCHARToQString((const ushort*)str.c_str()));
        qApp->quit();
        return;
    }
    
    
    connect(ui->pushButton_Start,       &QPushButton::clicked, this, &Widget::onBtnClickedStart);
    connect(ui->pushButton_Pause,       &QPushButton::clicked, this, &Widget::onBtnClickedPause);
    connect(ui->pushButton_End,         &QPushButton::clicked, this, &Widget::onBtnClickedEnd);
    connect(ui->pushButton_SaveFile,    &QPushButton::clicked, this, &Widget::onBtnClickedSaveFile);
}

void Widget::onBtnClickedStart(bool)
{
    qDebug() << "onBtnClickedStart" ;
}

void Widget::onBtnClickedPause(bool)
{
    
    qDebug() << "onBtnClickedPause" ;
}

void Widget::onBtnClickedEnd(bool)
{
    
    qDebug() << "onBtnClickedEnd" ;
}

void Widget::onBtnClickedSaveFile(bool)
{
    
    qDebug() << "onBtnClickedSaveFile" ;
}


void Widget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Resize: " << event->size();
}


Widget::~Widget()
{
    delete ui;
}
