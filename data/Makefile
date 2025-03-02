SPRITE_CLI := ../tools/bin/bmp2data.exe
MAP_CLI := ../tools/bin/tiled2map.exe

assets:
	$(SPRITE_CLI) font.bmp ../out/font.fnt 59
	$(SPRITE_CLI) fede.bmp ../out/fede.spr 17
	$(SPRITE_CLI) manoloca.bmp ../out/whip.spr 1
	$(SPRITE_CLI) joystick.bmp ../out/enemy1.spr 7
	$(SPRITE_CLI) tiles.bmp ../out/tiles.spr 30
	$(MAP_CLI) school.json school-tiles.json ../out/school1.map
	$(SPRITE_CLI) intro.bmp ../out/intro.spr 1

.PHONY: assets