#!/usr/bin/make

.DEFAULT_GOAL: all
all: dist

tools/depbo:
	@mkdir -p ./tools
	curl -L -o ./tools/depbo.tgz \
		"https://mikero.bytex.digital/api/download?filename=depbo-tools-0.10.18-linux-amd64.tgz"
	tar xzfv ./tools/depbo.tgz -C ./tools
	mv -v ./tools/depbo-* ./tools/depbo
	rm -v ./tools/depbo.tgz

## Build the server-ready mod into ./dist/
.PHONY: dist
dist: tools/depbo
	@rm -rf ./.build ./dist
	mkdir -pv ./.build ./dist/@common-mod/addons
	cp -rv ./config.cpp ./src ./.build/
	LD_LIBRARY_PATH=./tools/depbo/lib ./tools/depbo/bin/makepbo \
		-P -D -@=common-mod ./.build ./dist/@common-mod/addons/common-mod.pbo
	@rm -rf ./.build
