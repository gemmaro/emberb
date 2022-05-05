.PHONEY: default
default: translate build

.PHONEY: watch
watch:
	bundle exec guard

.PHONEY: serve
serve: translate
	bundle exec jekyll serve --watch --baseurl= --livereload

.PHONEY: build
build:
	bundle exec jekyll build

.PHONEY: test
test:
	$(MAKE) -C _test

po4a := po4a.cfg

.PHONEY: translate
translate: $(po4a)
	po4a po4a.cfg

$(po4a): generate_po4a_config_file.rb *.md
	./$< > $@

