SPRITE_CLI := ../tools/bin/bmp2data.exe
MAP_CLI := ../tools/bin/tiled2map.exe


LEVEL_JSONS := $(wildcard level*.json)
LEVEL_MAPS := $(LEVEL_JSONS:.json=.map)

WAV_FILES := $(wildcard *.wav)
WAV_DESTS := $(patsubst %.wav,../out/%.wav,$(WAV_FILES))

assets: $(LEVEL_MAPS) $(WAV_DESTS)
# Sprites
	$(SPRITE_CLI) font.bmp ../out/font.fnt 59
	$(SPRITE_CLI) fede.bmp ../out/fede.spr 22
	$(SPRITE_CLI) manoloca.bmp ../out/whip.spr 1
	$(SPRITE_CLI) joystick.bmp ../out/enemy1.spr 7
	$(SPRITE_CLI) misc.bmp ../out/misc.spr 7
	$(SPRITE_CLI) tiles.bmp ../out/tiles.spr 39
	$(SPRITE_CLI) intro.bmp ../out/intro.spr 1
	$(SPRITE_CLI) intro2.bmp ../out/intro2.spr 1
	$(SPRITE_CLI) menu.bmp ../out/menu.spr 1


# Sounds
../out/%.wav: %.wav
	cp $< $@

# Maps
%.map: %.json school-tiles.json
	$(MAP_CLI) $< school-tiles.json ../out/$@

.PHONY: assets