CC      = g++
# CC      = clang++
INCLUDE  = -I.
CXXFLAGS = -std=c++11 -D_GLIBCXX_USE_NANOSLEEP

# CCFLAGS = -std=c++11 -stdlib=libc++
LDFLAGS = -lm -lboost_thread-mt -lboost_system-mt -lpthread

all:
	$(CC) $(INCLUDE) $(CXXFLAGS) test_shared_mutex.cc -o test_shared_mutex $(LDFLAGS)
