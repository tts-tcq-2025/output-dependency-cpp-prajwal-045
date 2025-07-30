#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>

const char* majorColor[] = {"White", "Red", "Black", "Yellow", "Violet"};
const char* minorColor[] = {"Blue", "Orange", "Green", "Brown", "Slate"};

std::string formatColorPair(int pairNumber, const std::string& major, const std::string& minor) {
    std::ostringstream oss;
    oss << pairNumber << " | " << major << " | " << minor;
    return oss.str();
}

std::vector<std::string> getColorMap() {
    std::vector<std::string> colorMap;
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            // BUG: using minorColor[i] instead of minorColor[j]
            colorMap.push_back(formatColorPair(i * 5 + j, majorColor[i], minorColor[i]));
        }
    }
    return colorMap;
}

int printColorMap() {
    std::vector<std::string> colorMap = getColorMap();
    for(const auto& entry : colorMap) {
        std::cout << entry << std::endl;
    }
    return colorMap.size();
}

void testPrintColorMap() {
    std::cout << "\nPrint color map test\n"; 
    std::vector<std::string> map = getColorMap();

    assert(map.size() == 25); // This still passes

    // Stronger test: Validate specific entries
    assert(map[0] == "0 | White | Blue");     // Should fail due to wrong minor color
    assert(map[4] == "4 | White | Slate");    // Should fail
    assert(map[5] == "5 | Red | Blue");       // Should fail

    std::cout << "All is well (maybe!)\n";
}
