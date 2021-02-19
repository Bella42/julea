CC = g++

# define any compile-time flags
CFLAGS = -fPIC -std=c++17 -D_GNU_SOURCE -shared

# define any directories containing header files other than /usr/include
#
INCLUDES = -I../dependencies/ceph/src -I../dependencies/ceph/build/include -I../dependencies/ceph/build/boost/include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -L../dependencies/ceph/build/lib -L../dependencies/ceph/build/src/rocksdb

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lceph-common -lbluefs -lcommon -lpthread -lz -lceph_lz4 -llz4 -los -lcrypto -ldl -lceph_snappy -lsnappy -lcephfs -lkv -lleveldb -lrocksdb -lheap_profiler -laio -ltcmalloc -lglobal -lerasure_code -lblk -luring

.PHONY: clean

all:    
		$(CC) $(CFLAGS) $(INCLUDES) -c julea_bluestore.cc $(LFLAGS) $(LIBS)
		$(CC) $(CFLAGS) $(INCLUDES) julea_bluestore.o -o libjulea_bluestore.so $(LFLAGS) $(LIBS)

clean:
		$(RM) *.o *~ $(MAIN)
