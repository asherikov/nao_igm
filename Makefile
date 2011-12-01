NAO_DIR = ./nao_model
OBJ_DIR = obj
LIB_DIR = ./lib
DOC_DIR = ./doc


all:
	cd $(NAO_DIR); $(MAKE) lib; $(MAKE)


cmake: 
	-mkdir build;
ifdef TOOLCHAIN
	cd build; cmake -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN} ..;
else
	cd build; cmake ..;
endif
	cd build; ${MAKE}


clean:
	rm -f $(NAO_DIR)/$(OBJ_DIR)/*.o
	rm -f $(NAO_DIR)/a
	rm -f $(LIB_DIR)/*.a
	rm -f a
	rm -rf $(DOC_DIR)
	rm -rf build

# dummy targets
.PHONY: clean

