#ifndef COMMANDCHANGECLASSCOLOR_H
#define COMMANDCHANGECLASSCOLOR_H


#include <QObject>
#include <QColor>
#include "command.h"


namespace itiviewer {

class ItiOtbVectorImageViewer;

class CommandChangeClassColor : public Command
{
    Q_OBJECT
    Q_PROPERTY(int  classLabelId    READ classLabelId   WRITE setClassLabelId   NOTIFY classLabelIdChanged)
public:
    explicit CommandChangeClassColor(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);
    
    QColor color() const {return m_color;}
    void setColor(const QColor &v) { m_color = v; }

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
    void updateData(const QString &color, int cid) { m_color.setNamedColor(color); m_classLabelId = cid; }

private:
    QColor m_color;

    int m_classLabelId;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
};

} // end of itiviewer namespace

#endif // COMMANDCHANGECLASSCOLOR_H
