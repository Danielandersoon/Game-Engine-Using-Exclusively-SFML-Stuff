CXX = g++
LDFLAGS = -L./SFML/x64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

TARGET = game_engine
SRCS = Source.cpp ResourceManager.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)
