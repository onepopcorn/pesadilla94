DOSBOX := $(DOSBOX_PATH)

LANG ?=
ifeq ($(LANG), en)
	SUFFIX := en
else
	SUFFIX := es
endif

all: assets
	@ echo -----------------
	@echo  - BUILD GAME -
	@ echo -----------------
	@echo
	@make --no-print-directory -j -C src all
	@echo

assets: tools
# TODO Get BMP files from data and compile them to spr in out folder
	@ echo ---------------------
	@echo  - BUILD GAME ASSETS -
	@ echo ---------------------
	@make --no-print-directory -j -C data assets

run: all
	$(DOSBOX) -config dosbox.conf

tools:
	@echo ------------------
	@echo - BUILD TOOLS -
	@echo ------------------
	@echo
	@make --no-print-directory -j -C tools all
	@echo

clean:
	@make --no-print-directory -C src clean

clean_all:
	@make --no-print-directory -C src clean
	@make --no-print-directory -C tools clean

pack: all
	cp data/dosbox.conf out/dosbox.conf
	./compress.bat $(SUFFIX)

list:
	@echo ------------------
	@echo - Commands list -
	@echo ------------------
	@echo
	@printf "all:\n - build all project\n"
	@printf "clean:\n - clean game files only\n"
	@printf "clean_all:\n - clean game and tools files\n"
	@printf "run:\n - compile game & run it in DOSBOX\n"
	@printf "pack:\n - compile and generate .zip compressed file\n"
	@printf "list:\n - this command\n"



.PHONY: all clean run tools clean_all pack list