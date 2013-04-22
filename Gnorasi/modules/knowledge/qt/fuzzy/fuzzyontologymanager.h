#ifndef FUZZYONTOLOGYMANAGER_H
#define FUZZYONTOLOGYMANAGER_H

#include <QObject>


#define FUZZYONTOLOGYCLASSMANAGER FuzzyOntologyManager::instance()

class FuzzyOntologyClass;

/*!
 * \brief The FuzzyOntologyManager class
 *
 *  This is the manager for the fuzzy ontology sub module
 */
class FuzzyOntologyManager : public QObject
{
    Q_OBJECT
public:

    static FuzzyOntologyManager* instance() ;


    static void deleteInstance();
    
    /*!
     * \brief addFuzzyOntologyClass
     * \param c
     */
    void addFuzzyOntologyClass(FuzzyOntologyClass* c) { m_fuzzyOntolgyClassList.append(c); }

    /*!
     * \brief deleteFuzzuyOntologyClass
     * \param c
     */
    void deleteFuzzuyOntologyClass(FuzzyOntologyClass * c) { m_fuzzyOntolgyClassList.removeOne(c);}

    /*!
     * \brief fuzzyOntologyClasses
     * \return
     */
    QList<FuzzyOntologyClass*> fuzzyOntologyClasses() const  { return m_fuzzyOntolgyClassList; }

    /*!
     * \brief clear
     */
    void clear() { m_fuzzyOntolgyClassList.clear(); }


    FuzzyOntologyClass* fuzzyByOntologyClassByName(const QString &name);

signals:
    
public slots:
    
private:
    /*!
     * \brief FuzzyOntologyManager
     * \param parent
     */
    explicit FuzzyOntologyManager(QObject *parent = 0);


    ~FuzzyOntologyManager();

    /*!
     * \brief m_fuzzyOntolgyClassList
     */
    QList<FuzzyOntologyClass*> m_fuzzyOntolgyClassList;

    /*!
     * \brief m_pInstance
     */
    static FuzzyOntologyManager *m_pInstance;

};

#endif // FUZZYONTOLOGYMANAGER_H
