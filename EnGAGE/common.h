#pragma once

#include <cstdint>
#include <string>
#include <memory>

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

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using SharedRef = const std::shared_ptr<T>&;

template<typename T, typename... Args>
Shared<T> makeShared(Args&&... args)
{
	return std::make_shared<T>(std::forward(args)...);
}
