#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

void printOptions() {
    std::cout << "What do you want to do?" << std::endl;
    std::cout << "1: Go to a website" << std::endl;
    std::cout << "2: Go to previous website in history" << std::endl;
    std::cout << "3: Go to next website in history" << std::endl;
    std::cout << "4: Create a new tab (not functional)" << std::endl;
    std::cout << "5: Switch tabs (not functional)" << std::endl;
    std::cout << "6: Go to current website" << std::endl;
    std::cout << "-";
}

void clear() {
    std::cout << "\033[2J\033[1;1H";
}

void render(std::string filename);

int main() {
    std::string history[50];
    int historySize = 50;
    int historyPos = 0;

    std::string filename;
    int option;

    while (true) {

        clear();

        printOptions();
        std::cin >> option;

        clear();

        switch (option) {
        case 1:
            std::cout << "Enter a website:" << std::endl;
            std::cin >> filename;
            clear();

            if (historyPos >= historySize) {
                historySize *= 2;
                history->resize(historySize * sizeof(std::string));
            }
            
            history[historyPos] = filename;
            historyPos++;

            for (int i = historyPos; i < historySize; i++) {
                history[i] = "";
            }

            render(filename);
            break;
        case 2:
            if (historyPos > 1) {
                clear();

                filename = history[historyPos - 2];
                historyPos--;

                render(filename);
            }
            break;
        case 3:
            if (history[historyPos] != "") {
                clear();

                filename = history[historyPos];
                historyPos++;

                render(filename);
            }
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            if (historyPos > 0) {
                clear();

                filename = history[historyPos - 1];

                render(filename);
            }
            break;
        }

        
    }
    return 0;
}

void render(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string html = buffer.str();

    std::string text[200];
    int index = 0;
    int insideTag = false;
    for (int i = 0; i < html.length(); i++) {
        if (html[i] == '<') {
            insideTag = true;

            if (html.substr(i, 4) == "<br>") {
                text[index] += '\0';
            }
        }
        else if (html[i] == '>') {
            insideTag = false;
        }
        else if (!insideTag) {
            if (html[i] != '\n' && html[i] != '\r' && html[i] != '\t') {
                text[index] += html[i];
            }
        }
        if (html[i] == '>' && !insideTag && !text[index].empty()) {
            index++;
        }
    }


    std::cout << filename << '\n' << '\n' << std::endl;
    for (int i = 0; i < index; i++) {
        std::string trimmedText = trim(text[i]);
        if (!trimmedText.empty()) {
            std::cout << trimmedText << std::endl;
        }
    }

    std::cout << "\n\nPress enter to continue" << std::endl;
    std::cin.get();
    std::cin.get();

}