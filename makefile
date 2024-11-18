C = gcc
O = -o
W = -Wall
PAQUETES = -lm

ARCHIVO_C = main.c
EJECUTABLE_C = oscilador
VENV = venv
ARCHIVO_PY = main.py

.PHONY: all clean run python_run c_run

all: $(EJECUTABLE_C) python_venv

# C compilation rule
$(EJECUTABLE_C): $(ARCHIVO_C)
	$(C) $(W) $(O) $@ $^ $(PAQUETES)

# Python virtual environment setup
python_venv: $(VENV)/bin/activate

$(VENV)/bin/activate: requirements.txt
	python3 -m venv $(VENV)
	./$(VENV)/bin/pip install -r requirements.txt

# Run C executable
c_run: $(EJECUTABLE_C)
	./$(EJECUTABLE_C)

# Run Python script in virtual environment
python_run: python_venv
	./$(VENV)/bin/python3 $(ARCHIVO_PY)

# Clean both C executable and Python virtual environment
clean:
	rm -f $(EJECUTABLE_C)
	rm -rf $(VENV)
	find . -type f -name '*.pyc' -delete