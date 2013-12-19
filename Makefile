##
# BEGIN_COPYRIGHT
#
# This file is part of SciDB.
# Copyright (C) 2008-2013 SciDB, Inc.
#
# SciDB is free software: you can redistribute it and/or modify
# it under the terms of the AFFERO GNU General Public License as published by
# the Free Software Foundation.
#
# SciDB is distributed "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
# INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
# NON-INFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE. See
# the AFFERO GNU General Public License for the complete license terms.
#
# You should have received a copy of the AFFERO GNU General Public License
# along with SciDB.  If not, see <http://www.gnu.org/licenses/agpl-3.0.html>
#
# END_COPYRIGHT
##

#The scidb-boost-devel package also needs to be installed:
BOOST_LOCATION=/opt/scidb/13.9/include
SCIDB_SOURCE_DIR=/home/scidb/scidbtrunk

CFLAGS=-pedantic -W -Wextra -Wall -Wno-strict-aliasing -Wno-long-long -Wno-unused-parameter -fPIC -D__STDC_FORMAT_MACROS -Wno-system-headers -isystem -O2 -g -DNDEBUG -ggdb3  -D__STDC_LIMIT_MACROS

INC=-I. -DPROJECT_ROOT="\"$(SCIDB_SOURCE_DIR)\"" -I"$(SCIDB_SOURCE_DIR)/include" -I"$(BOOST_LOCATION)"
LIBS=-L"$(SCIDB_SOURCE_DIR)/lib" -shared -Wl,-soname,libexample_udos.so -L. -lm

all:
	@if test ! -d "$(SCIDB_SOURCE_DIR)"; then echo  "Error. Try:\n\nmake SCIDB_SOURCE_DIR=<PATH TO SCIDB TRUNK>"; exit 1; fi 
	$(CXX) $(CFLAGS) $(INC) -o plugin.cpp.o -c plugin.cpp

	$(CXX) $(CFLAGS) $(INC) -o LogicalEnviWrite.cpp.o -c EnviWrite/LogicalEnviWrite.cpp
	$(CXX) $(CFLAGS) $(INC) -o PhysicalEnviWrite.cpp.o -c EnviWrite/PhysicalEnviWrite.cpp

	$(CXX) $(CFLAGS) $(INC) -o libenviwrite.so \
	                           plugin.cpp.o \
	                           LogicalEnviWrite.cpp.o \
	                           PhysicalEnviWrite.cpp.o \
	                           $(LIBS)

clean:
	rm -f *.o *.so
