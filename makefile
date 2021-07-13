C = gcc
CFLAGS = -Wall -std=c99
DBGFLAGS = -g
IFLAGS = -I include
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
TARGET = game
SRCSDIR = src
OBJSDIR = obj
DBGDIR = dbg
SRCS = $(wildcard $(SRCSDIR)/*.c)
OBJS = $(patsubst $(SRCSDIR)/%.c,$(OBJSDIR)/%.o,$(SRCS))
DBGS = $(patsubst $(SRCSDIR)/%.c,$(DBGDIR)/%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(C) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(TARGET)

$(OBJSDIR)/%.o : $(SRCSDIR)/%.c
	mkdir -p $(OBJSDIR)
	$(C) $(CFLAGS) $(IFLAGS) -c $< -o $@


$(DBGDIR)/%.o : $(SRCSDIR)/%.c
	mkdir -p $(DBGDIR)
	$(C) $(CFLAGS) $(DBGFLAGS) $(IFLAGS) -c $< -o $@

.PHONY: clean debug

clean:
	rm -rf $(OBJSDIR)
	rm -rf $(DBGDIR)
	rm $(TARGET)

debug: $(DBGS)
	$(C) $(CFLAGS) $(DBGFLAGS) $(LFLAGS) $(DBGS) -o $(TARGET)
