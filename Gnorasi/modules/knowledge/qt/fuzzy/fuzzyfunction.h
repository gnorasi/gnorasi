#ifndef FUZZYFUNCTION_H
#define FUZZYFUNCTION_H

#include <QObject>
#include <QStringList>

#define MAXNUMBEROFPARAMETERS 3

class FuzzyFunction;

class FuzzyFunction : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief parametersCount
     * \return
     */
    virtual int parametersCount() const = 0;
    
    /*!
     * \brief parameterFunctionForIndex
     * \param i
     * \return
     */
    virtual double parameterFunctionForIndex(int i) = 0;

    /*!
     * \brief MAXPARAMETERSCOUNT
     */
    static const int MAXPARAMETERSCOUNT = MAXNUMBEROFPARAMETERS;


    static QStringList availableFunctions;


protected:
    /*!
     * \brief FuzzyFunction
     * \param parent
     */
    explicit FuzzyFunction(QObject *parent = 0) : QObject(parent) { }

signals:
    
public slots:
    
private:



};

#endif // FUZZYATTRIBUTE_H
