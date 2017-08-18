#include <iostream>
#include <string>
#include "SmartSignals.h"
#include "test.h"

void testfunc(int a, int& b, const std::string& c, std::string d)
{
	std::cout << a << ":" << b << ":" << c << ":" << d << std::endl;
}

int main()
{
	////
	{
		std::cout << "Start Test" << std::endl;

		int a = 100;
		int b = 200;
		std::string c = "Hi";
		std::string d = "wat";

		smartsignals::Signal<void(int, int&, const std::string&, std::string)> signal;

		signal.connect(testfunc);

		signal.call(a, b, c, d);
		signal(a, b, c, d);
	}

	////
	{
		std::cout << "Start Test2" << std::endl;

		std::string teststring2 = "TestString2 ";

		TestEmitter emitter;

		TestUser user(emitter);

		emitter.signal.call(teststring2, "Green");

		user.connections.disconnectAll();

		emitter.signal.call(teststring2, "Red");
	}

	////
	{
		std::cout << "Start Test3" << std::endl;

		smartsignals::Signal<int(int)> signal;

		signal.connect([](int val) { std::cout << 1 << ":"; return val + 1; });
		signal.connect([](int val) { std::cout << 2 << ":"; return val + 2; });
		signal.connect([](int val) { std::cout << 3 << ":"; return val + 3; });
		signal.connect([](int val) { std::cout << 4 << std::endl; return val + 4; });

		std::cout << "Call1" << std::endl;

		signal.call(10);

		std::cout << "Call2" << std::endl;

		std::vector<int> vec;

		signal.call( [&](auto el) {vec.push_back(el); }, 10);

	}

	system("pause");

	return 0;
}
