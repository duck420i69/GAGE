#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <queue>
#include <unordered_map>

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

using Real = float;
using Double = double;
using String = std::string;
using StringRef = const std::string&;


//Dynamic arrray
template<typename T>
using DynArr = std::vector<T>;

template<typename T>
using DynArrRef = const DynArr<T>&;


//Static array
template<typename T, size_t size>
using Arr = std::array<T, size>;

//Shared ptr

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using SharedRef = const Shared<T>&;

template<typename T, typename... Args>
Shared<T> makeShared(Args&&... args)
{
	return std::make_shared<T>(std::forward(args)...);
}

template< class T, class U >
Shared<T> sharedStaticCast(SharedRef<U> r) noexcept
{
	auto p = static_cast<typename Shared<T>::element_type*>(r.get());
	return Shared<T>{r, p};
}


//Unique ptr

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename... Args>
Unique<T> makeUnique(Args&&... args)
{
	return std::make_unique<T>(std::forward(args)...);
}

//Queue

template<typename T>
using Queue = std::queue<T>;

//Unordered map

template<typename K, typename T>
using UnorderedMap = std::unordered_map<K, T>;
