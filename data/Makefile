SPRITE_CLI := ../tools/bin/bmp2data.exe
MAP_CLI := ../tools/bin/tiled2map.exe

# Find all JSON files starting with "level"
LEVEL_JSONS := $(wildcard level*.json)
# Generate corresponding .map filenames
LEVEL_MAPS := $(LEVEL_JSONS:.json=.map)

assets: $(LEVEL_MAPS)
# Sprites
	$(SPRITE_CLI) font.bmp ../out/font.fnt 59
	$(SPRITE_CLI) fede.bmp ../out/fede.spr 22
	$(SPRITE_CLI) manoloca.bmp ../out/whip.spr 1
	$(SPRITE_CLI) joystick.bmp ../out/enemy1.spr 7
	$(SPRITE_CLI) misc.bmp ../out/misc.spr 7
	$(SPRITE_CLI) tiles.bmp ../out/tiles.spr 39
	$(SPRITE_CLI) intro.bmp ../out/intro.spr 1
# Maps
	$(MAP_CLI) school.json school-tiles.json ../out/school1.map
# Sounds
	cp whip.wav ../out/whip.wav

%.map: %.json school-tiles.json
	$(MAP_CLI) $< school-tiles.json ../out/$@

.PHONY: assets