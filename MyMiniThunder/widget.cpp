#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"

#define TIMER_QUERY_TASK_INFO       (800)  // 0.8秒更新一次信息
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    m_objTaskParam(),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // QMessageBox::warning(this, "testTitle", "TestMsg");
    
    m_hTask = NULL;
    
    m_strBtFilePath = "";
    m_pTimerGetTaskInfo = new QTimer(this);
    m_pTimerGetTaskInfo->setInterval(TIMER_QUERY_TASK_INFO);
    m_pTimerGetTaskInfo->stop();
    connect(m_pTimerGetTaskInfo, &QTimer::timeout, this, &Widget::onTimerQueryTaskInfo);        

    
    m_bIsDownloading = FALSE;
    m_bIsDownCancel = FALSE;
    
    this->setLayout(ui->verticalLayoutMain );
    this->resize(QSize(680, 280));
    
    ui->pushButton_End->setEnabled(false);
    ui->pushButton_Pause->setEnabled(false);
    
    
    m_pDownEngine = NULL;
    m_bIsEngineInitFinish = FALSE;
    
    m_pDownEngine = new DownEngine::CDownWrapper();
    if(m_pDownEngine == NULL)
    {
        QMessageBox::warning(this, QString("初始化"), QString("空间申请失败, 请联系管理员..."));
        // QMessageBox::warning(this, "Init", "Alloc Mem Error, Please Contact Admin...");
        qApp->quit();
        return;
    }
    try
    {
        BOOL bRet = m_pDownEngine->InitEngine(TEXT("./xldl.dll"));
        if (!bRet)
        {
            QMessageBox::warning(this, QString("初始化"), QString("未知错误, 初始化引擎失败, 请联系管理员..."));
            // QMessageBox::warning(this, "Init", "Alloc Mem Error, Please Contact Admin...");
            qApp->quit();
            return;
        }   
    }
    catch(WSTRING str)
    {
        QMessageBox::warning(this, "Init", TCHARToQString((const ushort*)str.c_str()));
        qApp->quit();
        return;
    }
    
    ui->checkBox_bIsBT->setCheckState(Qt::Unchecked);
    connect(ui->pushButton_Start,       &QPushButton::clicked,      this, &Widget::onBtnClickedStart);
    connect(ui->pushButton_Pause,       &QPushButton::clicked,      this, &Widget::onBtnClickedPause);
    connect(ui->pushButton_End,         &QPushButton::clicked,      this, &Widget::onBtnClickedEnd);
    connect(ui->checkBox_bIsBT,         &QCheckBox::stateChanged,   this, &Widget::onCheckBoxState);

}


WSTRING Widget::convertTaskInfo(DownEngine::DOWN_TASK_STATUS Status, DownEngine::DownTaskInfo info)
{   
    WSTRING strResult = L"";
    switch (Status)
    {
        case DownEngine::DOWN_TASK_STATUS::NOITEM:
        {
            strResult = L"未知错误";
            break;
        }
        case DownEngine::DOWN_TASK_STATUS::TSC_ERROR:
        {
            strResult = DownEngine::CDownWrapper::ConvertErrcodeToErrString(info.fail_code);
            //PrintTaskError(info.fail_code, info);
            m_pDownEngine->TaskDelete(m_hTask);
            DownEngine::DownTaskParam deleteParam;
            wcscpy(deleteParam.szFilename, info.szFilename);
            wcscpy(deleteParam.szSavePath, m_objTaskParam.szSavePath);
            m_pDownEngine->DelTempFile(deleteParam);
            break;
        }
        case DownEngine::DOWN_TASK_STATUS::TSC_PAUSE:
        {
            strResult = L"暂停下载";
            break;
        }
        case DownEngine::DOWN_TASK_STATUS::TSC_DOWNLOAD:
        {
            std::wstring szBuffer;
            szBuffer += L"正在下载";
            szBuffer += L":";
            szBuffer += info.szFilename;
            strResult = szBuffer.c_str();
            break;
        }
        case DownEngine::DOWN_TASK_STATUS::TSC_COMPLETE:
        {
            strResult = L"下载完成";
            break;
        }
        case DownEngine::DOWN_TASK_STATUS::TSC_STARTPENDING:
        {
            strResult = L"发送请求";
            break;
        }
        case DownEngine::DOWN_TASK_STATUS::TSC_STOPPENDING:
        {
            strResult = L"结束请求";
            break;
        }
        default:
            break;
    }
    return strResult;
}

