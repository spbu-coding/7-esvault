include CONFIG.cfg

CC = gcc

EXECUTABLE = $(BUILD_DIR)/$(NAME)
SOURCES = $(SOURCE_DIR)/$(NAME).c
OBJECTS = $(EXECUTABLE).o

TESTS_IN = $(wildcard $(TEST_DIR)/*.in)
TESTS_NAMES = $(TESTS_IN:$(TEST_DIR)/%.in=%)
TESTS_OUT = $(wildcard $(TEST_DIR)/*.out)
SORT_OUT = $(TESTS_OUT:$(TEST_DIR)/%=$(BUILD_DIR)/%)

.PHONY: all check clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $<

$(OBJECTS): $(SOURCES) | $(BUILD_DIR)
	$(CC) -c -o $@ $<

$(BUILD_DIR): $(SOURCES)
	mkdir -p $@

check: $(BUILD_DIR)/log
	@exit $$(cat $<)

$(BUILD_DIR)/log: $(EXECUTABLE) $(TESTS_IN) $(TESTS_OUT)
	@test_check=0 ; \
	for test in $(TESTS_NAMES) ; do \
	  ./$< $(TEST_DIR)/$$test.in > $(BUILD_DIR)/$$test.out ; \
	  if cmp -s $(BUILD_DIR)/$$test.out $(TEST_DIR)/$$test.out ;  then \
	  	echo test $$test passed ; \
	  else \
	    echo test $$test failed ; \
	    test_check=1 ; \
	  fi \
	done ;\
	echo $$test_check > $@

clean:
	rm -rf $(BUILD_DIR)/
