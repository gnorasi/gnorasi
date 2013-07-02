#ifndef CLASSDESCRIPTIONDIALOG_H
#define CLASSDESCRIPTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>

#include <QPushButton>

class ClassExpressionsView;

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

    void onCancelClicked();

private:
    void initializeFuzzyRuleTreeView();

    void createNewClass();

    void editClass();

    bool validateLabel(const QString& );

    void initialize();

    void setupObjectLevels();

    QComboBox *m_pObjectLevelComboBox;

    QComboBox *m_pOperatorComboBox;

    QLineEdit *m_pQLineEdit;

    ClassExpressionsView *m_pClassExpressionsView;

    QStandardItemModel *m_pFuzzyRuleModel;
    
    QPushButton *m_pOkButton;
    QPushButton *m_pCancelButton;

    QString m_classId;

    QString m_parentClassId;
};

#endif // CLASSDESCRIPTIONDIALOG_H
