/* function.h                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 30 Mar 2014
   FreeBSD-style copyright and disclaimer apply

   Function reflection
*/

#include "reflect.h"
#pragma once

namespace reflect {

/******************************************************************************/
/* TRAITS                                                                     */
/******************************************************************************/

#define reflectFnTrait(fn, trait)               \
    type_->function(#fn).addTrait(#trait)

#define reflectFnValue(fn, trait, value)                \
    type_->function(#fn).addTrait(#trait, value)


/******************************************************************************/
/* CONS                                                                       */
/******************************************************************************/

template<typename T, typename... Args>
void reflectConstructor(Type* type)
{
    type->addFunction(type->id(), [] (Args... args) {
                return T(std::forward<Args>(args)...);
            });
}

#define reflectCons(...)                        \
    reflect::reflectConstructor<T_, __VA_ARGS__>(type_)


/******************************************************************************/
/* ALLOC                                                                      */
/******************************************************************************/

template<typename T>
void reflectAllocator(Type* type)
{
    type->addFunction("new", [] { return new T; });
}

#define reflectAlloc()                          \
    reflect::reflectAllocator<T_>(type_)


/******************************************************************************/
/* REFLECT FN                                                                 */
/******************************************************************************/

template<typename Fn>
void reflectFunction(Type* type, std::string name, Fn fn)
{
    type->addFunction(std::move(name), std::move(fn));
}

#define reflectFn(fn)                           \
    reflect::reflectFunction(type_, #fn, &T_::fn)

#define reflectFnTyped(fn, signature)           \
    reflect::reflectFunction<signature>(type_, #fn, &T_::fn)


/******************************************************************************/
/* REFLECT CUSTOM                                                             */
/******************************************************************************/

struct AddLambdaToType
{
    AddLambdaToType(Type* type, std::string name) :
        type(type), name(std::move(name))
    {}

    template<typename Fn>
    void operator+= (Fn fn)
    {
        type->addFunction(std::move(name), std::move(fn));
    }

private:
    Type* type;
    std::string name;
};

inline AddLambdaToType reflectLambda(Type* type, std::string name)
{
    return AddLambdaToType(type, std::move(name));
}

#define reflectCustom(name)                     \
    reflect::reflectLambda(type_, #name) += []

} // reflect
