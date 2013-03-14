#ifndef DOUBLESPINBOXDELEGATE_H
#define DOUBLESPINBOXDELEGATE_H

#include <QItemDelegate>

namespace voreen {

class DoubleSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DoubleSpinBoxDelegate(double min, double max, int decCount, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:

private:
    double m_min;
    double m_max;
    double m_decimalCount;
    
};

} // end of namespace voreen

#endif // DOUBLESPINBOXDELEGATE_H
