#pragma once

#include <QString>
#include <QVector>

class jbeam_object
{
public:
    jbeam_object();
    ~jbeam_object();
    jbeam_object *parent;
    jbeam_object *childR;
    jbeam_object *childL;

    /* pointer to actual parent object */
    jbeam_object *actualparent;

    /*tokens used in parsing of file*/
    enum tokens { KEY, VALUE, END, LIST };
    int token;

    /*token used for content type*/
    enum {STRING, ARRAY, NUMBER, OBJECT, LIST_STR, LIST_NUMBER, LIST_CONTAINER, BOOLVAL};
    int contenttype;

    //OBJECT

    QString key;
    QString value;

    void NewLeftChild();
    void NewRightChild();
    void Delete();

    bool iterated;
    bool stophere;
    void ResetIterated();
    bool objectparent;

    /* Comments that were right before this object */
    QVector <QString> comments;
};
