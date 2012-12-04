#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

namespace itiviewer{

/*!
 * \brief The Command class
 *
 * \ingroup Visualization
 *
 *  This class is an interface class
 *  Whenever a new functionality is needed to be  implemented
 *  a new subclass of this class should be created.
 *  the new subclass should have a pointer to the receiver object
 *  the reveiver object should have the actual action needs to be implemented by the subclass command.
 */
class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = 0);

    //! every subclass must implement the execute function
    virtual void execute() = 0;

    
};

}

#endif // COMMAND_H
