LOCAL=$$system(echo $HOME)/local

QMAKE_CFLAGS_X86_64   -= -mmacosx-version-min=10.5
QMAKE_CXXFLAGS_X86_64 -= -mmacosx-version-min=10.5
QMAKE_LFLAGS_X86_64   -= -mmacosx-version-min=10.5
QMAKE_LFLAGS          -= -mmacosx-version-min=10.5

QMAKE_CXXFLAGS  += -std=c++11 -I$${LOCAL}/include -D_GLIBCXX_USE_NANOSLEEP
QMAKE_LFLAGS    += -std=c++11 -L$${LOCAL}/lib

macx {
QMAKE_CXXFLAGS  += -mmacosx-version-min=10.7
# QMAKE_LFLAGS    +=
}
unix:!macx{
# linux only
}

LIBS        +=     \
-ldl -lpthread -lz \
-lboost_thread-mt  \
-lboost_system-mt

HEADERS =

SOURCES = \
test_shared_mutex.cc
