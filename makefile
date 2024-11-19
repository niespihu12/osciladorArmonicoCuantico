CC = gcc
CFLAGS = -Wall -lm
TARGET = oscilador
SRC = main.c
PYTHON_SCRIPT = main.py
VENV_DIR = .venv

# Detección de sistema operativo
OS := $(shell uname -s)

# Variables para verificar dependencias
PYTHON_CMD := $(shell which python3 2>/dev/null || which python 2>/dev/null)
GCC_CMD := $(shell which gcc 2>/dev/null)
EOG_CMD := $(shell which eog 2>/dev/null)

.PHONY: all clean venv run_c run_python view_image check_dependencies install_dependencies

all: check_dependencies run_c run_python

# Verificar dependencias
check_dependencies: install_dependencies
	@echo "Verificando dependencias..."

# Instalar dependencias según el sistema operativo
install_dependencies:
ifeq ($(OS), Linux)
ifeq ($(DISTRO), ubuntu)
	@if [ -z "$(GCC_CMD)" ]; then \
		echo "Instalando gcc en Ubuntu..."; \
		sudo apt-get update && sudo apt-get install -y gcc; \
	fi
	@if [ -z "$(PYTHON_CMD)" ]; then \
		echo "Instalando Python en Ubuntu..."; \
		sudo apt-get update && sudo apt-get install -y python3 python3-pip python3-venv; \
	fi
	@if [ -z "$(EOG_CMD)" ]; then \
		echo "Instalando eog en Ubuntu..."; \
		sudo apt-get update && sudo apt-get install -y eog; \
	fi
else ifeq ($(DISTRO), fedora)
	@if [ -z "$(GCC_CMD)" ]; then \
		echo "Instalando gcc en Fedora..."; \
		sudo dnf install -y gcc; \
	fi
	@if [ -z "$(PYTHON_CMD)" ]; then \
		echo "Instalando Python en Fedora..."; \
		sudo dnf install -y python3 python3-pip python3-virtualenv; \
	fi
	@if [ -z "$(EOG_CMD)" ]; then \
		echo "Instalando eog en Fedora..."; \
		sudo dnf install -y eog; \
	fi
endif
else ifeq ($(OS), Darwin)
	@if [ -z "$(GCC_CMD)" ]; then \
		echo "Instalando gcc..."; \
		xcode-select --install; \
	fi
	@if [ -z "$(PYTHON_CMD)" ]; then \
		echo "Instalando Python..."; \
		brew install python3; \
	fi

else ifeq ($(OS), Windows_NT)
	@echo "En Windows, por favor instala manualmente Python, GCC (MinGW) y un visor de imágenes"
endif

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lm

venv: $(VENV_DIR)/bin/activate

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
	if [ -x "$(EOG_CMD)" ]; then \
		eog graficas.png; \
	else \
		xdg-open graficas.png; \
	fi
else ifeq ($(OS), Windows_NT) # Windows
	start graficas.png
endif

clean:
	rm -f $(TARGET)
	rm -rf $(VENV_DIR)
	rm -f estado_0.txt estado_1.txt estado_2.txt estado_3.txt graficas.png
ifeq ($(OS), Windows_NT)
	del /f $(TARGET).exe
endif