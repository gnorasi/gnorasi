#ifndef RULEITEMVIEW_H
#define RULEITEMVIEW_H

#include <QTableView>

class ContextMenuEvent;

namespace voreen {

class Rule;

class RuleItemView : public QTableView
{
    Q_OBJECT
public:
    explicit RuleItemView(QWidget *parent = 0);

    QString ruleId() const { return m_rule_id; }
    void setRuleId(const QString &r){ m_rule_id = r; }
    
public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void onAddRuleItem();
    void onDeleteRuleItem();

private:
    Rule* getRule();

    QString m_rule_id;
};

}

#endif // RULEITEMVIEW_H