/// 用于显示任务信息
/// \brief Widget::onTimerQueryTaskInfo
///
void Widget::onTimerQueryTaskInfo()
{
    DownEngine::DownTaskInfo info;
    if (m_bIsDownloading)
    {
        if (this->m_pDownEngine->TaskQueryInfoEx(m_hTask, info))
        {
            WSTRING wstrErr = convertTaskInfo(info.stat, info);
            ui->label_ErrInfo->setText(QString::fromWCharArray(wstrErr.c_str()));
            
            QString strSet = "";
            
            strSet = QString::asprintf("当前速度：%0.2fMbps", ((info.nSpeed / 1024.0f) / 1024.0f));
            // qDebug() << "strSet: " << strSet;
            ui->label_CurSpeed->setText(strSet);
            
            strSet = QString::asprintf("已经下载：%0.2f%%", info.fPercent * 100.0f);
            ui->label_AlreadyDown->setText(strSet);
            
            strSet = QString::asprintf("文件大小：%0.2fMB", ((info.nTotalSize / 1024.0f) / 1024.0f));
            ui->label_FileSize->setText(strSet);
            
            strSet = QString::asprintf("当前进度：%0.2fMB", ((info.nTotalDownload / 1024.0f) / 1024.0f));
            ui->label_CurPlannedSpeed->setText(strSet);
            
            strSet = QString::asprintf("资源加速：%0.2fMB", (((info.nTotalDownload - info.nDonationOrgin) / 1024.0f) / 1024.0f));
            ui->label_ResAddSpeed->setText(strSet);
            
            strSet = QString::asprintf("资源数量：%d/%d", info.nSrcUsing, info.nSrcTotal);
            ui->label_ResCount->setText(strSet);
            
            ui->label_CurSpeed->repaint();
            ui->label_AlreadyDown->repaint();
            ui->label_FileSize->repaint();
            ui->label_CurPlannedSpeed->repaint();
            ui->label_ResAddSpeed->repaint();
            ui->label_ResCount->repaint();
            
            ui->progressBar->setValue((UINT)(info.fPercent * 100.0f));
            
//            //strSet = (QString("当前速度：%0.2fMbps").arg((info.nSpeed / 1024.0f) / 1024.0f));
//            strSet.asprintf("当前速度：%0.2fMbps", ((info.nSpeed / 1024.0f) / 1024.0f));
//            ui->label_CurSpeed->setText(strSet);
            
//            //strSet = (QString("已经下载：%0.2f%%").arg(info.fPercent * 100.0f);
//            strSet.asprintf("已经下载：%0.2f%%", info.fPercent * 100.0f);
//            ui->label_CurSpeed->setText(strSet);
            
//            //strSet = (QString("文件大小：%0.2fMB").arg((info.nSpeed / 1024.0f) / 1024.0f));
//            strSet.asprintf("文件大小：%0.2fMB", ((info.nTotalSize / 1024.0f) / 1024.0f));
//            ui->label_CurSpeed->setText(strSet);
            
//            //strSet = (QString("当前速度：%0.2fMbps").arg((info.nSpeed / 1024.0f) / 1024.0f));
//            strSet.asprintf("当前进度：%0.2fMB", ((info.nTotalDownload / 1024.0f) / 1024.0f));
//            ui->label_CurSpeed->setText(strSet);
            
//            //strSet = (QString("资源加速：%0.2fMB").arg((info.nSpeed / 1024.0f) / 1024.0f));
//            strSet.asprintf("资源加速：%0.2fMB", (((info.nTotalDownload - info.nDonationOrgin) / 1024.0f) / 1024.0f));
//            ui->label_CurSpeed->setText(strSet);
            
//            //strSet = (QString("资源数量：%d/%d").arg(info.nSrcUsing).arg(info.nSrcTotal);
//            strSet.asprintf("资源数量：%d/%d", info.nSrcUsing, info.nSrcTotal);
//            ui->label_CurSpeed->setText(strSet);
            
            
            //PrintTaskInfo(info.stat, info);
            //_stprintf(szBuffer, "当前速度：%0.2fMbps", ((info.nSpeed / 1024.0f) / 1024.0f));
            //m_Recv_Speed.strResult = szBuffer);
            //_stprintf(szBuffer, "已经下载：%0.2f%%", info.fPercent * 100.0f);
            //m_Recv_Precent.strResult = szBuffer);
            //_stprintf(szBuffer, "文件大小：%0.2fMB", ((info.nTotalSize / 1024.0f) / 1024.0f));
            //m_File_TotalSize.strResult = szBuffer);
            //_stprintf(szBuffer, "当前进度：%0.2fMB", ((info.nTotalDownload / 1024.0f) / 1024.0f));
            //m_Recv_TotalDownload.strResult = szBuffer);
            //_stprintf(szBuffer, "资源加速：%0.2fMB", (((info.nTotalDownload - info.nDonationOrgin) / 1024.0f) / 1024.0f));
            //m_Share_Speed.strResult = szBuffer);
            //_stprintf(szBuffer, "资源数量：%d/%d", info.nSrcUsing, info.nSrcTotal);
            //m_Share_Count.strResult = szBuffer);
            //m_Download_Progress.SetPos((UINT)(info.fPercent * 100.0f));
        }
        if (info.stat == DownEngine::TSC_COMPLETE || info.stat == DownEngine::TSC_ERROR || info.fail_code != DownEngine::TASK_ERROR_UNKNOWN)
        {
            //QThread::sleep(2);
            m_bIsDownloading = FALSE;
            //m_Event.SetEvent();
            //KillTimer(m_uTimerIdForQuery);
            if (info.stat == DownEngine::TSC_COMPLETE)
            {
                ui->label_ErrInfo->setText("下载结束!");
            }
            m_pTimerGetTaskInfo->stop();
            m_pDownEngine->TaskStop(m_hTask);
            m_pDownEngine->TaskDelete(m_hTask);
            m_hTask = NULL;
            
            ui->pushButton_Start->setEnabled(true);
            ui->pushButton_Pause->setEnabled(false);
            ui->pushButton_End->setEnabled(false);
        }
    }
}

