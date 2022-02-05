CXX = g++ -std=c++11
MAIN = utrip.out
BUILD_DIR = build
BUILD_DEP = build
SRC_DIR = utrip

all: $(BUILD_DIR) $(MAIN)
	@echo Compiled $(MAIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(MAIN): $(BUILD_DEP)/Estimated_weights.o $(BUILD_DEP)/Filter.o $(BUILD_DEP)/Hotel.o $(BUILD_DEP)/Handlers.o $(BUILD_DEP)/main.o $(BUILD_DEP)/Rating.o $(BUILD_DEP)/Reservation.o $(BUILD_DEP)/Reservation_date.o $(BUILD_DEP)/Rooms.o $(BUILD_DEP)/Sort.o $(BUILD_DEP)/User.o $(BUILD_DEP)/Utrip.o $(BUILD_DEP)/response.o $(BUILD_DEP)/request.o $(BUILD_DEP)/utilities.o $(BUILD_DEP)/server.o $(BUILD_DEP)/route.o
	$(CXX) $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/Estimated_weights.o $(BUILD_DIR)/Filter.o $(BUILD_DIR)/Hotel.o $(BUILD_DIR)/Handlers.o $(BUILD_DIR)/main.o $(BUILD_DIR)/Rating.o $(BUILD_DIR)/Reservation.o $(BUILD_DIR)/Reservation_date.o $(BUILD_DIR)/Rooms.o $(BUILD_DIR)/Sort.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Utrip.o -o $(MAIN)

$(BUILD_DEP)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CXX) -c utils/response.cpp -o $(BUILD_DIR)/response.o

$(BUILD_DEP)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CXX) -c utils/request.cpp -o $(BUILD_DIR)/request.o

$(BUILD_DEP)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CXX) -c utils/utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DEP)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CXX) -c server/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DEP)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CXX) -c server/route.cpp -o $(BUILD_DIR)/route.o

$(BUILD_DEP)/Estimated_weights.o: $(SRC_DIR)/Estimated_weights.cpp $(SRC_DIR)/Estimated_weights.h $(SRC_DIR)/Exceptions.h $(SRC_DIR)/Utrip.h $(SRC_DIR)/User.h $(SRC_DIR)/Hotel.h $(SRC_DIR)/Rating.h
	$(CXX) -c $(SRC_DIR)/Estimated_weights.cpp -o $(BUILD_DIR)/Estimated_weights.o

$(BUILD_DEP)/Filter.o: $(SRC_DIR)/Filter.h $(SRC_DIR)/Filter.cpp $(SRC_DIR)/Hotel.h $(SRC_DIR)/Exceptions.h
	$(CXX) -c $(SRC_DIR)/Filter.cpp -o $(BUILD_DIR)/Filter.o

$(BUILD_DEP)/Hotel.o: $(SRC_DIR)/Hotel.h $(SRC_DIR)/Hotel.cpp $(SRC_DIR)/Exceptions.h $(SRC_DIR)/Reservation_date.h $(SRC_DIR)/Rooms.h $(SRC_DIR)/Rating.h
	$(CXX) -c $(SRC_DIR)/Hotel.cpp -o $(BUILD_DIR)/Hotel.o

$(BUILD_DEP)/Handlers.o: $(SRC_DIR)/Handlers.cpp $(SRC_DIR)/Handlers.h $(SRC_DIR)/Exceptions.h $(SRC_DIR)/Utrip.h $(BUILD_DEP)/server.o
	$(CXX) -c $(SRC_DIR)/Handlers.cpp -o $(BUILD_DIR)/Handlers.o

$(BUILD_DEP)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/Utrip.h $(SRC_DIR)/Handlers.h
	$(CXX) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DEP)/Rating.o: $(SRC_DIR)/Rating.h $(SRC_DIR)/Rating.cpp $(SRC_DIR)/User.h $(SRC_DIR)/Exceptions.h 
	$(CXX) -c $(SRC_DIR)/Rating.cpp -o $(BUILD_DIR)/Rating.o

$(BUILD_DEP)/Reservation_date.o: $(SRC_DIR)/Reservation_date.h $(SRC_DIR)/Reservation_date.cpp $(SRC_DIR)/Exceptions.h 
	$(CXX) -c $(SRC_DIR)/Reservation_date.cpp -o $(BUILD_DIR)/Reservation_date.o

$(BUILD_DEP)/Reservation.o: $(SRC_DIR)/Reservation.h $(SRC_DIR)/Reservation.cpp $(SRC_DIR)/User.h $(SRC_DIR)/Hotel.h $(SRC_DIR)/Reservation_date.h
	$(CXX) -c $(SRC_DIR)/Reservation.cpp -o $(BUILD_DIR)/Reservation.o

$(BUILD_DEP)/Rooms.o: $(SRC_DIR)/Rooms.h $(SRC_DIR)/Rooms.cpp $(SRC_DIR)/Utrip.h $(SRC_DIR)/Reservation.h 
	$(CXX) -c $(SRC_DIR)/Rooms.cpp -o $(BUILD_DIR)/Rooms.o

$(BUILD_DEP)/Sort.o: $(SRC_DIR)/Sort.h $(SRC_DIR)/Sort.cpp $(SRC_DIR)/Hotel.h 
	$(CXX) -c $(SRC_DIR)/Sort.cpp -o $(BUILD_DIR)/Sort.o

$(BUILD_DEP)/User.o: $(SRC_DIR)/User.h $(SRC_DIR)/User.cpp $(SRC_DIR)/Reservation.h $(SRC_DIR)/Hotel.h 
	$(CXX) -c $(SRC_DIR)/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DEP)/Utrip.o: $(SRC_DIR)/Utrip.h $(SRC_DIR)/Utrip.cpp $(SRC_DIR)/Exceptions.h $(SRC_DIR)/Reservation.h $(SRC_DIR)/Handlers.h $(SRC_DIR)/Hotel.h $(SRC_DIR)/User.h $(SRC_DIR)/Filter.h $(SRC_DIR)/Sort.h $(SRC_DIR)/Estimated_weights.h 
	$(CXX) -c $(SRC_DIR)/Utrip.cpp -o $(BUILD_DIR)/Utrip.o

clean:
	rm -rf $(BUILD_DIR) *.o *.out &> /dev/null