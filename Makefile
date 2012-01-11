NAO_DIR = ./nao_model
OBJ_DIR = obj
LIB_DIR = ./lib
DOC_DIR = ./doc


ifdef DEBUG
CMAKEFLAGS+=-DCMAKE_BUILD_TYPE=DEBUG 
else
CMAKEFLAGS+=-DCMAKE_BUILD_TYPE=Release 
endif

ifdef LEG2JOINTS_ENABLE
CMAKEFLAGS+=-DLEG2JOINTS_ENABLE=ON 
else
CMAKEFLAGS+=-DLEG2JOINTS_ENABLE=OFF 
endif


all:
	cd $(NAO_DIR); $(MAKE) lib; $(MAKE)


cmake: 
	-mkdir build;
ifdef TOOLCHAIN
	cd build; cmake -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN} ${CMAKEFLAGS} ..;
else
	cd build; cmake ${CMAKEFLAGS} ..;
endif
	cd build; ${MAKE}


clean:
	cd $(NAO_DIR); ${MAKE} clean;
	rm -f $(LIB_DIR)/*.a
	rm -f a
	rm -rf $(DOC_DIR)
	rm -rf build

# dummy targets
.PHONY: clean

