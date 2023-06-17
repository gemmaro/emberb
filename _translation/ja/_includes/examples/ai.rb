def think ai, player
  # 位置を得る
  ax, ay = ai.pos

  # プレーヤーが動いた方向を得る
  dx, dy = player.dir

  # ……動作の仕組み……
  x = dy
  y = -dx

  # この方向に動く
  ai.move x, y
end
