#ifndef FUZZYFUNCTIONLEFTSHOULDER_H
#define FUZZYFUNCTIONLEFTSHOULDER_H

#include <QObject>

#include "fuzzyfunction.h"

/*!
 * \brief The FuzzyFunctionLeftShoulder class
 */
class FuzzyFunctionLeftShoulder : public FuzzyFunction
{
    Q_OBJECT
public:
    explicit FuzzyFunctionLeftShoulder(QObject *parent = 0);

    /*!
     * \brief paramtersCount
     * \return
     */
    int parametersCount() const { return 2; }

    /*!
     * \brief parameterFunctionForIndex
     * \param index
     * \return
     */
    double parameterFunctionForIndex(int index);

    /*!
     * \brief setParameterA
     * \param a
     */
    void setParameterA(double a) { m_parameterA = a; }
    void setParameterB(double b) { m_parameterB = b; }

    /*!
     * \brief parameterA
     * \return
     */
    double parameterA() const { return m_parameterA; }
    double parameterB() const { return m_parameterB; }
    

signals:
    
public slots:

private:

    double m_parameterA;
    double m_parameterB;
    
};

#endif // FUZZYFUNCTIONLEFTSHOULDER_H
