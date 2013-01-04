#ifndef COMMANDCONTRASTENHANCEMENTGAUSSIAN_H
#define COMMANDCONTRASTENHANCEMENTGAUSSIAN_H

#include "command.h"

namespace itiviewer{

class ItiOtbVectorImageViewer;

/*!
 * \brief The CommandContrastEnhancementGaussian class
 *  this class encapsulates the itkGaussianOperator functionality ,
 *  plz see http://www.orfeo-toolbox.org/SoftwareGuide/SoftwareGuidech26.html#x54-36200026.4.1
 */
class CommandContrastEnhancementGaussian : public Command
{
    Q_OBJECT
    Q_PROPERTY(double deviation READ deviation WRITE setDeviation   NOTIFY deviationChanged)

public:
    explicit                CommandContrastEnhancementGaussian(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);
    
    double                  deviation() const           { return m_deviation; }
    void                    setDeviation(double dev)    { m_deviation = dev; }

    /*!
     * \brief execute
     */
    void                    execute();

signals:
    void                    deviationChanged();

public slots:
    void                    updateDeviation(double v) { setDeviation(v); }

private:
    double                  m_deviation;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
    
};

} // end of namespace itiviewer

#endif // COMMANDCONTRASTENHANCEMENTGAUSSIAN_H
