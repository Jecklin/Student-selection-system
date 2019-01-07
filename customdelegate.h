#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QDate>
#include <QSpinBox>

class CustomDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CustomDelegate(QObject *parent = 0);
    
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void     setEditorData(QWidget *editor, const QModelIndex &index)const override;
    void     setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const override;
    void     updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)const override;
    
};

#endif // CUSTOMDELEGATE_H
