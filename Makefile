# Copyright (c) 2014, Hasnain Lakhani.
# All rights reserved.
# Please see LICENSE for details.

CXX=clang++
CFLAGS=-Wall -Wextra -Wpedantic -g -std=c++11 -I$(INCLUDE_DIR) -fPIC -O3
LDFLAGS=
BLOCKYTESTLDFLAGS=-L$(BIN_DIR) -lblocky
BLOCKYBENCHLDFLAGS=-L$(BIN_DIR) -lblocky
LIBLDFLAGS=-shared

_LIBDEPS=gf28 utils blockypacket coder blockycoder blockycodermemory blockycoderfile blockycodermmap
_LIBOBJ=gf28 utils coder blockycoder blockycoderfile blockycodermemory blockycodermmap
_BLOCKYTESTDEPS=
_BLOCKYTESTOBJ=blockytest
_BLOCKYBENCHDEPS=
_BLOCKYBENCHOBJ=blockybench

BLOCKYTESTEXECUTABLE=blockytest
BLOCKYBENCHEXECUTABLE=blockybench
LIBRARY=libblocky.so

INCLUDE_DIR=include
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
DOC_DIR=doc

LIBDEPS=$(patsubst %,$(INCLUDE_DIR)/%.h,$(_LIBDEPS))
LIBOBJ=$(patsubst %,$(OBJ_DIR)/%.o,$(_LIBOBJ)) 
BLOCKYTESTDEPS=$(patsubst %,$(INCLUDE_DIR)/%.h,$(_BLOCKYTESTDEPS))
BLOCKYTESTOBJ=$(patsubst %,$(OBJ_DIR)/%.o,$(_BLOCKYTESTOBJ)) 
BLOCKYBENCHDEPS=$(patsubst %,$(INCLUDE_DIR)/%.h,$(_BLOCKYBENCHDEPS))
BLOCKYBENCHOBJ=$(patsubst %,$(OBJ_DIR)/%.o,$(_BLOCKYBENCHOBJ)) 

.PHONY: all
all: dirs library blockytest blockybench

.PHONY: blockytest
blockytest: $(BLOCKYTESTOBJ)
	$(CXX) -o $(BIN_DIR)/$(BLOCKYTESTEXECUTABLE) $^ $(LDFLAGS) $(BLOCKYTESTLDFLAGS)

.PHONY: blockybench
blockybench: $(BLOCKYBENCHOBJ)
	$(CXX) -o $(BIN_DIR)/$(BLOCKYBENCHEXECUTABLE) $^ $(LDFLAGS) $(BLOCKYBENCHLDFLAGS)

.PHONY: library
library: $(LIBOBJ)
	$(CXX) -o $(BIN_DIR)/$(LIBRARY) $^ $(LDFLAGS) $(LIBLDFLAGS)

.PHONY: doc
doc: $(LIBDEPS) $(LIBOBJ)
	doxygen

.PHONY: clean
clean:
	rm -rf $(DOC_DIR)
	rm -f -d $(OBJ_DIR)/*.o $(OBJ_DIR) $(BIN_DIR)/$(LIBRARY) $(BIN_DIR)/$(BLOCKYTESTEXECUTABLE) $(BIN_DIR)/$(BLOCKYBENCHEXECUTABLE) $(BIN_DIR)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(LIBDEPS) $(BLOCKYTESTDEPS) $(BLOCKYBENCHDEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)
