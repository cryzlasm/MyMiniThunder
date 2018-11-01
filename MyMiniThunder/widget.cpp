#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QDebug>
#include <QSize>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QMessageBox::warning(this, "testTitle", "TestMsg");
    
    this->setLayout(ui->verticalLayoutMain );
    this->resize(QSize(680, 280));
    
}

#include <QResizeEvent>
void  Widget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Resize: " << event->size();
}


Widget::~Widget()
{
    delete ui;
}
