// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#pragma once

#include "Luau/Variant.h"
#include "Luau/Symbol.h"

#include <map> // TODO: Kill with LuauLValueAsKey.
#include <memory>
#include <unordered_map>

namespace Luau
{

struct TypeVar;
using TypeId = const TypeVar*;

struct Field;
using LValue = Variant<Symbol, Field>;

struct Field
{
    std::shared_ptr<LValue> parent;
    std::string key;

    bool operator==(const Field& rhs) const;
    bool operator!=(const Field& rhs) const;
};

struct LValueHasher
{
    size_t operator()(const LValue& lvalue) const;
};

const LValue* baseof(const LValue& lvalue);

std::optional<LValue> tryGetLValue(const class AstExpr& expr);

// Utility function: breaks down an LValue to get at the Symbol, and reverses the vector of keys.
std::pair<Symbol, std::vector<std::string>> getFullName(const LValue& lvalue);

// Kill with LuauLValueAsKey.
std::string toString(const LValue& lvalue);

template<typename T>
const T* get(const LValue& lvalue)
{
    return get_if<T>(&lvalue);
}

using NEW_RefinementMap = std::unordered_map<LValue, TypeId, LValueHasher>;
using DEPRECATED_RefinementMap = std::map<std::string, TypeId>;

// Transient. Kill with LuauLValueAsKey.
struct RefinementMap
{
    NEW_RefinementMap NEW_refinements;
    DEPRECATED_RefinementMap DEPRECATED_refinements;
};

void merge(RefinementMap& l, const RefinementMap& r, std::function<TypeId(TypeId, TypeId)> f);
void addRefinement(RefinementMap& refis, const LValue& lvalue, TypeId ty);

} // namespace Luau
