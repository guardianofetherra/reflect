/* test_types.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 12 Apr 2014
   FreeBSD-style copyright and disclaimer apply

   Implementation of the type reflections.
*/

#include "test_types.h"

#include "dsl/basics.h"
#include "dsl/plumbing.h"
#include "dsl/field.h"
#include "dsl/operators.h"


/******************************************************************************/
/* OBJECT                                                                     */
/******************************************************************************/

reflectTypeImpl(test::Object)
{
    reflectPlumbing();
    reflectCons(int);

    reflectField(value);

    reflectFnTyped(ref, int& (test::Object::*)());
    reflectFnTyped(ref, void (test::Object::*)(int&));

    reflectFnTyped(constRef, const int& (test::Object::*)());
    reflectFnTyped(constRef, void (test::Object::*)(const int&));

    reflectFn(rref);

    reflectOperators();
}


/******************************************************************************/
/* NOT COPIABLE                                                               */
/******************************************************************************/

reflectTypeImpl(test::NotCopiable)
{
    reflectPlumbing();
}


/******************************************************************************/
/* NOT MOVABLE                                                                */
/******************************************************************************/

reflectTypeImpl(test::NotMovable)
{
    reflectPlumbing();
}


/******************************************************************************/
/* NOT CONSTRUCTIBLE                                                          */
/******************************************************************************/

reflectTypeImpl(test::NotConstructible)
{
    reflectPlumbing();
}


/******************************************************************************/
/* INTERFACE                                                                  */
/******************************************************************************/

reflectTypeImpl(test::Interface)
{
    reflectFn(pureVirtual);
}

/******************************************************************************/
/* PARENT                                                                     */
/******************************************************************************/

reflectTypeImpl(test::Parent)
{
    reflectParent(test::Interface);
    reflectPlumbing();
    reflectCons(test::Object, int);

    reflectField(value);
    reflectField(shadowed);

    reflectFn(normalVirtual);
}


/******************************************************************************/
/* CHILD                                                                      */
/******************************************************************************/

reflectTypeImpl(test::Child)
{
    reflectParent(test::Parent);
    reflectPlumbing();
    reflectCons(test::Object, int);

    reflectField(childValue);
    reflectField(shadowed);

    reflectFn(normalVirtual);
}


/******************************************************************************/
/* CONVERTIBLE                                                                */
/******************************************************************************/

reflectTypeImpl(test::Convertible)
{
    reflectPlumbing();
    reflectField(value);
    reflectOpCast(int);
    reflectOpCast(test::Parent);
}
