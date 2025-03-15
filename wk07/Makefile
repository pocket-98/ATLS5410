# Makefile for compiling and uploading arduino sketches
PANDOC  = pandoc
PDFLAGS  ?=  --from=gfm --to=html5+smart --css styling.css -s --mathjax

MDSOURCES = documentation.md
MDOUTPUTS = $(patsubst %.md,%.html, $(filter %.md,$(MDSOURCES)))

ACLI        = arduino-cli
FLAGS      ?= --verbose
PORT       ?= /dev/tty.usbserial-0001
BOARD      ?= esp32:esp32:esp32da
BAUD       ?= 115200

SOURCES     = ir_blast/ir_blast.ino
BIN         = bin
OUTPUT      = $(BIN)/$(notdir $(basename $(SOURCES))).ino.bin

TMP_COMPILE = $(BIN)/.compile
TMP_UPLOAD  = $(BIN)/.upload

.PHONY: all compile upload serial boards help vars clean cleaner

all: $(MDOUTPUTS) upload

%.html: %.md
	$(PANDOC) $(PDFLAGS) -o $@ $<

$(TMP_COMPILE): $(OUTPUT)
	touch $(TMP_COMPILE)

$(OUTPUT): $(SOURCES)
	$(ACLI) compile $(FLAGS) --fqbn $(BOARD) --output-dir $(BIN) $(SOURCES)

compile: $(TMP_COMPILE)

$(TMP_UPLOAD): compile
	$(ACLI) upload $(FLAGS) --port $(PORT) --fqbn $(BOARD) --build-path $(BIN) $(OUTPUT)
	touch $(TMP_UPLOAD)

upload: $(TMP_UPLOAD)

$(TMP_UPLOAD): $(TMP_COMPILE)

serial:
	$(ACLI) monitor --config $(BAUD) $(FLAGS) --port $(PORT)

boards:
	$(ACLI) board list

RM = rm -f
clean:
	$(RM) $(TMP_COMPILE) $(TMP_UPLOAD)
	$(RM) $(OUTPUT)
	$(RM) $(MDOUTPUTS)

cleaner:
	$(RM) -r $(BIN)
	$(RM) $(MDOUTPUTS)

# print variables
vars:
	@echo "PANDOC                   $(PANDOC)"
	@echo "PDFLAGS                  $(PDFLAGS)"
	@echo "MDSOURCES                $(MDSOURCES)"
	@echo "MDOUTPUTS                $(MDOUTPUTS)"
	@echo "ACLI                     $(ACLI)"
	@echo "FLAGS                    $(FLAGS)"
	@echo "PORT                     $(PORT)"
	@echo "BOARD                    $(BOARD)"
	@echo "SOURCES                  $(SOURCES)"
	@echo "OUTPUT                   $(OUTPUT)"

# print help
help:
	@echo "make all     compile and upload the sketch"
	@echo "make boards  list available boards"
	@echo "make compile compile sketch"
	@echo "make upload  upload sketch"
	@echo "make serial  connect to serial port of board"
	@echo "make help    show this help message"
	@echo "make vars    show variables"
	@echo "make clean   show variables"
