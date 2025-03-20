DOSBOX := "$(DOSBOX_PATH)/DOSBox.exe"

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

.PHONY: all clean run tools clean_all