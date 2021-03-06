#include "IrisLangLibrary.h"

#include <iostream>
using namespace std;

#pragma comment(lib, "IrisLangLibrary.lib")

int ExitCondition() {
	return 0;
}

void ShowFatalErrorMessage(char* pMessage) {
	cout << pMessage << endl;
}

int main(int argc, char* argv[]) {

	IrisInitializeStruct iisInit;
	iisInit.m_pfExitConditionFunction = ExitCondition;
	iisInit.m_pfFatalErrorMessageFunction = ShowFatalErrorMessage;

	if (!IR_Initialize(&iisInit)) {
		cout << "Error when initializing Iris!" << endl;
		return 0;
	}

	if (!IR_LoadScriptFromPath("script.ir")) {
		cout << "Error when loading script!" << endl;
		return 0;
	}

	if (!IR_Run()) {
		cout << "Error when running script!" << endl;
		return 0;
	}

	if (!IR_ShutDown()) {
		cout << "Error when Shutting down script!" << endl;
		return 0;
	}

	return 0;
}