gPlayer = {
  x: 0,
  y: 0,
  xVel: 0,
  yVel: 0,
  rad: 0,
  size: 20,
  speed: PLAYER_SPEED,
  allowFire: true,
  projectiles: [],
  fire: function() {
    if (this.allowFire) {
      this.projectiles.push(new Bolt(this.x, this.y, this.rad));
      this.allowFire = false;
      gAudio.eChan1.startBolt();
      setTimeout(function() { this.allowFire = true }.bind(this), Bolt.delay);
    }
  },
  collide: function(other) {
    var rayToCenter = Math.sqrt(Math.pow(other.x - this.x, 2) + Math.pow(other.y - this.y, 2));
    var collide = rayToCenter < this.size;

    if (!collide) {
      var lastX = this.x - this.xVel;
      var lastY = this.y - this.yVel;
      var lastRay = Math.sqrt(Math.pow(this.x - lastX, 2) + Math.pow(this.y - lastY, 2));
      collide = Math.sqrt(Math.pow(rayToCenter, 2) + Math.pow(lastRay)) < other.size;
    }

    return collide;
  },
  update: function() {
    var xAcc = 0;
    var yAcc = 0;
    this.xVel *= FRICTION;
    this.yVel *= FRICTION;
    if (gInput.isActive('thrust')) {
      this.speed = THRUSTER_SPEED;
      xAcc = Math.cos(this.rad);
      yAcc = Math.sin(this.rad);
    }
    else {
      this.speed = PLAYER_SPEED;
      if (gInput.isActive('left')) xAcc--;
      if (gInput.isActive('right')) xAcc++;
      if (gInput.isActive('up')) yAcc--;
      if (gInput.isActive('down')) yAcc++;
    }
    // this.xVel = Math.abs(this.xVel) > 2e-20 ? this.xVel + xAcc : 0;
    // this.yVel = Math.abs(this.yVel) > 2e-20 ? this.yVel + yAcc : 0;
    this.xVel += xAcc;
    this.yVel += yAcc;

    var collision;
    for (var i in gScene.targets) {
      collision = this.collide(gScene.targets[i]);
      if (collision) {
        this.xVel *= -1;
        this.yVel *= -1;
        break;
      }
    }
    this.x = Math.min(Math.min(gCamera.x + SCREEN_WIDTH, gScene.bounds.r) - 20, Math.max(Math.max(gCamera.x, gScene.bounds.l) + 20, this.x + this.xVel * this.speed));
    this.y = Math.min(Math.min(gCamera.y + SCREEN_HEIGHT, gScene.bounds.b) - 20, Math.max(Math.max(gCamera.y, gScene.bounds.t) + 20, this.y + this.yVel * this.speed));

    this.rad = Math.atan2(gInput.mouseY - (this.y - gCamera.y), gInput.mouseX - (this.x - gCamera.x));

    if (gInput.isActive('fire')) this.fire();
    // Loop through projectiles and only keep the ones still in the level bounds
    var liveProjectiles = [];
    this.projectiles.forEach(function(p) {
      var collision;
      for (var i in gScene.targets) {
        collision = p.collide(gScene.targets[i]);
        if (collision) break;
      }
      if (!collision && gScene.bounds.collide(p)) p.dying = true;
      p.update();
      if (p.life > 0) liveProjectiles.push(p);
    });
    this.projectiles = liveProjectiles;
  },
  render: (function(context) {
    function render() {
      context.beginPath();
      context.moveTo(this.x + (PLAYER_RADIUS * Math.cos(this.rad)) - gCamera.x, this.y + (PLAYER_RADIUS * Math.sin(this.rad)) - gCamera.y);
      context.lineTo(this.x + ((PLAYER_RADIUS / 2) * Math.cos(this.rad + (Math.PI / 4))) - gCamera.x, this.y + ((PLAYER_RADIUS / 2) * Math.sin(this.rad + (Math.PI / 4))) - gCamera.y);
      context.lineTo(this.x + (PLAYER_RADIUS * Math.cos(this.rad + (Math.PI / 2))) - gCamera.x, this.y + (PLAYER_RADIUS * Math.sin(this.rad + (Math.PI / 2))) - gCamera.y);
      context.lineTo(this.x + ((PLAYER_RADIUS * 1.25) * Math.cos(this.rad + (Math.PI * 3 / 4))) - gCamera.x, this.y + ((PLAYER_RADIUS * 1.25) * Math.sin(this.rad + (Math.PI * 3 / 4))) - gCamera.y);
      context.lineTo(this.x + ((PLAYER_RADIUS / 2) * Math.cos(this.rad + Math.PI)) - gCamera.x, this.y + ((PLAYER_RADIUS / 2) * Math.sin(this.rad + Math.PI)) - gCamera.y);
      context.lineTo(this.x + ((PLAYER_RADIUS * 1.25) * Math.cos(this.rad - (Math.PI * 3 / 4))) - gCamera.x, this.y + ((PLAYER_RADIUS * 1.25) * Math.sin(this.rad - (Math.PI * 3 / 4))) - gCamera.y);
      context.lineTo(this.x + (PLAYER_RADIUS * Math.cos(this.rad - (Math.PI / 2))) - gCamera.x, this.y + (PLAYER_RADIUS * Math.sin(this.rad - (Math.PI / 2))) - gCamera.y);
      context.lineTo(this.x + ((PLAYER_RADIUS / 2) * Math.cos(this.rad - (Math.PI / 4))) - gCamera.x, this.y + ((PLAYER_RADIUS / 2) * Math.sin(this.rad - (Math.PI / 4))) - gCamera.y);
      context.closePath();

      context.fillStyle = '#49D';
      context.strokeStyle = '#FB7';
      context.lineWidth = 3;

      context.fill();
      context.stroke();
    }
    return render;
  })(gRenderer.context)
}
