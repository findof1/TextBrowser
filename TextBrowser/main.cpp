#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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
    std::cout << "4: Create a new tab" << std::endl;
    std::cout << "5: Switch tabs" << std::endl;
    std::cout << "6: Go to current website" << std::endl;
    std::cout << "-";
}

void clear() {
    std::cout << "\033[2J\033[1;1H";
}

void render(std::string filename);

int main() {
    const int maxTabs = 10;
    const int initialHistorySize = 50;
    std::vector<std::vector<std::string>> history(maxTabs, std::vector<std::string>(initialHistorySize));
    std::vector<int> historyPos(maxTabs, 0);
    int tab = 0;
    int totalTabs = 1;

    std::string filename;
    int option;

    while (true) {
        clear();
        printOptions();
        std::cin >> option;
        clear();

        switch (option) {
        case 1: {
            std::cout << "Enter a website:" << std::endl;
            std::cin >> filename;
            clear();

            if (historyPos[tab] >= history[tab].size()) {
                history[tab].resize(history[tab].size() * 2);
            }

            history[tab][historyPos[tab]++] = filename;
            for (int i = historyPos[tab]; i < history[tab].size(); ++i) {
                history[tab][i].clear();
            }

            render(filename);
            break;
        }
        case 2: {
            if (historyPos[tab] > 1 && !history[tab][historyPos[tab] - 2].empty()) {
                clear();
                filename = history[tab][historyPos[tab] - 2];
                historyPos[tab]--;
                render(filename);
            }
            break;
        }
        case 3: {
            if (!history[tab][historyPos[tab]].empty()) {
                clear();
                filename = history[tab][historyPos[tab]];
                historyPos[tab]++;
                render(filename);
            }
            break;
        }
        case 4: {
            if (totalTabs < maxTabs) {
                totalTabs++;
                tab = totalTabs - 1;
            }
            break;
        }
        case 5: {
            for (int i = 0; i < totalTabs; ++i) {
                if (historyPos[i] > 0) {
                    std::cout << i << ": " << history[i][historyPos[i] - 1] << std::endl;
                }
                else {
                    std::cout << i << ": New Tab" << std::endl;
                }
            }
            std::cin >> tab;
            break;
        }
        case 6: {
            if (historyPos[tab] > 0) {
                clear();
                filename = history[tab][historyPos[tab] - 1];
                render(filename);
            }
            break;
        }
        default: {
            if (historyPos[tab] > 0) {
                clear();
                filename = history[tab][historyPos[tab] - 1];
                render(filename);
            }
            break;
        }
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