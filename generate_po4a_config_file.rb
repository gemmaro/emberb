#!/usr/bin/env ruby

# frozen_string_literal: true

require 'pathname'

po4a_config_file_content = "[po_directory] _po/\n"

Pathname.glob('_posts/*.markdown').each do |markdown_file_path|
  markdown_base_name = markdown_file_path.basename
  markdown_stem_name = markdown_file_path.basename('.markdown')

  next if (/\d{4}-\d{2}-\d{2}-.+\...\.markdown/).match?(markdown_base_name.to_s)

  po4a_config_file_content += <<~END_OF_CFG

    [type: text] \\
      _posts/#{markdown_base_name} \\
      $lang:_posts/#{markdown_stem_name}.$lang.markdown \\
      opt:"--option markdown --keep 0"
  END_OF_CFG
end

print po4a_config_file_content
