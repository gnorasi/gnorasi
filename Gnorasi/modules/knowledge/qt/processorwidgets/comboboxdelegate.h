#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QItemDelegate>

namespace voreen {

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboBoxDelegate(const QStringList& ,QObject *parent = 0);

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
    QStringList m_data;
    
};

}

#endif // COMBOBOXDELEGATE_H
