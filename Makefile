.PHONEY: default
default: generate_translations

.PHONEY: serve
serve:
	bundle exec jekyll serve --watch --baseurl=

.PHONEY: build
build:
	bundle exec jekyll build

.PHONEY: test
test:
	$(MAKE) -C _test

pot_file_path := po/all.pot

.PHONEY: generate_translations
generate_translations: $(pot_file_path)
	po4a po4a.cfg

$(pot_file_path): generate_po4a_config_file.sh *.md
	./$<

