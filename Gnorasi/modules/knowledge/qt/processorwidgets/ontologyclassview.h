#ifndef VRN_ONTOLOGYCLASSVIEW_H
#define VRN_ONTOLOGYCLASSVIEW_H

#include <QtGui/QTreeView>

class OntologyClassView : public QTreeView{
    Q_OBJECT

public:

    OntologyClassView(QWidget *parent = 0);

public slots:
    void onNameValidationError();

signals:
    void nameValidationError();

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);


public slots:
    void onAddChildClass();
    void onRemoveCurrentClass();
    void onAddSiblingClass();

private:
    QString getUniqueNameFromIndex(const QModelIndex& );

};

#endif // VRN_ONTOLOGYCLASSVIEW_H