/// 当开始按钮按下
/// \brief Widget::onBtnClickedStart
///
void Widget::onBtnClickedStart(bool)
{
    qDebug() << "onBtnClickedStart" ;
    QString strDstURL = ui->lineEdit_URL->text();
    qDebug() << "URL: " << strDstURL;
    if(strDstURL.isEmpty())
    {
        QMessageBox::warning(this, "错误", 
                 QString("请检查URL是否为空\r\n"));
        return;
    }
    
//    if(!strDstURL.isNull())
//    {
//        QMessageBox::warning(this, "错误", 
//                 QString("请检查URL是否输入错误\r\n %1.").arg(strDstURL.left(10)));
//        return;
//    }

    if(this->InitDownLoad(strDstURL))
    {
        ui->pushButton_Start->setEnabled(false);
        ui->pushButton_Pause->setEnabled(true);
        ui->pushButton_End->setEnabled(true);
    }
    
    //m_pDownEngine->CreateDownTaskParam(urlDst, );
    
}

/// 初始化, 创建任务
/// \brief Widget::InitDownLoad
/// \param urlDst
/// \return 
///
BOOL Widget::InitDownLoad(QString& strDstURL)
{
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    
    //QString& strURL = urlDst.toString();
    
    BOOL bResult = FALSE;
    
    if( ui->checkBox_bIsBT->isChecked() || 
        strDstURL.contains("ed2k://", Qt::CaseSensitive)) // 是否是BT
    {
        bResult = CreateBtTask(strDstURL);  
    }
    else  // 非BT任务 
    {
        // 是否为迅雷私有链
        if(strDstURL.contains("thunder://", Qt::CaseSensitive))
        {
            bResult = CreateThunderTask(strDstURL);
        }
        else // 常规任务创建
        {
            bResult = CreateNomalTask(strDstURL);
        }
    }
    
    
    return bResult;
}

/// 创建迅雷私有链接的任务
/// \brief Widget::CreateThunderTask
/// \param urlDst
/// \return 
///
BOOL Widget::CreateThunderTask(QString& strDstURL)
{
    QUrl urlDst = strDstURL;
    qDebug() << "CreateThunderTask";
    QString strFileName = urlDst.fileName();  // 得到文件名
    if (strFileName.isEmpty())
    {
        strFileName = "tmp.out";
    }
    qDebug() << "FileName: " << strFileName;
    
    return TRUE;
}

