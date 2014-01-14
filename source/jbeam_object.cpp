#include "jbeam_object.h"

jbeam_object::jbeam_object()
{
    childL = NULL;
    childR = NULL;
    parent = NULL;
    token = KEY;
    objectparent=0;
    iterated = 0;
    stophere = false;
}

jbeam_object::~jbeam_object()
{
    if(childL!=NULL)
    {
        delete childL;
        childL = NULL;
    }
    if(childR!=NULL)
    {
        delete childR;
        childR = NULL;
    }
}

/* Left child goes lower in a tree */
void jbeam_object::NewLeftChild()
{
    if(childL == NULL)
    {
        childL = new jbeam_object;
        childL->parent = this;
        childL->actualparent = this;
    }
    else
    {
        childL->NewRightChild();
    }

}

/* Right child is in the same level in a tree */
void jbeam_object::NewRightChild()
{
    if(childR == NULL)
    {
        childR = new jbeam_object;
        childR->parent = this;
        childR->actualparent = actualparent;
    }
    else
    {
        childR->NewRightChild();
    }
}

/* Delete all child nodes */
void jbeam_object::Delete()
{
    if(childR != NULL)
    {
        childR->Delete();
    }
    if(childL != NULL)
    {
        childL->Delete();

    }
    delete childR;
    delete childL;
    childR=NULL;
    childL=NULL;
}

void jbeam_object::ResetIterated()
{
    if(childR != NULL)
    {
        childR->ResetIterated();
    }
    if(childL != NULL)
    {
        childL->ResetIterated();

    }
    iterated=0;
}

