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


    void setClassId(const QString &o) { m_classId = o; }
    
signals:
    void fuzzyRuleAdded(int);
    
public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void onAddExpressionItem();
    void onDeleteFuzzyRule();
    void onEditExpressionItem();
    void onChangeOperator();

private:

    bool inheritsFromInheritedItem(QStandardItem* );

    int m_levelId;

    QString m_classId;

    QStandardItem *m_pOperatorItem;

    InsertExpressionDialog *m_pInsertExpressionDialog;

    MembershipFunctionDialog *m_pMembershipFunctionDialog;
};

#endif // FUZZYRULEVIEW_H
