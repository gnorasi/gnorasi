#ifndef FUZZYFUNCTION_H
#define FUZZYFUNCTION_H

#include <QObject>
#include <QStringList>

#define MAXNUMBEROFPARAMETERS 3

class FuzzyFunction;

/*!
 * \brief The FuzzyFunction class
 *  This is an interface class that provides the interface for all specialized , concrete sub classes
 *  It supports a set of pure virtual functions
 */
class FuzzyFunction : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief parametersCount
     *  This is used on enabling and disabling the table fields,
     * \return
     */
    virtual int parametersCount() const = 0;
    
    /*!
     * \brief parameterFunctionForIndex
     *  This function supports fast access to the parameter values , from the GUI side.
     * \param i
     * \return
     */
    virtual double parameterFunctionForIndex(int i) = 0;

    /*!
     * \brief MAXPARAMETERSCOUNT
     *  This is a general static value , for setting up the table view columns.
     */
    static const int MAXPARAMETERSCOUNT = MAXNUMBEROFPARAMETERS;


    /*!
     * \brief availableFunctions
     *  Another list providing all the available names of the concrete function classes.
     */
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
