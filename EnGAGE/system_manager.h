#pragma once

#include "common.h"
#include "signature.h"
#include "system.h"

class SystemManager
{
	UnorderedMap<const char*, Signature>		mSignatures;
	UnorderedMap<const char*, Shared<System>>	mSystems;
public:
	SystemManager() :
		mSignatures(),
		mSystems() {}

	template<typename T>
	Shared<System> registerSystem()
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		auto ptr = makeShared<T>();
		mSystems.insert({ typeName, ptr });
		return ptr;
	}

	template<typename T>
	void setSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
		mSignatures.insert({ typeName, signature });
	}


	void entityDestroyed(Entity e)
	{
		for (const auto& pair : mSystems)
		{
			auto const& system = pair.second;
			system->mEntities.erase(e);
		}
	}

	void entitySignatureChanged(Entity e, Signature entitySignature)
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