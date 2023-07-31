COUNTER_FILE := .counter.txt

ISSUE ?= default_issue

all: create_file

create_file: $(COUNTER_FILE)
	@echo "Creating file with name: $(FILE_NAME)"
	touch "$(FILE_NAME).txt"
	./.fill $(FILE_NAME) $(./)

