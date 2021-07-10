#pragma once

#include "pch.h"

class ICoreEngine
{
public:
	virtual void init() {};
	virtual void update(Real delta) {};
	virtual void render() {};
	virtual void shutdown() {};
};

class CoreEngine
{
	Shared<ICoreEngine> mCoreEngine;
public:
	CoreEngine(SharedRef<ICoreEngine> coreEngine, U16 width, U16 height, StringRef title);
	~CoreEngine();

	void run();
};