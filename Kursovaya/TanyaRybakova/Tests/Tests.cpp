#include "CppUnitTest.h"
#include <stdexcept>
#include "../TanyaRybakova/Flow.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:

		TEST_METHOD(right_open_file)
		{
			try
			{
				Flow testFlow("EX1.txt");
			}
			catch (const exception& ex) {
				Assert::IsTrue(false);
			}
		}
		TEST_METHOD(get_max_flow)
		{
			Flow testFlow("ex1.txt");
			int excepted = 65;
			Assert::AreEqual(excepted, testFlow.getMaxFlow());
		}
		TEST_METHOD(negative_capacity)
		{
			try
			{
				Flow testFlow("NEGATIVE.txt");
			}
			catch (const exception& ex) {

				Assert::AreEqual(ex.what(), "Capacity can only be a positive integer");
			}
		}
		TEST_METHOD(duplicate_edge)
		{
			try
			{
				Flow testFlow("DUP.txt");
			}
			catch (const exception& ex) {

				Assert::AreEqual(ex.what(), "File contains two lines describing one edge");
			}
		}
		TEST_METHOD(invalid_text)
		{
			try
			{
				Flow testFlow("INVALID.txt");
			}
			catch (const exception& ex) {

				Assert::AreEqual(ex.what(), "invalid stoi argument");
			}
		}
		TEST_METHOD(no_sink_source)
		{
			try
			{
				Flow testFlow("NOSINK.txt");
			}
			catch (const exception& ex) {
				Assert::AreEqual(ex.what(), "File does not contain a source or sink (or both)");
			}
		}
	};
}
