#!/bin/bash

po4a_config_file_path=po4a.cfg

echo '[po_directory] po/' > $po4a_config_file_path

for markdown_file_path in _posts/*.markdown ; do
	markdown_basename=$(basename "$markdown_file_path")
  markdown_stem_name=$(basename "$markdown_file_path" .markdown)

	echo "[type: text] _posts/$markdown_basename \$lang:_posts/$markdown_stem_name.\$lang.markdown opt:\"--option markdown --option neverwrap\" --keep 0" >> $po4a_config_file_path
done

