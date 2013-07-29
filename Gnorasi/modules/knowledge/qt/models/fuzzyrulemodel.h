#ifndef FUZZYRULEMODEL_H
#define FUZZYRULEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace voreen{
    class FuzzyRule;
}

class FuzzyRuleModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit FuzzyRuleModel(int ontolgoyClass = 0, QObject *parent = 0, bool editable = true);

    ~FuzzyRuleModel();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;



    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());

    voreen::FuzzyRule* getRootItem() { return rootItem; }


    void setOntologyClassId(int i) { m_ontologyClassId = i; }
    int ontologyClassId() const { return m_ontologyClassId; }


    void setupModel();
    
signals:
    
public slots:

private:
    voreen::FuzzyRule *getItem(const QModelIndex &index) const;

    voreen::FuzzyRule *rootItem;

    bool m_editable;

    int m_ontologyClassId;
    
};

#endif // FUZZYRULEMODEL_H
