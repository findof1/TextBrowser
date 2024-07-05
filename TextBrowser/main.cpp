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

void render(std::string filename);

int main() {
    std::string filename;
    while (true) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Enter a website:" << std::endl;
        std::cin >> filename;
        std::cout << "\033[2J\033[1;1H";
        render(filename);
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

    std::cout << "\n\nPress enter to search" << std::endl;
    std::cin.get();
    std::cin.get();

}