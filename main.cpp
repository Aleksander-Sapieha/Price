#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <python3.12/Python.h>
#include <stdlib.h>

using json = nlohmann::json;

std::string currency = "PLN";
float multiplier = 1.00;

void setNonBlocking(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }
}

float convertPLNtoUSD(float plnAmount) {
    float result = 0.0;

    PyObject* pName, *pModule, *pFunc;
    PyObject* pArgs, *pValue;

    Py_Initialize();

    // Add current directory to Python path so it finds currency_converter.py
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
        std::cerr << "Failed to load 'currency_converter' module\n";
    }

    Py_Finalize();
    return result;
}

// Display the menu
void displayMenu() {
    std::cout << "1) Display Gold Price\n";
    std::cout << "2) Track Gold Price\n";
    std::cout << "3) Quit\n";
    std::cout << "> ";
}

float getGoldPrice() {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.nbp.pl/api/cenyzlota"});

    if (r.status_code == 200) {
        json j = json::parse(r.text);

        if (!j.empty() && j.is_array()) {
            double gold_price = j[0]["cena"];
            return gold_price;
        } else {
            std::cerr << "Unexpected JSON format.\n";
        }
    } else {
        std::cerr << "Failed to get data. HTTP status: " << r.status_code << "\n";
    }
    return 0.00;
}

int main() {

    std::string curChoice;
    std::cout << "1. USD\n";
    std::cout << "2. PLN\n";
    std::cout << "> ";
    std::cin >> curChoice;
    if (curChoice == "1" || curChoice == "USD"){
        currency = "USD";
        multiplier = convertPLNtoUSD(multiplier);

    }

    while (true){
        std::string choice;
        displayMenu();
        std::cin >> choice;

        if (choice == "1") {
            std::cout << std::flush;
            system("clear");
            std::cout << "Gold price: " << getGoldPrice() * multiplier << " " << currency << "\n";
  
        } else if (choice == "2") {
            setNonBlocking(true);
            std::cout << "PRESS Q TO STOP\n";
            while (true) {
                std::cout << "Gold price: " << getGoldPrice() * multiplier << " " << currency << "\n";
                char c;
                ssize_t n = read(STDIN_FILENO, &c, 1);
                if (n > 0) {
                    if (c == 'q') {
                        std::cout << std::flush;
                        system("clear");
                        break;
                    } else {
                        continue;
                    }
                }
                usleep(100000);  // Sleep 0.1 seconds
            }
            setNonBlocking(false);
        } else if (choice == "3") {
            std::cout << "Quitting...\n";
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }
    return 0;
}
