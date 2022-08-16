#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


// variables
string specificItem, menuSelection;
bool menuCheck = true;
int  tableNum, doubleNum;
fstream ifile;
string str;
string fileLine;


/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char *procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();	
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen: 
		You sent me: Test
Return:
	100 is returned to the C++	
*/
int callIntFunc(string proc, string param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char *paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

string menuDisplay() {

	cout << "1: Produce a list of all items purchased in a given day along with the number of times each item was purchased. \n";
	cout << "2: Produce a number representing how many times a specific item was purchased in a given day. \n";
	cout << "3: Produce a text-based histogram listing all items purchased in a given day, along with a representation of the number of "
	"times each item was purchased. \n";
	cout << "4: Exit \n";
	cout << "Enter your selection as a number 1, 2, 3, or 4. \n";
	cin >> menuSelection;
	return menuSelection;

}

int main()
{
	
	while (menuCheck == true) {
		menuDisplay();

		if (menuSelection == "1") {
			cout << "Here is your frequency list: \n";
			CallProcedure("printFrequency");

		}
		else if (menuSelection == "2") {
			cout << "Please enter a specific item: \n";
			cin >> specificItem;
			callIntFunc("checkList", specificItem);
			cout << "There were " << callIntFunc("findItem", specificItem) << " " << specificItem << " sold." << endl;
			
		}
		else if (menuSelection == "3") {

			// First write the appropriate counts from the input file to the output file
			cout << "\n\nWriting items and quantities sold today to a file: \n\n";
			CallProcedure("writeToFile");
			cout << "\n\n";
			// Open the output file and read the data
			ifstream frequencyFile("frequency.dat");
			string item;
			int count;
			// Parse through the file line by line and print the histogram
			while (getline(frequencyFile, fileLine))
			{
				istringstream iss(fileLine);
				if (!(iss >> item >> count)) { break; } // error
				// Print the item name and "count" number of asterisks
				cout << item << " " << string(count, '*') << endl;
			}
			cout << "\n\n";	
		}
		else if (menuSelection == "4") {
			cout << "You have exited the program.";
			CallProcedure("strip");
			menuCheck = false;
		}
		else {
			cout << "I'm sorry, but that is not a valid input.\n";
			menuCheck = true;
		} 
	} 
	return 0;
} 