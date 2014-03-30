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

template<typename Fn>
void
Type::
add(const std::string& name, Fn rawFn)
{
    Function fn(name, std::move(rawFn));
    fields_[name].add(std::move(fn));
}

} // namespace reflect
