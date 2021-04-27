TARGET = generate_metadata_file
OBJS += generate_metadata_file.o
#LIBS += -lleveldb
#LIBS += -lpthread
#LIBS += -lsnappy
#LIBS += -lpie

CC = g++
CFLAGS += -MMD -MP # dependency tracking flags
CFLAGS += -I./
CFLAGS += -Wall
#CFLAGS += -Werror
#CFLAGS += -Wconversion
#CFLAGS += -fPIE
LDFLAGS += -I/usr/include
LDFLAGS += $(LIBS)

all: CFLAGS += -g -O3 # release flags
all: $(TARGET)

release: clean all

debug: CFLAGS += -g -O0 -D_GLIBC_DEBUG # debug flags
debug: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

DEPS = $(OBJS:%.o=%.d)
	-include $(DEPS)

clean:
	-@rm $(TARGET) $(OBJS) $(DEPS) 2> /dev/null || true
