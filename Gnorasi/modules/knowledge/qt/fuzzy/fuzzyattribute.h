#ifndef FUZZYATTRIBUTE_H
#define FUZZYATTRIBUTE_H

#include <QObject>
#include <QStringList>

class FuzzyFunction;

/*!
 * \brief The FuzzyAttribute class
 */
class FuzzyAttribute : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief The FUZZYOPERATORTYPE enum
     */
    enum FUZZYOPERATORTYPE{
        FOT_NOTSET          = -1,
        FOT_GREATEREEQUAL   = 0,
        FOT_GREATER         = 1,
        FOT_LESSEQUAL       = 2,
        FOT_LESS            = 3
    };

    /*!
     * \brief FuzzyAttribute
     * \param n
     * \param parent
     */
    explicit FuzzyAttribute(const QString &n , QObject *parent = 0);

    /*!
     * \brief id
     * \return
     */
    QString name() const { return m_name; }

    /*!
     * \brief setMemberShipValues
     * \param v
     */
    void setMemberShipValues(double v) { m_memberShipValue = v; }

    /*!
     * \brief memberShipValue
     * \return
     */
    double memberShipValue() const { return m_memberShipValue; }
    
    /*!
     * \brief fuzzuFunction
     * \return
     */
    FuzzyFunction* fuzzuFunction( ) { return m_pFuzzyFunction; }

    /*!
     * \brief setFuzzuFunction
     * \param f
     */
    void setFuzzuFunction( FuzzyFunction *f) { m_pFuzzyFunction = f;}

    /*!
     * \brief parameterValueForIndex
     * \param index
     * \return
     */
    double parameterValueForIndex(int index) const ;

    /*!
     * \brief fuzzyOperator
     * \return
     */
    FUZZYOPERATORTYPE fuzzyOperator( ) { return m_fuzzyOperator;}

    /*!
     * \brief setFuzzyOperator
     * \param f
     */
    void setFuzzyOperator(FUZZYOPERATORTYPE f) { m_fuzzyOperator = f; }

    /*!
     * \brief fuzzyOperatorNames
     */
    static QStringList fuzzyOperatorNames;

    /*!
     * \brief fuzzyOperatorName
     * \return
     */
    QString fuzzyOperatorName() ;

signals:
    
public slots:
    
private:
    double m_memberShipValue;

    QString m_name;

    FuzzyFunction *m_pFuzzyFunction;

    FUZZYOPERATORTYPE m_fuzzyOperator;

};

#endif // FuzzyAttribute_H
