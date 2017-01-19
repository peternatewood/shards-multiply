gCamera = {
  x: 0,
  y: 0,
  xVel: 0,
  yVel: 0,
  xAcc: 0,
  yAcc: 0,
  isInView: function(object) {
    return object.x + object.size > this.x && object.x - object.size < this.x + SCREEN_WIDTH && object.y + object.size > this.y && object.y - object.size < this.y + SCREEN_HEIGHT;
  },
  update: function() {
    var xAcc = 0;
    var yAcc = 0;
    this.xVel *= FRICTION;
    this.yVel *= FRICTION;
    if (gInput.isActive('thrust')) {
      xAcc = Math.cos(gPlayer.rad);
      yAcc = Math.sin(gPlayer.rad);
    }
    // this.xVel = Math.abs(xAcc) > 2e-20 ? this.xVel + xAcc : 0;
    // this.yVel = Math.abs(yAcc) > 2e-20 ? this.yVel + yAcc : 0;
    this.xVel += xAcc;
    this.yVel += yAcc;
    this.x = Math.min(gScene.bounds.r - SCREEN_WIDTH, Math.max(gScene.bounds.l, this.x + this.xVel * THRUSTER_SPEED));
    this.y = Math.min(gScene.bounds.b - SCREEN_HEIGHT, Math.max(gScene.bounds.t, this.y + this.yVel * THRUSTER_SPEED));
  }
}
