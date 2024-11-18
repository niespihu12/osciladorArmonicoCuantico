CC = gcc
CFLAGS = -Wall -lm
TARGET = oscilador
SRC = main.c
PYTHON_SCRIPT = main.py
VENV_DIR = .venv

# Detección de sistema operativo
OS := $(shell uname -s)

.PHONY: all clean venv run_c run_python view_image

all: run_c run_python

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lm

venv: $(VENV_DIR)/bin/activate

# Ajusta la ruta de activación del entorno virtual según el sistema operativo
$(VENV_DIR)/bin/activate:
ifeq ($(OS), Darwin) # macOS
	python3 -m venv $(VENV_DIR)
	$(VENV_DIR)/bin/pip install -r requirements.txt
else ifeq ($(OS), Linux) # Linux
	python3 -m venv $(VENV_DIR)
	$(VENV_DIR)/bin/pip install -r requirements.txt
else ifeq ($(OS), Windows_NT) # Windows
	python -m venv $(VENV_DIR)
	$(VENV_DIR)\Scripts\pip install -r requirements.txt
endif

run_c: $(TARGET)
ifeq ($(OS), Windows_NT)
	$(TARGET).exe
else
	./$(TARGET)
endif

run_python: venv
ifeq ($(OS), Windows_NT)
	$(VENV_DIR)\Scripts\python $(PYTHON_SCRIPT)
else
	$(VENV_DIR)/bin/python $(PYTHON_SCRIPT)
endif
	$(MAKE) view_image

view_image:
ifeq ($(OS), Darwin) # macOS
	open graficas.png
else ifeq ($(OS), Linux) # Linux
	xdg-open graficas.png
else ifeq ($(OS), Windows_NT) # Windows
	start graficas.png
endif

clean:
	rm -f $(TARGET)
	rm -rf $(VENV_DIR)
	rm -f estado_0.txt
	rm -f estado_1.txt
	rm -f estado_2.txt
	rm -f estado_3.txt
	rm -f graficas.png
ifeq ($(OS), Windows_NT)
	del /f $(TARGET).exe
endif