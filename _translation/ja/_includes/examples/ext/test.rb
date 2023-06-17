require 'minitest/autorun'
require './gmp'

describe GMP::Integer do
  describe "when initialized" do
    it "must accept a Fixnum" do
      x = 3
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must accept a Bignum" do
      x = 99999999999999999999999999999999
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must accept a GMP::Integer" do
      x = GMP::Integer.new(4)
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must accept a String" do
      x = '87678392019238123109238'
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must raise if the base is invalid" do
      # 文字列付きの基数のみを受け付けます
      proc { GMP::Integer.new(3, 10) }.must_raise TypeError
      # fixnumの基数のみ受け付けます
      proc { GMP::Integer.new('3', 1.5) }.must_raise TypeError
      # 間違った基数の範囲
      proc { GMP::Integer.new('3', 64) }.must_raise RangeError
      # 文字列が不正な基数
      proc { GMP::Integer.new('3', 2) }.must_raise ArgumentError
      # ヨシ
      GMP::Integer.new('3', 9)
    end
  end

  describe "when converting to String" do
    it "must raise if the base is invalid" do
      x = GMP::Integer.new(3)

      # 基数はFixnumでなければなりません
      proc { x.to_s(1.5) }.must_raise TypeError
      # 間違った基数の範囲
      proc { x.to_s(64) }.must_raise RangeError
      # ヨシ
      x.to_s(2)
    end
  end

  describe "when converting to integer" do
    it "must be equal" do
      x = GMP::Integer.new(3)
      x.to_i.to_s.must_equal x.to_s
    end
  end

  describe "when comparing" do
    it "must equal only the same type" do
      x = GMP::Integer.new(3)
      y = GMP::Integer.new(3)
      z = 3
      x.must_equal y
      x.wont_equal z
    end

    it "can be sorted" do
      ary = [3,5,1,2,0,-123,-8,6]
      sorted = ary.sort
      ary.map { |x| GMP::Integer.new(x) }.sort.must_equal(sorted.map { |x| GMP::Integer.new(x) })
    end
  end

  describe "when adding" do
    it "must be correct" do
      x = GMP::Integer.new(3)
      y = GMP::Integer.new(4)
      (x + y).must_equal GMP::Integer.new(7)
    end

    it "must raise for other types" do
      x = GMP::Integer.new(3)
      y = 4
      proc { x + y }.must_raise TypeError
    end
  end

  describe "when negating" do
    it "must add to 0" do
      x = GMP::Integer.new(3)
      y = -x
      (x + y).must_equal GMP::Integer.new(0)
    end
  end
end