/// 创建常规链接的任务
/// \brief Widget::CreateNomalTask
/// \param urlDst
/// \return 
///
BOOL Widget::CreateNomalTask(QString& strDstURL)
{
    qDebug() << "CreateNomalTask";
    
    QUrl urlDst = strDstURL;
    
    m_bIsDownloading = FALSE;
    
    QString strFileName = urlDst.fileName();  // 得到文件名
    if (strFileName.isEmpty())
    {
        strFileName = "tmp.out";
    }
    qDebug() << "FileName: " << strFileName;
    
    ui->progressBar->setValue(0);
    
    m_objTaskParam = m_pDownEngine->CreateDownTaskParam(
                QStringToTCHAR(urlDst.toString()), 
                QStringToTCHAR(urlDst.fileName()), 
                QStringToTCHAR(QString("./")));
    m_hTask = m_pDownEngine->TaskCreate(m_objTaskParam);
    if(m_pDownEngine->TaskStart(m_hTask))
    {
        m_pTimerGetTaskInfo->start();
        m_bIsDownloading = TRUE;
        return TRUE;
    }
    
    return FALSE;
}

/// 创建BT种子的任务
/// \brief Widget::CreateBtTask
/// \param urlDst
/// \return 
///
BOOL Widget::CreateBtTask(QString& strDstURL)
{
    
    m_bIsDownloading = FALSE;
    qDebug() << "CreateBtTask";
//    QString strFileName = urlDst.fileName();  // 得到文件名
//    if (strFileName.isEmpty())
//    {
//        strFileName = "tmp.out";
//    }
//    qDebug() << "FileName: " << strFileName;
    
    m_hTask = m_pDownEngine->CreateBTTaskByThunder(QStringToTCHAR(strDstURL));
    if(m_pTimerGetTaskInfo != NULL && m_hTask != NULL)
    {
        qDebug() << "BT创建成功: " << m_hTask;
        ui->label_ErrInfo->setText("BT创建成功!");
        m_pTimerGetTaskInfo->start();
        m_bIsDownloading = TRUE;
    }
    else 
    {
        qDebug() << "BT创建失败: " << m_hTask;
        m_pDownEngine->TaskDelete(m_hTask);
        m_hTask = NULL;
    }
    return TRUE;
}

/// 点击了暂停按钮
/// \brief Widget::onBtnClickedPause
///
void Widget::onBtnClickedPause(bool)
{
    
    qDebug() << "onBtnClickedPause" ;
}

/// 点击了结束按钮
/// \brief Widget::onBtnClickedEnd
///
void Widget::onBtnClickedEnd(bool)
{
    qDebug() << "onBtnClickedEnd" ;
    m_bIsDownloading = FALSE;
    m_bIsDownCancel = FALSE;
    m_pTimerGetTaskInfo->stop();
    m_pDownEngine->TaskStop(this->m_hTask);
    m_pDownEngine->TaskDelete(this->m_hTask);
    ui->label_ErrInfo->setText("已停止下载");
    
    ui->pushButton_Start->setEnabled(true);
    ui->pushButton_Pause->setEnabled(false);
    ui->pushButton_End->setEnabled(false);
}

/// 用于判断是否选中BT下载功能
/// \brief Widget::onCheckBoxState
/// \param nCheck  :  Qt::CheckState
///
void Widget::onCheckBoxState(int nCheck)
{
    //Qt::CheckState& enumCheckState = nCheck;
    qDebug() << "onCheckBoxState" ;
    // QDir objDir("./");
    // qDebug() << objDir.dirName();
    // qDebug() << objDir.path();
    // qDebug() << objDir.absolutePath();
    if(nCheck == Qt::Checked)
    {
        m_strBtFilePath = QFileDialog::getOpenFileName(this, "打开种子文件", "./", "种子(*.torrent)");
        if(m_strBtFilePath.isEmpty())
        {
            ui->checkBox_bIsBT->setCheckState(Qt::Unchecked);
            return;
        }
        
        ui->lineEdit_URL->setText(m_strBtFilePath);
//        else
//        {
//            ui->checkBox_bIsBT->setCheckState(Qt::Checked);
//            //ui->checkBox_bIsBT->setCheckable(TRUE);
//        }
        
        qDebug() << m_strBtFilePath;
    }
}


void Widget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Resize: " << event->size();
}


Widget::~Widget()
{
    delete ui;
}
