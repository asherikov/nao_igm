OBJ_DIR = ./obj
LIB_DIR = ../lib
INCLUDE_DIRS = -I../include -I/usr/local/include/eigen2 -I/usr/include
CFLAGS = -Wall -O3 -pedantic -Wshadow -DNDEBUG
CXXFLAGS = -Wall -O3 -DNDEBUG -DENABLE_ALL_IGM_VERSIONS
AR = ar


test: lib
	g++ $(CXXFLAGS) -o test_3_1.a test_igm_3_1.cpp $(INCLUDE_DIRS) -L$(LIB_DIR) -lnaoigm
	g++ $(CXXFLAGS) -o test_4.a test_igm_4.cpp $(INCLUDE_DIRS) -L$(LIB_DIR) -lnaoigm


lib:
	-mkdir obj
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
	g++ $(CXXFLAGS) -c igm_versions.cpp -o $(OBJ_DIR)/igm_versions.o $(INCLUDE_DIRS)

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
	$(OBJ_DIR)/igm_versions.o \
	$(OBJ_DIR)/igm.o 

leg2joints:
	-mkdir obj
	gcc $(CFLAGS) -c LLeg2JointsEE.c -o $(OBJ_DIR)/LLeg2JointsEE.o
	gcc $(CFLAGS) -c RLeg2JointsEE.c -o $(OBJ_DIR)/RLeg2JointsEE.o

	$(AR) -r $(LIB_DIR)/libleg2joints.a \
	$(OBJ_DIR)/LLeg2JointsEE.o \
	$(OBJ_DIR)/RLeg2JointsEE.o

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f *.a

# dummy targets
.PHONY: clean

