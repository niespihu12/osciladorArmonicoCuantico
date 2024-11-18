CC = gcc
CFLAGS = -Wall -lm
TARGET = oscilador
SRC = main.c
PYTHON_SCRIPT = main.py
VENV_DIR = .venv

.PHONY: all clean venv run_c run_python

all: run_c run_python

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lm

venv: $(VENV_DIR)/bin/activate

$(VENV_DIR)/bin/activate:
	python3 -m venv $(VENV_DIR)
	$(VENV_DIR)/bin/pip install -r requirements.txt

run_c: $(TARGET)
	./$(TARGET)

run_python: venv
	./$(VENV_DIR)/bin/python $(PYTHON_SCRIPT)

clean:
	rm -f $(TARGET)
	rm -rf $(VENV_DIR)
	rm -r estado_0.txt
	rm -r estado_1.txt
	rm -r estado_2.txt
	rm -r estado_3.txt
	rm -r graficas.png
