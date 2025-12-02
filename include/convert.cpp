#include <iostream>
#include <python3.12/Python.h>
#include "convert.hpp"


float convertPLNtoUSD(float plnAmount) {
    float result = 0.0;

    PyObject* pName, *pModule, *pFunc;
    PyObject* pArgs, *pValue;

    Py_Initialize();

    // Add current directory to Python path so it finds currency.py
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");

    pName = PyUnicode_DecodeFSDefault("include.currency");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        pFunc = PyObject_GetAttrString(pModule, "changeCurrency");
        if (pFunc && PyCallable_Check(pFunc)) {
            // Build arguments tuple for Python function (single float argument)
            pArgs = PyTuple_New(1);
            pValue = PyFloat_FromDouble(plnAmount);
            PyTuple_SetItem(pArgs, 0, pValue);  // pValue reference stolen here

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != nullptr) {
                result = (float)PyFloat_AsDouble(pValue);
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                std::cerr << "Call failed\n";
            }
        } else {
            if (PyErr_Occurred()) PyErr_Print();
            std::cerr << "Cannot find function 'convert_to_usd'\n";
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load 'currency' module\n";
    }

    Py_Finalize();
    return result;
}