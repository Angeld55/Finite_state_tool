#include "stdafx.h"
#include "CppUnitTest.h"
#include  "../main.cpp"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AFL_tests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			CommandDispatcher dispatcher;
			std::string input = "fsa angel";
			std::string output = dispatcher.dispatch(input);

			Assert::AreEqual(output, output);

			// TODO: Your test code here
		}

	};
}