# Makefile to clean all executables

# This pattern matches all executable files (no extension)
EXECUTABLES = $(wildcard *)

# Filter out only files that are executable
EXECUTABLES := $(filter-out $(wildcard *.*),$(EXECUTABLES))

clean:
	@echo "Deleting executables..."
	@for file in $(EXECUTABLES); do \
		if [ -x $$file ]; then \
			echo "Removing $$file"; \
			rm -f $$file; \
		fi; \
	done