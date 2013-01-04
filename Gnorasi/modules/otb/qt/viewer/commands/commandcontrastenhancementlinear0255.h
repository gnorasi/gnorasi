#ifndef COMMANDCONTRASTENHANCEMENTLINEAR0255_H
#define COMMANDCONTRASTENHANCEMENTLINEAR0255_H

#include "command.h"

namespace itiviewer{

class ItiOtbVectorImageViewer;

/*!
 * \brief The CommandContrastEnhancementGaussian class
 *  this class encapsulates the itkGaussianOperator functionality ,
 *  plz see http://www.orfeo-toolbox.org/SoftwareGuide/SoftwareGuidech26.html#x54-36200026.4.1
 */
class CommandContrastEnhancementLinear0255 : public Command
{
    Q_OBJECT

public:
    explicit                CommandContrastEnhancementLinear0255(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);


    /*!
     * \brief execute
     */
    void                    execute();

private:

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
    
};

} // end of namespace itiviewer

#endif // COMMANDCONTRASTENHANCEMENTLINEAR0255_H
