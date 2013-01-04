#ifndef COMMANDCONTRASTENHANCEMENTSQUAREROOT_H
#define COMMANDCONTRASTENHANCEMENTSQUAREROOT_H

#include "command.h"

namespace itiviewer{

class ItiOtbVectorImageViewer;

/*!
 * \brief The CommandContrastEnhancementGaussian class
 *  this class encapsulates the itkGaussianOperator functionality ,
 *  plz see http://www.orfeo-toolbox.org/SoftwareGuide/SoftwareGuidech26.html#x54-36200026.4.1
 */
class CommandContrastEnhancementSquareRoot : public Command
{
    Q_OBJECT
    Q_PROPERTY(double upperQuantile READ upperQuantile WRITE setUpperQuantile   NOTIFY upperQuantileChanged)
    Q_PROPERTY(double lowerQuantile READ lowerQuantile WRITE setLowerQuantile   NOTIFY lowerQuantileChanged)

public:
    explicit                CommandContrastEnhancementSquareRoot(ItiOtbVectorImageViewer *viewer, QObject *parent = 0);
    
    double                  upperQuantile() const           { return m_upperQuantile; }
    void                    setUpperQuantile(double v)      { m_upperQuantile = v; }

    double                  lowerQuantile() const           { return m_lowerQuantile; }
    void                    setLowerQuantile(double v)      { m_lowerQuantile = v; }

    /*!
     * \brief execute
     */
    void                    execute();

signals:
    void                    upperQuantileChanged();

    void                    lowerQuantileChanged();

public slots:
    void                    updateUpperQuantile(double v) { setUpperQuantile(v); }

    void                    updateLowerQuantile(double v) { setLowerQuantile(v); }


private:
    /*!
     * \brief m_upperQuantile, a variable for holding the upper quantile value
     */
    double m_upperQuantile;

    /*!
     * \brief m_upperQuantile, a variable for holding the lower quantile value
     */
    double m_lowerQuantile;

    ItiOtbVectorImageViewer *m_pItiOtbVectorImageViewer;
    
};

} // end of namespace itiviewer

#endif // COMMANDCONTRASTENHANCEMENTSQUAREROOT_H
