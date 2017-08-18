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
	std::cout << "Start Test" << std::endl;

	int a = 100;
	int b = 200;
	std::string c = "Hi";
	std::string d = "wat";

	smartsignals::Signal<void(int, int&, const std::string&, std::string)> signal;

	signal.connect(testfunc);

	signal.call(a, b, c, d);
	signal(a, b, c, d);

	std::cout << "Start Test2" << std::endl;

	std::string teststring2 = "TestString2 ";

	TestEmitter emitter;

	TestUser user(emitter);

	emitter.signal.call(teststring2, "Green");

	user.connections.disconnectAll();

	emitter.signal.call(teststring2, "Red");

	system("pause");

	return 0;
}
