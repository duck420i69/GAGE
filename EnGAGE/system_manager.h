#pragma once

#include "signature.h"
#include "system.h"

class SystemManager
{
	std::unordered_map<const char*, Signature>					mSignatures;
	std::unordered_map<const char*, std::shared_ptr<System>>	mSystems;
public:
	SystemManager() noexcept  :
		mSignatures(),
		mSystems() {}

	template<typename T>
	std::shared_ptr<T> registerSystem() noexcept
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		std::shared_ptr<T> ptr;
		try
		{
			ptr = std::make_shared<T>();
		}
		catch (std::bad_alloc& e)
		{
			Logger::error("Exception thrown: {}", e.what());
		}
		mSystems.insert({ typeName, ptr });
		return ptr;
	}

	template<typename T>
	void setSignature(Signature signature) noexcept
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
		Logger::info("Signature ({}) updated: {}", typeName, signature.to_string());
		mSignatures.insert({ typeName, signature });
	}


	void entityDestroyed(Entity e) noexcept
	{
		for (const auto& pair : mSystems)
		{
			auto const& system = pair.second;
			system->mEntities.erase(e);
		}
	}

	void entitySignatureChanged(Entity e, Signature entitySignature) noexcept
	{
		for (const auto& pair : mSystems)
		{
			const auto& type = pair.first;
			const auto& system = pair.second;
			const auto& signature = mSignatures[type];

			if ((entitySignature & signature) == signature)
			{
				system->mEntities.insert(e);

			}
			else
			{
				system->mEntities.erase(e);
			}
		}
	}
};