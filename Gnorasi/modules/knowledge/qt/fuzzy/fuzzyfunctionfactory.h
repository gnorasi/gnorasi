#ifndef FUZZYFUNCTIONFACTORY_H
#define FUZZYFUNCTIONFACTORY_H

#include <QObject>

class FuzzyFunction;

/*!
 * \brief The FuzzyFunctionFactory class
 */
class FuzzyFunctionFactory : public QObject
{
    Q_OBJECT
public:\
    /*!
     * \brief FuzzyFunctionFactory
     * \param parent
     */
    explicit FuzzyFunctionFactory(QObject *parent = 0);

    /*!
     * \brief createFuzzyFunction
     * \return
     */
    FuzzyFunction* createFuzzyFunction(const QString &);
    
signals:
    
public slots:
    
};

#endif // FUZZYFUNCTIONFACTORY_H
