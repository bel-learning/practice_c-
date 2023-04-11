#include <iostream>
#include <fstream>

int main() {
    std::ifstream file("output-shorts.bin", std::ios::binary);
    if (file.is_open()) {
        // get length of file:
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        // allocate memory:
        char* buffer = new char[length];

        // read data as a block:
        file.read(buffer, length);

        // do something with the data...
        std::cout << "hello" << std::endl;
        std::cout << buffer << std::endl;
        // cleanup:
        delete[] buffer;
        file.close();
    }
    else {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    return 0;
}
