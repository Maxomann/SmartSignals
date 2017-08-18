#pragma once
#include <iostream>
#include <string>

#include "SmartSignals.h"

class TestEmitter
{
public:

	smartsignals::Signal<void(std::string& strRef, std::string str)> signal;
};

class TestUser
{
	void memberFunction(std::string& strRef, std::string str)
	{
		strRef += "memberFunction";
		std::cout<< strRef << str << std::endl;
	};

public:
	TestUser(TestEmitter& emitter)
	{
		emitter.signal.connect(connections, this, &TestUser::memberFunction);
	};

	smartsignals::Connections connections;
};
