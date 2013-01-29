#ifndef COMMANDTOGGLECLASSIFICATIONLABELVISIBILTIY_H
#define COMMANDTOGGLECLASSIFICATIONLABELVISIBILTIY_H


#include <QObject>

#include "command.h"


namespace itiviewer {

class ItiOtbVectorImageViewer;

class CommandToggleClassificationLabelVisibiltiy : public Command
{
    Q_OBJECT
    Q_PROPERTY(bool visible         READ visible        WRITE setVisible        NOTIFY visibleChanged)
    Q_PROPERTY(int  classLabelId    READ classLabelId   WRITE setClassLabelId   NOTIFY classLabelIdChanged)
public:
    explicit CommandToggleClassificationLabelVisibiltiy(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);
    
    bool visible() const {return m_visible;}
    void setVisible(bool v) { m_visible = v; }

    int classLabelId() const {return m_classLabelId; }
    void setClassLabelId(int cl) { m_classLabelId = cl; }

    /*!
     * \brief execute
     */
    void execute();

signals:
    void visibleChanged();
    void classLabelIdChanged();

public slots:
    void updateData(bool checked, int cid) { m_visible = checked; m_classLabelId = cid; }

private:
    bool m_visible;

    int m_classLabelId;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
};

} // end of itiviewer namespace

#endif // COMMANDTOGGLECLASSIFICATIONLABELVISIBILTIY_H
