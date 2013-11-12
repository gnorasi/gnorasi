#ifndef CLASSDESCRIPTIONDIALOG_H
#define CLASSDESCRIPTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>

#include <QPushButton>

class FuzzyRuleView;
class OntologyClass;

class ClassDescriptionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ClassDescriptionDialog(QWidget *parent = 0);
    
    void setupData();

    void clearFuzzyRules();

    QString lineEditText() const { return m_pQLineEdit->text(); }

    int levelId() const ;

    QString classId() const { return m_classId; }
    void setClassId(const QString &i ) { m_classId = i; }


    QString parentClassId() const { return m_parentClassId; }
    void setParentClassId(const QString &n) { m_parentClassId = n;}

signals:
    void ontologyClassAdded(int );
    
public slots:

private slots:
    void onOkClicked();
    void onLevelComboboxChanged(int );
    void onCancelClicked();
    void onFuzzyRuleAdded(int );

private:
    void initializeFuzzyOperators();

    void initializeFuzzyRuleTreeView();

    bool createNewClass();

    bool editClass();

    bool validateLabel(const QString& );

    void initialize();

    void setupObjectLevels();

    void processParentClass(OntologyClass* );

    QComboBox *m_pObjectLevelComboBox;

    QComboBox *m_pOperatorComboBox;

    QLineEdit *m_pQLineEdit;

    FuzzyRuleView *m_pFuzzyRuleView;

    QStandardItemModel *m_pFuzzyRuleModel;
    
    QPushButton *m_pOkButton;
    QPushButton *m_pCancelButton;

    QString m_classId;

    QString m_parentClassId;

    QStandardItem *m_pOperatorItem;
    QStandardItem *m_pInheritedItem;
};

#endif // CLASSDESCRIPTIONDIALOG_H
