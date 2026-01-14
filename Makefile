CXX := g++
CXXFLAGS := -O2 -lcpr

DESTDIR ?= $$HOME/.local
BINDIR := $(PREFIX)/bin

SRC := main.cpp
TARGET := hsltt

hsltt: main.cpp Makefile
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

install: $(TARGET)
	@echo "Installing $(TARGET) to $(DESTDIR)$(BINDIR)"
	install -dv "$(DESTDIR)$(BINDIR)"
	install -vm 755 "$(TARGET)" "$(DESTDIR)$(BINDIR)/$(TARGET)"

uninstall:
	@echo "Removing $(DESTDIR)$(BINDIR)/$(TARGET)"
	rm "$(DESTDIR)$(BINDIR)/$(TARGET)"

clean:
	rm $(TARGET)
