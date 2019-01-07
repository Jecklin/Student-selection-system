#include "widget.h"
#include "ui_widget.h"
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTableView>
#include <QSpinBox>
#include <QDateEdit>
#include <QDebug>
#include <QVariantMap>

#include "customdelegate.h"
#include "database.h"

Widget::Widget(QWidget *parent) :QWidget(parent)
        ,ui(nullptr)
        ,m_model(nullptr)
        ,m_view(nullptr)
{
    ui = new Ui::Widget;
    ui->setupUi(this);
    
    this->setMinimumHeight(700);
    this->setMinimumWidth(1300);
    
    this->initModel();
    this->initView();

    //link
    connect(m_model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(onModelChanged(QModelIndex,QModelIndex,QVector<int>)));
   
}

Widget::~Widget()
{
    delete ui;
    delete m_model;
    delete m_view;
}

void Widget::setModelFromDB()
{
    Database db;
    db.connect();
    
    int size = db.size();
    for (int i = 0; i < size; ++i)
    {
        QVariantMap map     = db.record(i);
        int         id      = map.value("id").toInt();
        QString     name    = map.value("name").toString();
        int         age     = map.value("age").toInt();
        QString     course  = map.value("course").toString();
        
        QString     strDate    = map.value("date").toString();
        QStringList dateList   = strDate.split("/");
        QDate       date(0, 1, 1);
        if (dateList.size() == 3)
        {
            date.setDate(dateList[0].toInt(), dateList[1].toInt(), dateList[2].toInt());
        }

        m_model->setData(m_model->index(i, 0), id);
        m_model->setData(m_model->index(i, 1), name);
        m_model->setData(m_model->index(i, 2), age);
        m_model->setData(m_model->index(i, 3), course);
        m_model->setData(m_model->index(i, 4), date);
    }
}

void Widget::initModel()
{
    m_model = new QStandardItemModel(4, 5, this);
    m_model->setHeaderData(0, Qt::Horizontal, tr("ID"), Qt::EditRole);
    m_model->setHeaderData(1, Qt::Horizontal, tr("姓名"), Qt::EditRole);
    m_model->setHeaderData(2, Qt::Horizontal, tr("年龄"), Qt::EditRole);
    m_model->setHeaderData(3, Qt::Horizontal, tr("课程"), Qt::EditRole);
    m_model->setHeaderData(4, Qt::Horizontal, tr("出生日期"), Qt::EditRole);
    this->setModelFromDB();
}

void Widget::initView()
{
    m_view = new QTableView(this);
    m_view->setModel(m_model);
    m_view->setFixedHeight(600);
    m_view->setFixedWidth(1200);
    
    CustomDelegate *customDelegate = new CustomDelegate(this);
    m_view->setItemDelegate(customDelegate);
    m_view->setColumnHidden(0, true);
    m_view->show();
}

void Widget::onModelChanged(QModelIndex current, QModelIndex previous, QVector<int> vec)
{
    Database    db;
    db.connect();
    
    int         row    = current.row();
    int         column = current.column();
    QVariantMap map    = db.record(row);
    
    if (column == 1)
    {
        QString name = current.model()->data(current).toString();
        map.insert("name", name);
        db.setRecord(map);
    }
    else if (column == 2)
    {
        int age = current.model()->data(current).toInt();
        map.insert("age", age);
        db.setRecord(map);
        
        //link column 4
        int spinBoxValue = current.model()->data(current).toInt();

        QDate date(2019 - spinBoxValue,1, 1);
        m_model->setData(m_model->index(current.row(), 4), date);
        m_view->setModel(m_model);
    }
    else if (column == 3)
    {
        QString course = current.model()->data(current).toString();
        map.insert("course", course);
        db.setRecord(map);
    }
    else if (column == 4)
    {
        QDate preDate = current.model()->data(current).toDate();
        QString date = QString::number(preDate.year(), 10) 
                + "/"
                + QString::number(preDate.month(), 10)
                + "/"
                + QString::number(preDate.day(), 10);
        map.insert("date", date);
        db.setRecord(map);
        
        //link column 2
        QDate dateEditValue = current.model()->data(current).toDate();
        int value = 2019 - dateEditValue.year();
        m_model->setData(m_model->index(current.row(), 2), value);
        m_view->setModel(m_model);
    }
}
