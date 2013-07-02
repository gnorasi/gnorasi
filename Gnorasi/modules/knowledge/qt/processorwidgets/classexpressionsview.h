#ifndef CLASSEXPRESSIONSVIEW_H
#define CLASSEXPRESSIONSVIEW_H

#include <QTreeView>

class ContextMenuEvent;

class ClassExpressionsView : public QTreeView
{
    Q_OBJECT
public:
    explicit ClassExpressionsView(QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void onAddExpressionItem();
    void onDeleteExpressionItem();
    void onEditExpressionItem();

private:


};

#endif // CLASSEXPRESSIONSVIEW_H
