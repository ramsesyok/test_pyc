#include <Python.h>
#include <iostream>
#include <string>

int main()
{
    // Initialize the Python interpreter
    Py_Initialize();

    // Add the current directory to the Python path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    // Import the script_main module
    PyObject *pModule = PyImport_ImportModule("script_main");
    if (!pModule)
    {
        PyErr_Print();
        std::cerr << "Failed to load script_main module" << std::endl;
        Py_Finalize();
        return 1;
    }

    // Get the greeting function from the module
    PyObject *pFunc = PyObject_GetAttrString(pModule, "greeting");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        PyErr_Print();
        std::cerr << "Failed to load greeting function" << std::endl;
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // Import the IntroductionRequest class
    PyObject *pClassModule = PyImport_ImportModule("introduction_request");
    if (!pClassModule)
    {
        PyErr_Print();
        std::cerr << "Failed to load introduction_request module" << std::endl;
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    PyObject *pClass = PyObject_GetAttrString(pClassModule, "IntroductionRequest");
    if (!pClass || !PyCallable_Check(pClass))
    {
        PyErr_Print();
        std::cerr << "Failed to load IntroductionRequest class" << std::endl;
        Py_DECREF(pClassModule);
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // Create an instance of IntroductionRequest
    PyObject *pReqArgs = Py_BuildValue("(s,i,[s,s])", "John", 30, "coding", "reading");
    PyObject *pReq = PyObject_CallObject(pClass, pReqArgs);
    Py_DECREF(pReqArgs);
    Py_DECREF(pClassModule);

    if (!pReq)
    {
        PyErr_Print();
        std::cerr << "Failed to create IntroductionRequest instance" << std::endl;
        Py_DECREF(pClass);
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // Create arguments for the greeting function
    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, pReq); // pReq reference is stolen here

    // Call the greeting function
    PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
    if (pValue != nullptr)
    {
        // Call the respond method on the GreetingResponse object
        PyObject *pRespond = PyObject_GetAttrString(pValue, "respond");
        if (pRespond && PyCallable_Check(pRespond))
        {
            PyObject *pResponseValue = PyObject_CallObject(pRespond, nullptr);
            if (pResponseValue)
            {
                std::cout << "Greeting response: " << PyUnicode_AsUTF8(pResponseValue) << std::endl;
                Py_DECREF(pResponseValue);
            }
            else
            {
                PyErr_Print();
            }
            Py_DECREF(pRespond);
        }
        else
        {
            PyErr_Print();
        }
        Py_DECREF(pValue);
    }
    else
    {
        PyErr_Print();
    }

    // Clean up
    Py_DECREF(pArgs);
    Py_DECREF(pClass);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    // Finalize the Python interpreter
    Py_Finalize();

    return 0;
}