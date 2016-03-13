CC = gcc 
CXX = g++ 
CFLAGS = -g -Wall -O0

INCLUDE	:= -I./ -I../include -I../dep/curl

EXES = $(shell ls *.cpp | awk -F. '{print $$1}')
all : $(EXES)
LIBS = -lpthread -lexpat -lz -pthread  ../src/wslib.a -lcurl ../dep/curl/libcurl.a

.%.d: %.cpp
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^\. "/" "/  |sed s/"\.o"/""/ > $@
	@echo '$^' | awk -F. '{print "	$(CXX) $(CFLAGS) $(INCLUDE) -o "$$1" $^ $(LIBS)"}' >> $@

DEPS = $(patsubst %, .%.d, $(EXES))

-include $(DEPS)

clean:
	rm -f *.o .*.d *.log $(EXES)
