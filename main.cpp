#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <python3.12/Python.h>
#include <stdlib.h>
#include "convert.hpp"

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
    }else{
        currency = "PLN";
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
