#ifndef MEMBERSHIPFUNCTIONWIDGET_H
#define MEMBERSHIPFUNCTIONWIDGET_H

#include <QWidget>
#include <QListView>

class MembershipFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MembershipFunctionWidget(QWidget *parent = 0);
    
signals:
    
public slots:

private:
    void initialize();

    void setupAvailableFunctionsButtons();

    QListView *m_pParameterListView;
    
};

#endif // MEMBERSHIPFUNCTIONWIDGET_H
