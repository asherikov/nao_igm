OBJ_DIR = ./obj
LIB_DIR = ../lib
INCLUDE_DIRS = -I../include -I/usr/local/include/eigen2 -I/usr/include
CFLAGS = -Wall -O3 -pedantic -Wshadow -DNDEBUG
CXXFLAGS = -Wall -O3 -DNDEBUG
AR = ar


test: 
	g++ $(CFLAGS) -o a test_igm_3.cpp $(INCLUDE_DIRS) -L$(LIB_DIR) -lnaoigm


lib:
	gcc $(CFLAGS) -c from_LLeg_1.c -o $(OBJ_DIR)/from_LLeg_1.o
	gcc $(CFLAGS) -c from_RLeg_1.c -o $(OBJ_DIR)/from_RLeg_1.o
	gcc $(CFLAGS) -c from_LLeg_2.c -o $(OBJ_DIR)/from_LLeg_2.o
	gcc $(CFLAGS) -c from_RLeg_2.c -o $(OBJ_DIR)/from_RLeg_2.o
	gcc $(CFLAGS) -c from_LLeg_3.c -o $(OBJ_DIR)/from_LLeg_3.o
	gcc $(CFLAGS) -c from_RLeg_3.c -o $(OBJ_DIR)/from_RLeg_3.o
	gcc $(CFLAGS) -c from_LLeg_4.c -o $(OBJ_DIR)/from_LLeg_4.o
	gcc $(CFLAGS) -c from_RLeg_4.c -o $(OBJ_DIR)/from_RLeg_4.o

	gcc $(CFLAGS) -c JacCoM.c -o $(OBJ_DIR)/JacCoM.o
	gcc $(CFLAGS) -c JacCoM_a.c -o $(OBJ_DIR)/JacCoM_a.o

	gcc $(CFLAGS) -c Leg2EE.c -o $(OBJ_DIR)/Leg2EE.o

	g++ $(CXXFLAGS) -c igm.cpp -o $(OBJ_DIR)/igm.o $(INCLUDE_DIRS)

	$(AR) -r $(LIB_DIR)/libnaoigm.a \
	$(OBJ_DIR)/from_LLeg_1.o \
	$(OBJ_DIR)/from_RLeg_1.o \
	$(OBJ_DIR)/from_LLeg_2.o \
	$(OBJ_DIR)/from_RLeg_2.o \
	$(OBJ_DIR)/from_LLeg_3.o \
	$(OBJ_DIR)/from_RLeg_3.o \
	$(OBJ_DIR)/from_LLeg_4.o \
	$(OBJ_DIR)/from_RLeg_4.o \
	$(OBJ_DIR)/JacCoM.o \
	$(OBJ_DIR)/JacCoM_a.o \
	$(OBJ_DIR)/Leg2EE.o \
	$(OBJ_DIR)/igm.o 

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f a

# dummy targets
.PHONY: clean

