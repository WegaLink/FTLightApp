#include "Services/Basic/Modules/CmTypes.h"

import CmBasic;
import CmFTLight;

using namespace Cosmos;

#define TEST_CmString
#define TEST_CmDateTime
#define TEST_CmIFTL
#define TEST_CmStringFTL
#define TEST_CmMatrixFTL
#define TEST_CmValueFTL
#define TEST_CmValueINI

bool testCosmosModules()
{
	// info
	printf("======================================================");
	printf("\n Cosmos Module test: CmModuleTest");
	printf("\n======================================================\n");
	bool isSuccess = true;

	// folders, statistics
	printf("\nInitialize...");
	char WorkingDirectory[2048];
	_getcwd(WorkingDirectory, sizeof(WorkingDirectory));
	CmString PathFTL(WorkingDirectory);
	PathFTL += "\\..\\runtime\\IFTLght";
	CmString Memory;
	Memory.clearMemoryState();
	printf("\n %s\n", Memory.getMemoryState(true).getText());

	// test CmString
#ifdef TEST_CmString
	printf("\nCmString...");
	CmString* String = new CmString;
	isSuccess = String->testCmString();
	delete String;
	if (false == isSuccess)	return false;

	printf("\n %s\n", Memory.getMemoryState(true).getText());
#endif

	// test CmDateTime
#ifdef TEST_CmDateTime
	printf("\nCmDateTime...");
	CmDateTime* DateTime = new CmDateTime;
	isSuccess = DateTime->testCmTime();
	delete DateTime;
	if (false == isSuccess)	return false;

	printf("\n %s\n", Memory.getMemoryState(true).getText());
#endif

	// test CmIFTL
#ifdef TEST_CmIFTL
	printf("\nCmIFTL...");
	CmIFTL* IFTL = new CmIFTL;
	isSuccess = IFTL->testCmIFTL();
	delete IFTL;
	if (false == isSuccess)	return false;

	printf("\n %s\n", Memory.getMemoryState(true).getText());
#endif

	// test CmStringFTL
#ifdef TEST_CmStringFTL
	printf("\nCmStringFTL...");
	CmStringFTL* StringFTL = new CmStringFTL;
	isSuccess = StringFTL->testCmStringFTL();
	delete StringFTL;
	if (false == isSuccess)	return false;

	printf("\n %s\n", Memory.getMemoryState(true).getText());
#endif

	// test CmMatrixFTL
#ifdef TEST_CmMatrixFTL
	printf("\nCmMatrixFTL...");
	CmMatrixFTL* MatrixFTL = new CmMatrixFTL;
	isSuccess = MatrixFTL->testCmMatrixFTL();
	delete MatrixFTL;
	if (false == isSuccess)	return false;

	printf("\n %s\n", Memory.getMemoryState(true).getText());
#endif

	// test CmValueFTL
#ifdef TEST_CmValueFTL
	printf("\nCmValueFTL...");
	CmValueFTL* ValueFTL = new CmValueFTL;
	isSuccess = ValueFTL->testCmValueFTL();
	delete ValueFTL;
	if (false == isSuccess)	return false;

	printf("\n %s\n", Memory.getMemoryState(true).getText());
#endif

	// test CmValueINI
#ifdef TEST_CmValueINI
	printf("\nCmValueINI...");
	CmValueINI* ValueINI = new CmValueINI;
	isSuccess = ValueINI->testCmValueINI();
	delete ValueINI;
	if (false == isSuccess)	return false;

	printf("\n %s\n", Memory.getMemoryState(true).getText());
#endif

	return true;
}


int main()
{
	// run unit test
	if (testCosmosModules()) {
		printf("\n\nCmTest finished SUCCESSFULLY");
	}
	else {
		printf("\n\nCmTest FAILED!");
	}

	// wait for any key pressed
	printf("\n\npress any key to exit program...\n");
	char Input[256];
	scanf_s("%c", Input, 1);

}

