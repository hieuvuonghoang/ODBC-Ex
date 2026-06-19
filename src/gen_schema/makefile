CXX = g++
CXXFLAGS = -Wall -O2 -g -I../../libs

LIBS = -lodbc32 -lodbccp32

TARGET = gen_schema
SRC = main.cpp

# TARGET = json_ex
# SRC = json_ex.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	del /Q $(TARGET).exe