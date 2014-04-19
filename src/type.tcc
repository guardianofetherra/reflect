/* type.tcc                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 30 Mar 2014
   FreeBSD-style copyright and disclaimer apply

   Template implementation of Type.
*/

#include "reflect.h"
#pragma  once

namespace reflect {

/******************************************************************************/
/* TYPE                                                                       */
/******************************************************************************/

template<typename... Args>
Value
Type::
construct(Args&&... args) const
{
    return call<Value>(id(), std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
Ret
Type::
call(const std::string& fn, Args&&... args) const
{
    return function(fn).call<Ret>(std::forward<Args>(args)...);
}


template<typename Fn>
void
Type::
add(const std::string& name, Fn rawFn)
{
    Function fn(name, std::move(rawFn));
    fns_[name].add(std::move(fn));
}

} // namespace reflect
