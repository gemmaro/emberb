PORT ?= 8080

all:
	po4a po4a.cfg
	cd _po/ja && jekyll build
.PHONY: all

serve:
	ruby -run -e httpd _po/ja/_site --port=8081
.PHONY: serve

.PHONEY: watch
watch:
	guard

.PHONEY: build
build:
	jekyll build

.PHONEY: test
test:
	$(MAKE) -C _test
