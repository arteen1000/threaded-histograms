TARGET_EXEC ?= histo

OPT ?= -O3 -ftree-vectorize
GCC ?= gcc

CFLAGS ?= -g $(OPT) -Wall -std=c11 -D_GNU_SOURCE

LDFLAGS ?= -lm -pthread


FILE = $(shell basename "$$PWD").tar.gz
TARGETS = histo.c cookie.txt comment.txt
SERVER = lnxsrv09
PORT = 15214
SUBMIT_API = http://${SERVER}.seas.ucla.edu:${PORT}/upload
CHECK_QUEUE_API = http://${SERVER}.seas.ucla.edu:${PORT}/ping-submission-queue

$(TARGET_EXEC): main.o histo.o histo-check.o
	$(GCC) $^ -o $@ $(LDFLAGS)

histo.o: histo.c histo.h
	$(GCC) $(CFLAGS) $(OPT) -c $< -o $@

main.o: main.c main.h histo.h
	$(GCC) $(CFLAGS) $(OPT) -c $< -o $@

ping:
	curl $(CHECK_QUEUE_API)

submit: build
	echo 'sending request to the server...'
	echo "it might take a few seconds..."
	curl -F 'file=@$(FILE)' $(SUBMIT_API)

.SILENT:
build:
	tar -czf $(FILE) $(TARGETS) 2>/dev/null

.PHONY: clean

clean:
	$(RM) -r $(TARGET_EXEC) histo.o main.o
