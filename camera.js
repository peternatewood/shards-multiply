gCamera = {
  x: 0,
  y: 0,
  xVel: 0,
  yVel: 0,
  xAcc: 0,
  yAcc: 0,
  target: { x: 0, y: 0 },
  isInView: function(object) {
    return object.x + object.size > this.x && object.x - object.size < this.x + SCREEN_WIDTH && object.y + object.size > this.y && object.y - object.size < this.y + SCREEN_HEIGHT;
  },
  update: function() {
    if (gInput.isActive('thrust')) {
      this.target.x = (gPlayer.x - (SCREEN_WIDTH / 2)) + (200 * Math.cos(gPlayer.rad));
      this.target.y = (gPlayer.y - (SCREEN_HEIGHT / 2)) + (200 * Math.sin(gPlayer.rad));
    }

    var rad = Math.atan2(this.target.y - this.y, this.target.x - this.x);
    var maxDistance = Math.max(-1 * CAMERA_SPEED, Math.min(CAMERA_SPEED, Math.sqrt(Math.pow(this.target.x - this.x, 2) + Math.pow(this.target.y - this.y, 2))));

    this.x = Math.min(gScene.bounds.r - SCREEN_WIDTH, Math.max(gScene.bounds.l, this.x + (maxDistance * Math.cos(rad))));
    this.y = Math.min(gScene.bounds.b - SCREEN_HEIGHT, Math.max(gScene.bounds.t, this.y + (maxDistance * Math.sin(rad))));
  }
}
