#include "stdafx.h"
#include "../compile_machine_data/MachineData.cpp"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace cmdTest
{
	[TestClass]
	public ref class UnitTest
	{
	private:
		TestContext^ testContextInstance;

	public: 
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
    #pragma endregion

    [TestMethod]
    void TestMethod1() {
      MachineData ^md = gcnew MachineData();
      Assert::AreEqual(md->record_exists(2, "Z65263"), true);
    };

    [TestMethod]
    void TestMethod2() {
      MachineData ^md = gcnew MachineData();
      Assert::AreEqual(md->record_exists(1, "Z65263"), false);
    };

    [TestMethod]
    void TestMethod3() {
      MachineData ^md = gcnew MachineData();
      Assert::AreEqual(md->get_machine_id("CMS1097"), 1);
      Assert::AreEqual(md->get_machine_id("CMS1283"), 2);
      Assert::AreEqual(md->get_machine_id("BHC750"), 11);
    };
	};
}
