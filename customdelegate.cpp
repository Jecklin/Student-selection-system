#include "customdelegate.h"
#include <QSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>

CustomDelegate::CustomDelegate(QObject *parent)
{
    
}

QWidget *CustomDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int column = index.column();
    if (column == 1)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
    else if (column == 2)
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setFrame(false);
        editor->setMinimum(0);
        editor->setMaximum(100);
        return editor;
    }
    else if (column == 3)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem(tr("高数"));
        editor->addItem(tr("C++"));
        editor->addItem(tr("java"));
        editor->addItem(tr("英语"));
        return editor;
    }
    else if (column == 4)
    {
        QDateEdit *editor = new QDateEdit(parent);
        editor->setDate(QDate(2019, 1, 1));
        return editor;
    }

}

void CustomDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int column = index.column();
    if (column == 1)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString data = index.model()->data(index, Qt::EditRole).toString();
        lineEdit->setText(data);
    }
    else if (column == 2)
    {
        QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
        int value = index.model()->data(index, Qt::EditRole).toInt();
        spinbox->setValue(value);
    }
    else if (column == 3)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString data = index.model()->data(index, Qt::EditRole).toString();
        comboBox->setItemData(comboBox->currentIndex(), data, Qt::EditRole);
    }
    else if (column == 4)
    {
        QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
        QDate date = index.model()->data(index, Qt::EditRole).toDate();
        dateEdit->setDate(date);
    }
}

void CustomDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    int column = index.column();
    if (column == 1)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString data = lineEdit->text();
        model->setData(index, data, Qt::EditRole);
    }
    else if (column == 2)
    {
        QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
        spinbox->interpretText();
        int value = spinbox->value();
        model->setData(index, value, Qt::EditRole);
    }
    else if (column == 3)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString data = comboBox->itemText(comboBox->currentIndex());
        model->setData(index, data, Qt::EditRole);
    }
    else if (column == 4)
    {
        QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
        QDate date = dateEdit->date();
        model->setData(index, date, Qt::EditRole);
    }
}

void CustomDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

