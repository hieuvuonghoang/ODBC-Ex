#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h> // Required header

int main() {
    // Allocate a large buffer to hold the driver names
    char buffer[4096];
    WORD bufferMax = sizeof(buffer);
    WORD bufferOut = 0;

    // Retrieve the installed drivers
    if (SQLGetInstalledDrivers(buffer, bufferMax, &bufferOut)) {
        char* driverPtr = buffer;

        std::cout << "Installed ODBC Drivers:\n";
        std::cout << "-----------------------\n";

        // Loop through the double-null terminated buffer
        while (*driverPtr != '\0') {
            std::cout << "- " << driverPtr << std::endl;
            
            // Move to the next string (advance past the null terminator)
            driverPtr += strlen(driverPtr) + 1;
        }
    } else {
        std::cerr << "Failed to retrieve ODBC drivers." << std::endl;
    }

    return 0;
}
