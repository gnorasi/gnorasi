#include "doublespinboxdelegate.h"

#include <QDoubleSpinBox>

using namespace voreen;

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(double min, double max, int decCount, QObject *parent) :
    m_min(min), m_max(max), m_decimalCount(decCount), QItemDelegate(parent)
{
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
 const QStyleOptionViewItem &/* option */,
 const QModelIndex &/* index */) const
{
   QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
   editor->setMinimum(m_min);
   editor->setMaximum(m_max);

   return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
   double value = index.model()->data(index, Qt::EditRole).toDouble();

   QDoubleSpinBox *cBox = static_cast<QDoubleSpinBox*>(editor);
   cBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
   QDoubleSpinBox *cBox = static_cast<QDoubleSpinBox*>(editor);
   double value = cBox->value();

   model->setData(index, QString::number(value,'f',m_decimalCount), Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
 const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
   editor->setGeometry(option.rect);
}
