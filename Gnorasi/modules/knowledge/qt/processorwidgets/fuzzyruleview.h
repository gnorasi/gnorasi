#ifndef FUZZYRULEVIEW_H
#define FUZZYRULEVIEW_H

#include <QTreeView>
#include <QStandardItem>

class ContextMenuEvent;
class InsertExpressionDialog;
class MembershipFunctionDialog;

class FuzzyRuleView : public QTreeView
{
    Q_OBJECT
public:
    explicit FuzzyRuleView(QWidget *parent = 0);

    void setLevelId(int l) { m_levelId = l; }

    void setOperatorItem(QStandardItem *item) { m_pOperatorItem = item; }
    
signals:
    
public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void onAddExpressionItem();
    void onDeleteExpressionItem();
    void onEditExpressionItem();

private:

    int m_levelId;

    QStandardItem *m_pOperatorItem;

    InsertExpressionDialog *m_pInsertExpressionDialog;

    MembershipFunctionDialog *m_pMembershipFunctionDialog;
};

#endif // FUZZYRULEVIEW_H
