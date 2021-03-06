/* registry.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 16 Mar 2014
   FreeBSD-style copyright and disclaimer apply

   Static global state for the reflection registry.
*/

#include "reflect.h"

#include <mutex>

namespace reflect {

/******************************************************************************/
/* REGISTRY STATE                                                             */
/******************************************************************************/

namespace {

struct RegistryState
{
    std::mutex lock;
    std::unordered_map<std::string, const Type*> types;
    std::unordered_map<std::string, std::string> aliases;
    std::unordered_map<std::string, std::function<void(Type*)> > loaders;
    Scope scopes;
};

RegistryState* registry_ = nullptr;

RegistryState& getRegistry()
{
    if (!registry_) registry_ = new RegistryState();
    return *registry_;
}

} // namespace anonymous


/******************************************************************************/
/* REGISTRY                                                                   */
/******************************************************************************/

Scope*
Registry::
globalScope()
{
    return &getRegistry().scopes;
}

const Type*
Registry::
get(const std::string& id)
{
    auto& registry = getRegistry();
    std::lock_guard<std::mutex>(registry.lock);

    const std::string* pId = &id;

    auto aliasIt = registry.aliases.find(*pId);
    if (aliasIt != registry.aliases.end())
        pId = &aliasIt->second;

    auto typeIt = registry.types.find(*pId);
    return typeIt != registry.types.end() ? typeIt->second : load(*pId);
}


const Type*
Registry::
load(const std::string& id)
{
    if (id.empty()) reflectError("can't add type for <%s>", id);

    auto& registry = getRegistry();

    auto it = registry.loaders.find(id);
    if (it == registry.loaders.end())
        reflectError("no loader found for <%s>", id);

    auto loader = std::move(it->second);
    registry.loaders.erase(it);

    Type* type;
    add(id, type = new Type(id));

    loader(type);
    return type;
}

void
Registry::
add(const std::string& id, const Type* type)
{
    if (id.empty() || !type)
        reflectError("can't add type for <%s>", id);

    auto& registry = getRegistry();

    auto ret = registry.types.emplace(id, type);
    if (!ret.second) reflectError("<%s> already has a type", id);
}

void
Registry::
add(const std::string& id, std::function<void(Type*)> loader)
{
    if (id.empty() || !loader)
        reflectError("can't add loader for<%s>", id);

    auto& registry = getRegistry();
    std::lock_guard<std::mutex>(registry.lock);

    // If we already have a loader then too-bad.
    registry.loaders.emplace(std::move(id), std::move(loader));
    registry.scopes.addType(id);
}

void
Registry::
alias(const std::string& id, const std::string& alias)
{
    if (id.empty() || alias.empty())
        reflectError("<%s> can't be aliased to <%s>", alias, id);

    auto& registry = getRegistry();
    std::lock_guard<std::mutex>(registry.lock);

    auto ret = registry.aliases.emplace(std::move(alias), std::move(id));
    if (!ret.second) {
        reflectError(
                "<%s> can't be aliased to <%s> because it's already aliased to <%s>",
                alias, id, ret.first->second);
    }
}


/******************************************************************************/
/* UTILS                                                                      */
/******************************************************************************/

Scope* scope(const std::string& name)
{
    Scope* global = Registry::globalScope();
    const Scope* cglobal = global;
    return name.empty() ? global : cglobal->scope(name);

}


} // reflect
