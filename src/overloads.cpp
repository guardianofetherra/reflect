/* overloads.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 15 Apr 2014
   FreeBSD-style copyright and disclaimer apply

   Overloads implementation
*/

#include "reflect.h"

namespace reflect {

/******************************************************************************/
/* OVERLOADS                                                                  */
/******************************************************************************/

void
Overloads::
add(Function fn)
{
    for (const auto& other : overloads) {
        if (fn.test(other) != Match::Exact) continue;

        reflectError("<%s, %s> is ambiguous with <%s, %s>",
                fn.name(), signature(fn),
                other.name(), signature(other));
    }

    overloads.emplace_back(std::move(fn));
}

bool
Overloads::
test(const Function& fn) const
{
    for (const auto& other : overloads) {
        if (fn.test(other) != Match::None) return true;
    }
    return false;
}

bool
Overloads::
test(const Argument& ret, const std::vector<Argument>& args) const
{
    for (const auto& fn : overloads) {
        if (fn.test(ret, args) != Match::None) return true;
    }
    return false;
}

const Function&
Overloads::
get(const Argument& ret, const std::vector<Argument>& args) const
{
    for (const auto& fn : overloads) {
        if (fn.test(ret, args) != Match::None) return fn;
    }

    reflectError("no overload <%s> available for function <%s>",
            signature(ret, args), name());
}

std::string
Overloads::
name() const
{
    if (overloads.empty()) return "?";
    return overloads.front().name();
}

std::string
Overloads::
print(size_t indent) const
{
    std::stringstream ss;
    std::string pad(indent, ' ');

    if (!traits().empty())
        ss << pad << Traits::print() << "\n";

    for (const auto& fn : overloads)
        ss << pad << signature(fn) << "\n";

    return ss.str();
}

} // reflect
