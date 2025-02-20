# Makefile for compiling and uploading arduino sketches
ACLI        = arduino-cli
FLAGS      ?= --verbose
PORT       ?= /dev/cu.usbmodem1101
BOARD      ?= arduino:avr:uno

SOURCES     = motor/motor.ino
BIN         = bin
OUTPUT      = $(BIN)/$(notdir $(basename $(SOURCES))).ino.hex

TMP_COMPILE = $(BIN)/.compile
TMP_UPLOAD  = $(BIN)/.upload

.PHONY: all compile upload serial boards help vars clean cleaner

all: upload

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
	$(ACLI) monitor $(FLAGS) --port $(PORT)

boards:
	$(ACLI) board list

RM = rm -f
clean:
	$(RM) $(TMP_COMPILE) $(TMP_UPLOAD)
	$(RM) $(OUTPUT)

cleaner:
	$(RM) -r $(BIN)

# print variables
vars:
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
