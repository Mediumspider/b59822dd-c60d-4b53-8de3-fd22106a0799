// b59822dd-c60d-4b53-8de3-fd22106a0799.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//Start leaking
	std::thread leaker([]() 
	{
		//Leak around 1MB per second
		while (true)
		{
			char* pLeak = new char[100 << 10];
			Sleep(100);
		}
	});

	//Start requesting
	std::cout << "Please press a key" << std::endl;
	std::atomic<int> lastPress = 0;
	std::thread requester([](std::atomic<int>& lastPress)
	{
		//Request key presses
		while (true)
		{
			++lastPress;
			//First 40 seconds
			if (lastPress < 400)
			{
				//Once every 5 second
				if (lastPress % 50 == 0)
					std::cout << "Please press a key" << std::endl;
			}
			//Next 20 seconds
			else if (lastPress < 600)
			{
				//Once every second
				if (lastPress % 10 == 0)
					std::cout << "Press any key" << std::endl;
			}
			//Next 20 seconds
			else if (lastPress < 800)
			{
				//Twice a second
				if (lastPress % 5 == 0)
					std::cout << "Press a key!" << std::endl;
			}
			//Go crazy
			else if (lastPress < 1000)
			{
				std::cout << "PRESS A KEY!" << std::endl;
			}
			Sleep(100);
		}
	}, std::ref(lastPress));
	//Start accepting
	std::thread accepter([](std::atomic<int>& lastPress)
	{
		while (true)
		{
			_getch();
			lastPress = 0;
			std::cout << "Aww yiss!" << std::endl;
		}
	}, std::ref(lastPress));

	leaker.join();
	return 0;
}

