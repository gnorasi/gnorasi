#ifndef MEMBERSHIPFUNCTIONDIALOG_H
#define MEMBERSHIPFUNCTIONDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class FuzzyFunctionFactory;

class MembershipFunctionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MembershipFunctionDialog(int lid, const QString &atid, QWidget *parent = 0);

    void setupByRuleId(int );

    int levelId() const {return m_levelId; }
    void setLevelId(int i) { m_levelId = i; }

    void setAttribute(const QString &a) { m_attributeId =  a; }
    QString attributeId() const { return m_attributeId; }
    
signals:
    void fuzzyRuleAdded(int );
    
public slots:

private slots:
    void onOkClicked();
    void onCancelClicked();

    void onFuzzyFunctionButtonClicked(int );

private:
    void initialize();

    void setupAvailableFunctionsButtons();

    void createbuttons();

    QTableView *m_pParameterListView;

    QStandardItemModel *m_pParameterModel;

    FuzzyFunctionFactory *m_pFuzzyFunctionFactory;

    QButtonGroup *m_pButtonGroup;

    QLabel *m_pAttributeLabel;

    QHash<int,QString> m_helperHash;

    QString m_attributeId;

    int m_levelId;

    int m_currentButtonId;

    int m_ruleId;

    QPushButton *m_pOkButton;
    QPushButton *m_pCancelButton;

    QHBoxLayout *m_pHBoxButtonLayout;
    
};

#endif // MEMBERSHIPFUNCTIONDIALOG_H
