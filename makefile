C = gcc
CFLAGS = -Wall -std=c99
DBGFLAGS = -g
IFLAGS = -I include
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

# EM_ENV=LLVM=/usr/bin NODE_JS=node EMSCRIPTEN_ROOT=/usr/lib/emscripten
EM_ENV=
EM_CC=emcc
EM_CFLAGS=-s WASM=1 -O3
EM_LDFLAGS=-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2

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

.PHONY: clean debug em

clean:
	rm -rf $(OBJSDIR)
	rm -rf $(DBGDIR)
	rm $(TARGET)
	rm index.js
	rm index.wasm

debug: $(DBGS)
	$(C) $(CFLAGS) $(DBGFLAGS) $(LFLAGS) $(DBGS) -o $(TARGET)

em:
	${EM_CC} ${EM_CLFAGS} $(SRCS) ${EM_LDFLAGS} -o index.html --preload-file res -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
