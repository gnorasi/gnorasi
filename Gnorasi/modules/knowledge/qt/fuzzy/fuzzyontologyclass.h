#ifndef FUZZYONTOLOGYCLASS_H
#define FUZZYONTOLOGYCLASS_H

#include <QObject>

class FuzzyAttribute;
class FuzzyFunctionFactory;

/*!
 * \brief The FuzzyOntologyClass class
 *  The FuzzyOntologyClass class is related to a set of FuzzyAttribute items
 *  The FuzzyOntologyClass item is also related to a min / max operator value.
 *
 */
class FuzzyOntologyClass : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief The MINMAXTYPE enum
     */
    enum MINMAXTYPE{
        MMT_NOTSET  = 0,
        MMT_MIN     = 1,
        MMT_MAX     = 2
    };

    explicit FuzzyOntologyClass(QObject *parent = 0);

    /*!
     * \brief addFuzzyAttribute
     * \param a
     */
    void addFuzzyAttribute(FuzzyAttribute *a) { m_fuzzyAttributeList.append(a); }

    /*!
     * \brief removeFuzzyAttribute
     * \param a
     */
    bool removeFuzzyAttribute(FuzzyAttribute *a) { return m_fuzzyAttributeList.removeOne(a); }

    /*!
     * \brief removeFuzzyAttribute
     * \param name
     */
    bool removeFuzzyAttribute(const QString &name);

    /*!
     * \brief fuzzyAttributes
     * \return
     */
    QList<FuzzyAttribute*> fuzzyAttributes() { return m_fuzzyAttributeList;}

    /*!
     * \brief clear
     */
    void clear() { m_fuzzyAttributeList.clear(); }


    /*!
     * \brief minMaxType
     * \return
     */
    MINMAXTYPE minMaxType() { return m_minMaxType; }

    /*!
     * \brief setMinMaxType
     * \param m
     */
    void setMinMaxType(MINMAXTYPE m) { m_minMaxType = m; }

    /*!
     * \brief setMinMaxType
     * \param val
     */
    void setMinMaxType(int val) { val == 1? m_minMaxType = MMT_MIN : m_minMaxType = MMT_MAX; }

    /*!
     * \brief isMin
     * \return
     */
    bool isMin() { return m_minMaxType == MMT_MIN? true: false; }

    /*!
     * \brief className
     *  getter
     * \return
     */
    QString className() const { return m_className; }
    void setClassName(const QString &name) { m_className = name ; }


    int id() const { return m_id; }
    void setId(int i ) { m_id = i; }

    /*!
     * \brief isEmpty
     * \return
     */
    bool isEmpty() { return m_fuzzyAttributeList.isEmpty(); }

    /*!
     * \brief updateAttribute
     * \return
     */
    bool updateAttribute(int , int , const QVariant&, FuzzyFunctionFactory* );

    /*!
     * \brief getAttributeByName
     * \return
     */
    FuzzyAttribute* getAttributeByName(const QString &);

    /*!
     * \brief getAttributeByIndex
     * \param index
     * \return
     */
    FuzzyAttribute* getAttributeByIndex(int index);


signals:
    
public slots:


private:

    QString createANewName(const QString &name);

    /*!
     * \brief m_className
     */
    QString m_className;

    /*!
     * \brief m_id
     */
    int m_id;

    /*!
     * \brief m_fuzzyAttributeList
     */
    QList<FuzzyAttribute*> m_fuzzyAttributeList;

    /*!
     * \brief m_minMaxType
     */
    MINMAXTYPE m_minMaxType;

    
};

#endif // FUZZYONTOLOGYCLASS_H
