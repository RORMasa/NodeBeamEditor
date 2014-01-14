#ifndef JBEAM_H
#define JBEAM_H
#include <QString>
#include <QTextStream>
#include "jbeam_object.h"

/* A class used to parse everything from a .jbeam file and to place
 * the information in a tree structure, where from editor can copy the sections
 * that it understands to its own data structures.
 *. */

class JBeam
{
public:
    /* file is parsed and all information placed in a binary tree */
    JBeam(const QString &fileName);
    ~JBeam();

    /* Writes the current binary tree to a file */
    void WriteFile(const QString &fileName);

    /* Print tree in console without any sort of ordering */
    void PrintTree();

    /* Print tree in console like in output file */
    void PrintTree3();

    /* Find object by key */
    bool FindObjectByKey(QString key);


    /* GoToNext() moves CurrentPos pointer to the next object in a tree, returns integer telling the direction
    1 = going down (left object) in a tree
    2 = going right on the same level (right object) in a tree
    3 = going up in a tree
    */
    int GoToNext();
    int GoToNextRight();
    void CurrPosReset();
    void ResetIterated();

    /* Same as go to next, but returns 4 when in stop point */
    int GoToNextUntilStop();
    int GoToNextUntilStop_List();
    /* Set stop point, does not set it if there is no child nodes */
    bool SetStopPointHere();
    bool SetStopPointHere_List();

    /* inserts empty object to CurrentPos */
    void InsertObject();

    /* Pointer used to move around in the tree */
    jbeam_object *CurrentPos;
    enum tokens {KEY, VALUE, END, LIST};
    enum {STRING, ARRAY, NUMBER, OBJECT, LIST_STR, LIST_NUMBER, LIST_CONTAINER, BOOLVAL};

    /**/
    bool GoToNextSlot();
    void ResetSlot();
    int CurrentSlot;

private:
    QString string;
    QString cha;
    QTextStream *in;
    jbeam_object * newobject;
    jbeam_object JBeamFile;

    void OpenFile();
    void ReadString();
    void ReadComment();
    void ReadNumber();
    void FindParent();
    void FindEndOfList();
    void NewObject();
    void EndObject();
    void HandleString();
    void HandleNumber();
    void NewList();
    int CheckBoolean();
    QString WriteFileIndent(int indentsize);

    //total amount of objects in the tree
    int object_amount;

    /* Comment */
    void SaveComments();
    bool UnsavedComments;
    QVector <QString> comments;
};



#endif // JBEAM_H
