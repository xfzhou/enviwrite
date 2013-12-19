#!/bin/bash
iquery -aq "unload_library('enviwrite')"
scidb.py stopall envi
sudo rm /opt/scidb/13.9/lib/scidb/plugins/libenviwrite.so
make clean
make
sudo cp libenviwrite.so /opt/scidb/13.9/lib/scidb/plugins
scidb.py startall envi
iquery -aq "load_library('enviwrite')"

