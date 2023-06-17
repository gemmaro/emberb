class Class
  def new *args, &blk
    obj = allocate
    obj.initialize(*args, &blk)
    obj
  end

  def allocate
    # 空のインスタンスを作って返します
  end
end
