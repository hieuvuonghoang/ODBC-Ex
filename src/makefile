CXX = g++
CXXFLAGS = -Wall -O2 -I".\libs\ODBC-Specification\Windows\inc"

LIBS = -lodbc32 -lodbccp32

TARGET = app
SRC = main.cpp

# TARGET = driver-list
# SRC = driver-list.cpp

# TARGET = get-installed-drives
# SRC = get-installed-drives.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	del /Q $(TARGET).exe