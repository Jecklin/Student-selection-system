#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDate>
#include <QModelIndex>

class QStandardItemModel;
class QTableView;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
    void    setModelFromDB();
    void    initModel();
    void    initView();
    
private slots:
    void    onModelChanged(QModelIndex,QModelIndex,QVector<int>);
    
private:
    Ui::Widget *ui;
    QStandardItemModel  *m_model;
    QTableView          *m_view;
};

#endif // WIDGET_H
