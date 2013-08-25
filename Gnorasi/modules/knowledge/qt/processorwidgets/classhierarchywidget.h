#ifndef CLASSHIERARCHYWIDGET_H
#define CLASSHIERARCHYWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>

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
    
signals:
    
public slots:

private slots:
    void onModelDataChanged(QModelIndex,QModelIndex);

private:
//    voreen::OntologyClassModel  *m_pOntologyClassModel;
    QStandardItemModel          *m_pOntologyClassModel;

    voreen::OntologyClassView   *m_pOntologyClassView;
    
};

#endif // CLASSHIERARCHYWIDGET_H
