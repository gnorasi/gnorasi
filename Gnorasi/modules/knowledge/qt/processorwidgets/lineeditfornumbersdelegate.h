#ifndef LINEEDITFORNUMBERSDELEGATE_H
#define LINEEDITFORNUMBERSDELEGATE_H

#include <QItemDelegate>

namespace voreen {


class LineEditForNumbersDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LineEditForNumbersDelegate(int decCount, QObject *parent = 0);

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
    double m_decimalCount;
    
};

} // end of namespace voreen

#endif // LINEEDITFORNUMBERSDELEGATE_H
