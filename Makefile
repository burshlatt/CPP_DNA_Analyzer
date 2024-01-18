CXX = g++
FLAGS = -Wall -Werror -Wextra -std=c++17 -O3

TEST_FLAGS = -lgtest -pthread

OS := $(shell uname)
ifeq ($(OS), Linux)
    TEST_FLAGS += -lgtest_main
else ifeq ($(OS), Darwin) 
    TEST_FLAGS += -lgmock
endif

.PHONY: all app tests clean style

#	== ВЫПОЛНИТЬ ВСЕ ==
all: clean tests app

#	== СБОРКА И ЗАПУСК ПРОГРАММЫ ==
app: clean
	@echo --------------------- START ---------------------
	$(CXX) $(FLAGS) src/main.cc -o DNA && ./DNA

#	== ЗАПУСК UNIT-ТЕСТОВ ==
tests:
	@echo --------------------- START ---------------------
	cd ./tests/ && $(CXX) $(FLAGS) unit_tests.cc -o unit_tests $(TEST_FLAGS)
	cd ./tests/ && ./unit_tests
	@echo -------------------- SUCCESS --------------------

#	== ОЧИСТКА ФАЙЛОВ ==
clean:
	@echo --------------------- CLEAN ---------------------
	rm -rf DNA tests/unit_tests
	@echo -------------------- SUCCESS --------------------

#   === ПРОВЕРКА СТИЛЯ ===
style:
	@echo --------------------- STYLE ---------------------
	clang-format -n -style=GOOGLE src/*/*.hpp
	@echo -------------------- SUCCESS --------------------
