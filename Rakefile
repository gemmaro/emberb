# frozen_string_literal: true

require 'pathname'

# po_file_path is corresponding file path
module PathnameRefinements
  refine Pathname do
    def po_file_path
      Pathname('locales/ja') / basename.sub_ext('.po')
    end
  end
end

class Post
  using PathnameRefinements

  def initialize(markdown_file_path)
    @markdown_file_path = markdown_file_path
  end

  def po
    @po ||= @markdown_file_path.po_file_path
  end

  def markdown = @markdown_file_path

  def translated_markdown
    @translated_markdown ||= @markdown_file_path.sub_ext('.ja.md')
  end
end

posts = Pathname.glob('_posts/*.markdown')
  .map { |post| Post.new(post) }

posts.each do |post|
  file post.po => post.markdown do |t|
    sh "po4a-gettextize -f text -m #{t.source} -p #{t.name}"
  end

  file post.translated_markdown => [post.markdown, post.po] do |t|
    markdown, po = t.sources

    sh "po4a-translate -f text -m #{markdown} -p #{po} -l #{t.name} -k 0"
  end
end

task default: posts.map { _1.translated_markdown }
