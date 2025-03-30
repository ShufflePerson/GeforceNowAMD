#include "utils.h"

std::string utils::uintptr_to_hex_string(uintptr_t ptr) {
    std::stringstream stream;
    stream << std::hex << std::uppercase << std::setw(2 * sizeof(uintptr_t)) << std::setfill('0') << ptr;
    return stream.str();
}

std::string utils::get_true_bits(uint32_t value)
{
    std::stringstream ss;
    std::bitset<32> binaryValue(value);
    ss << "// Binary representation of input: " << binaryValue << std::endl;

    for (int i = 0; i < 32; ++i) {
        if ((value >> i) & 1) { 
            ss << "(1u << " << i << ")" << std::endl;
        }
    }
    return ss.str();
}

void utils::alloc_console()
{
    AllocConsole();
    FILE* dummy_stdout;
    FILE* dummy_stderr;
    freopen_s(&dummy_stdout, "NUL", "w", stdout); 
    freopen_s(&dummy_stderr, "NUL", "w", stderr); 
    HWND consoleWindow = GetConsoleWindow();
}

void utils::log(std::string str) {
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsoleOutput == INVALID_HANDLE_VALUE) {
        FreeConsole();
        return;
    }

    const char* message = str.c_str(); 
    DWORD charsWritten;

    BOOL success = WriteConsoleA( 
        hConsoleOutput,
        message,  
        strlen(message),
        &charsWritten,
        NULL
    );

    if (!success) {
        FreeConsole();
        return;
    }

}


std::string utils::memory_to_hex_str(void* memory, size_t amount) {
    if (memory == nullptr || amount == 0) {
        return ""; 
    }

    std::stringstream hexStream;
    unsigned char* bytePtr = static_cast<unsigned char*>(memory); 

    hexStream << std::hex << std::uppercase; 

    for (size_t i = 0; i < amount; ++i) {
        unsigned char byteValue = bytePtr[i];

        hexStream << std::setw(2) << std::setfill('0') << static_cast<int>(byteValue);

        if (i < amount - 1) {
            hexStream << " ";
        }
    }

    return hexStream.str(); 
}


std::string utils::memory_to_uint16_str(void* memory, size_t amount) {
    if (memory == nullptr || amount == 0) {
        return ""; // Handle null pointer or zero amount
    }

    std::stringstream ss;
    unsigned char* byte_ptr = static_cast<unsigned char*>(memory); // Treat memory as bytes

    // Iterate through the memory in chunks of 2 bytes
    for (size_t i = 0; i < amount; i += 2) {
        if (i + 1 >= amount) {
            // Handle case where there's an odd number of bytes.
            // You might want to decide how to handle this.
            // Options:
            // 1. Ignore the last byte (as done here - breaks out of loop)
            // 2. Treat the last byte as the lower byte of a uint16 and pad with 0 as the higher byte.
            // 3. Throw an error/exception.
            // For this example, we'll ignore the last byte if amount is odd.
            break; // Stop processing if we don't have a pair
        }

        // Combine two bytes into a uint16 (assuming little-endian byte order)
        uint16_t value = 0;
        value |= (static_cast<uint16_t>(byte_ptr[i]) & 0xFF); // Lower byte
        value |= (static_cast<uint16_t>(byte_ptr[i + 1]) & 0xFF) << 8; // Higher byte

        ss << value;

        if (i + 2 < amount) {
            ss << " "; // Add space separator if it's not the last uint16
        }
    }

    return ss.str();
}

std::string utils::get_error_code(HRESULT hr) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(8) << std::hex << std::uppercase << hr;
    return stream.str();
}