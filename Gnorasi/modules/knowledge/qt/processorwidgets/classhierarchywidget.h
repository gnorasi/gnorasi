#ifndef CLASSHIERARCHYWIDGET_H
#define CLASSHIERARCHYWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>


class OntologyClass;

namespace voreen {
    class OntologyClassModel;
    class OntologyClassView;
}

/*!
 * \brief The ClassHierarchyWidget class
 */
class ClassHierarchyWidget : public QWidget
{
    Q_OBJECT
public:

    explicit ClassHierarchyWidget(QWidget *parent = 0);


    void initialize();


    QStandardItemModel* ontologyClassModel() { return m_pOntologyClassModel; }


    void setupModel();
    
signals:
    
public slots:

private slots:
    void onModelDataChanged(QModelIndex,QModelIndex);

private:
    void processOntology(OntologyClass*, QStandardItem *pParentItem = 0 );

//    voreen::OntologyClassModel  *m_pOntologyClassModel;
    QStandardItemModel          *m_pOntologyClassModel;

    voreen::OntologyClassView   *m_pOntologyClassView;
    
};

#endif // CLASSHIERARCHYWIDGET_H
