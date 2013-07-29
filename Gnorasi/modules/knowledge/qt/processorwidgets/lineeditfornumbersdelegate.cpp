#include "lineeditfornumbersdelegate.h"

#include <QLineEdit>
#include <QDoubleValidator>

using namespace voreen;

LineEditForNumbersDelegate::LineEditForNumbersDelegate(int decCount, QObject *parent) :
    m_decimalCount(decCount), QItemDelegate(parent)
{
}

QWidget *LineEditForNumbersDelegate::createEditor(QWidget *parent,
 const QStyleOptionViewItem &/* option */,
 const QModelIndex &/* index */) const
{
   QLineEdit *editor = new QLineEdit(parent);

   QDoubleValidator *val = new QDoubleValidator(editor);
   editor->setValidator(val);

   return editor;
}

void LineEditForNumbersDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
   double value = index.model()->data(index, Qt::EditRole).toDouble();

   QLineEdit *cEdit = static_cast<QLineEdit*>(editor);
   cEdit->setText(QString::number(value,'f',m_decimalCount));
}

void LineEditForNumbersDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
   QLineEdit *cEdit = static_cast<QLineEdit*>(editor);
   double value = cEdit->text().toDouble();

   model->setData(index, QString::number(value,'f',m_decimalCount), Qt::EditRole);
}

void LineEditForNumbersDelegate::updateEditorGeometry(QWidget *editor,
 const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
   editor->setGeometry(option.rect);
}

