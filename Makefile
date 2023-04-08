.PHONEY: default
default: translate build

.PHONEY: watch
watch:
	guard

.PHONEY: serve
serve: translate
	jekyll serve --watch --baseurl= --livereload

.PHONEY: build
build:
	jekyll build

.PHONEY: test
test:
	$(MAKE) -C _test

po4a := po4a.cfg

.PHONEY: translate
translate: $(po4a)
	po4a po4a.cfg
