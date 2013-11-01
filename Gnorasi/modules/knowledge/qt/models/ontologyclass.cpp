#include <QStringList>

#include "ontologyclass.h"

//using namespace voreen;

#include "../fuzzy/fuzzyrule.h"

OntologyClass::OntologyClass(OntologyClass *parent)
    : QObject(parent)
{
    parentItem = parent;
}


OntologyClass::~OntologyClass()
{
    qDeleteAll(childItems);
}


OntologyClass *OntologyClass::child(int number)
{
    return childItems.value(number);
}


int OntologyClass::childCount() const
{
    return childItems.count();
}


int OntologyClass::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OntologyClass*>(this));

    return 0;
}


OntologyClass *OntologyClass::parent()
{
    return parentItem;
}

void OntologyClass::removeFuzzyRule(int l, FuzzyRule *r){
    QList<FuzzyRule*> rList = m_fuzzyRuleHash.values(l);
    rList.removeOne(r);
    m_fuzzyRuleHash.remove(l);

    QList<FuzzyRule*>::const_iterator i;
    for(i = rList.begin(); i != rList.end(); i++){
        FuzzyRule *pr = *i;

        m_fuzzyRuleHash.insertMulti(l,pr);
    }
}

//!
//! BE CAREFUL!
//! REALLY DUMMY PARSING
//!
//! * HINT : DO NOT COUNT THE IF statements
//!
void OntologyClass::parseChildren(const QDomDocument &doc){
    //! helper headers
    QString classTagName            = QString::fromUtf8("owl:Class");
    QString subClassTagName         = QString::fromUtf8("rdfs:subClassOf");
    QString labelTagName            = QString::fromUtf8("rdfs:label");
    QString commentTagName          = QString::fromUtf8("rdfs:comment");
    QString resourceAttributeKey    = QString::fromUtf8("rdf:resource");
    QString idAttributeKey          = QString::fromUtf8("rdf:ID");
    QString aboutAttributeKey       = QString::fromUtf8("rdf:about");

    //! these are the values of the dom elements id, label and comment
    QString id, label, comment;

    //! a helper variable
    bool helperVal = false;

    //! get the first node
    QDomNode node = doc.firstChild();

    //! the second node is probably the node we need for iterating
    QDomNode snode = node.nextSibling();

    QDomNode childNode = snode.firstChild();

    //! iterate through child items
    while(!childNode.isNull()){

        if(childNode.isElement()){ // check whether the node is an element

            //! get element
            QDomElement childElement = childNode.toElement();
            QString tagName = childElement.tagName();
            if(!tagName.compare(classTagName)){ // meaning that this is an ontology class

                //! set helper value to false
                helperVal = false;
                //! clear the QString values..
                label.clear();
                comment.clear();

                //! set the id
                if(childElement.hasAttribute(idAttributeKey))
                    id = childElement.attribute(idAttributeKey);
                else //! check whether is of about type
                    id = childElement.attribute(aboutAttributeKey);

                //! remove '#' character at the position 0 of the id
                if(!id.isEmpty() && id.at(0) == QChar('#'))
                    id.remove(0,1);

                //! iterate through child items to find the subClassOf item and the other attribute elements
                QDomNode grandChildNode = childNode.firstChild();
                while(!grandChildNode.isNull()){ //! iterate through child nodes

                    if(!grandChildNode.hasChildNodes()){ //! check whether there is any number of child nodes

                        QDomElement grandChildElement = grandChildNode.toElement();
                        QString grandChildTagName = grandChildElement.tagName();
                        //! check whether it is a subclass type element
                        if(!grandChildTagName.compare(subClassTagName)){

                            //! the parent class name is set as an attribute of this element
                            if(grandChildElement.hasAttribute(resourceAttributeKey)){

                                //! get the value of ther attribute and check whether this equals to the id of this item
                                QString resourceAttributeVal = grandChildElement.attribute(resourceAttributeKey);
                                QString key = QString("#%1").arg(m_id);
                                if(!resourceAttributeVal.compare(key)) //
                                    helperVal = true;

                            }
                        }
                        else if(!grandChildTagName.compare(labelTagName)){ // check whether it is a label type
                            label = grandChildElement.text();
                        }else if(!grandChildTagName.compare(commentTagName)){ // check whether it is a comment type
                            comment = grandChildElement.text();
                        }
                    }else{ //! there is a number of child nodes

                        if(grandChildNode.isElement()) { //! then probably is a comment type or a label, maybe a subclass though

                            QDomElement grandChildElement = grandChildNode.toElement();
                            QString grandChildElementTagName = grandChildElement.tagName();
                            if(!grandChildElementTagName.compare(subClassTagName)){

                                //! the parent class name is set as an attribute of this element
                                if(grandChildElement.hasAttribute(resourceAttributeKey)){

                                    //! get the value of ther attribute and check whether this equals to the id of this item
                                    QString resourceAttributeVal = grandChildElement.attribute(resourceAttributeKey);
                                    QString key = QString("#%1").arg(m_id);
                                    if(!resourceAttributeVal.compare(key))  //
                                        helperVal = true;

                                }else{
                                    QDomNode classNode = grandChildNode.firstChild();
                                    if(classNode.isElement()){

                                        QDomElement attributeElement = classNode.toElement();
                                        QString attributeTagName = attributeElement.tagName();
                                        if(!attributeTagName.compare(classTagName)){

                                            //! check whether it has an id attribute and whether is is equal to this items m_id
                                            if(attributeElement.hasAttribute(idAttributeKey) && !attributeElement.attribute(idAttributeKey).compare(m_id))
                                                helperVal = true;
                                            else if(attributeElement.hasAttribute(aboutAttributeKey)){ // check the same for the resource attribute

                                                //! get the value of ther attribute and check whether this equals to the id of this item
                                                QString aboutAttributeVal = attributeElement.attribute(aboutAttributeKey);
                                                QString key = QString("%1").arg(m_id);
                                                if(!aboutAttributeVal.compare(key)) //
                                                    helperVal = true;

                                            }
                                        }
                                    }
                                }
                            }
                            else if(!grandChildElementTagName.compare(labelTagName)){ // check whether it is a label type
                                label = grandChildElement.text();
                            }else if(!grandChildElementTagName.compare(commentTagName)){ // check whether it is a comment type
                                comment = grandChildElement.text();
                            }
                        }
                        else{ //! then probably there is a sub node holding the subclass attribute value

                            QDomNode classNode = grandChildNode.firstChild();
                            if(classNode.isElement()){

                                QDomElement attributeElement = classNode.toElement();
                                QString attributeTagName = attributeElement.tagName();
                                if(!attributeTagName.compare(classTagName)){

                                    if(attributeElement.hasAttribute(idAttributeKey) && !attributeElement.attribute(idAttributeKey).compare(m_id))
                                        helperVal = true;
                                    else if(attributeElement.hasAttribute(aboutAttributeKey)){

                                        //! get the value of ther attribute and check whether this equals to the id of this item
                                        QString aboutAttributeVal = attributeElement.attribute(aboutAttributeKey);
                                        QString key = QString("#%1").arg(m_id);
                                        if(!aboutAttributeVal.compare(key)) //
                                            helperVal = true;

                                    }
                                }
                            }
                        }
                    }

                    //! get the next sibling
                    grandChildNode = grandChildNode.nextSibling();
                }

                //! if a ObjectDepiction is found then
                if(helperVal && checkSiblings(id)){
                    //! create a OntologyClass
                    if(label.isEmpty())
                        label = id;

                    OntologyClass * item = new OntologyClass(this);
                    //id,label,comment,
                    item->setId(id);
                    item->setName(id);

                    /*
                     *
                     *
                     * add the ontology class item to the list
                     *
                     *
                     */
                }

            } //! end of checking whether this is a class type node

        } //! end of checking whether the childNode is an element

        childNode = childNode.nextSibling();
    }

    //! parse all the siblings for finding their children
    parseSiblings(doc);
}
void OntologyClass::parseSiblings(const QDomDocument &doc){

//    QList<OntologyClass*>::const_iterator i;
//    for(i = m_owlchildren.constBegin(); i != m_owlchildren.constEnd(); i++){
//        OntologyClass *item = *i;

//        //! parse for children items
//        item->parseChildren(doc);
//    }
}


// Checks whether there is already a sibling item with the same id
bool OntologyClass::checkSiblings(const QString &cid){
//    QList<OntologyClass*>::const_iterator i;
//    for(i = m_owlchildren.constBegin(); i != m_owlchildren.constEnd(); i++){
//        OntologyClass *siblingItem = *i;
//        if(!siblingItem->id().compare(cid))
//            return false;
//    }

    return true;
}
