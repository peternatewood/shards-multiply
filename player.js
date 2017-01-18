gPlayer = {
  x: 0,
  y: 0,
  xVel: 0,
  yVel: 0,
  xAcc: 0,
  yAcc: 0,
  rad: 0,
  size: 20,
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
    this.xAcc = 0;
    this.yAcc = 0;
    this.xVel *= FRICTION;
    this.yVel *= FRICTION;
    if (gInput.isActive('left')) this.xAcc--;
    if (gInput.isActive('right')) this.xAcc++;
    if (gInput.isActive('up')) this.yAcc--;
    if (gInput.isActive('down')) this.yAcc++;
    this.xVel += this.xAcc;
    this.yVel += this.yAcc;
    var collision;
    for (var i in optionsTargets) {
      collision = this.collide(optionsTargets[i]);
      if (collision) {
        this.xVel *= -1;
        this.yVel *= -1;
        break;
      }
    }
    this.x = Math.min(gBounds[0].r - 20, Math.max(gBounds[0].l + 20, this.x + this.xVel * PLAYER_SPEED));
    this.y = Math.min(gBounds[0].b - 20, Math.max(gBounds[0].t + 20, this.y + this.yVel * PLAYER_SPEED));

    this.rad = Math.atan2(gInput.mouseY - this.y, gInput.mouseX - this.x);

    if (gInput.isActive('fire')) this.fire();
    // Loop through projectiles and only keep the ones still in the level bounds
    var liveProjectiles = [];
    this.projectiles.forEach(function(p) {
      var collision;
      for (var i in optionsTargets) {
        collision = p.collide(optionsTargets[i]);
        if (collision) break;
      }
      if (!collision && gBounds.collide(p)) p.dying = true;
      p.update();
      if (p.life > 0) liveProjectiles.push(p);
    });
    this.projectiles = liveProjectiles;
  },
  render: function(context) {
    context.beginPath();
    context.moveTo(this.x + (PLAYER_RADIUS * Math.cos(this.rad)), this.y + (PLAYER_RADIUS * Math.sin(this.rad)))
    context.lineTo(this.x + ((PLAYER_RADIUS / 2) * Math.cos(this.rad + (Math.PI / 4))), this.y + ((PLAYER_RADIUS / 2) * Math.sin(this.rad + (Math.PI / 4))));
    context.lineTo(this.x + (PLAYER_RADIUS * Math.cos(this.rad + (Math.PI / 2))), this.y + (PLAYER_RADIUS * Math.sin(this.rad + (Math.PI / 2))));
    context.lineTo(this.x + ((PLAYER_RADIUS * 1.25) * Math.cos(this.rad + (Math.PI * 3 / 4))), this.y + ((PLAYER_RADIUS * 1.25) * Math.sin(this.rad + (Math.PI * 3 / 4))));
    context.lineTo(this.x + ((PLAYER_RADIUS / 2) * Math.cos(this.rad + Math.PI)), this.y + ((PLAYER_RADIUS / 2) * Math.sin(this.rad + Math.PI)));
    context.lineTo(this.x + ((PLAYER_RADIUS * 1.25) * Math.cos(this.rad - (Math.PI * 3 / 4))), this.y + ((PLAYER_RADIUS * 1.25) * Math.sin(this.rad - (Math.PI * 3 / 4))));
    context.lineTo(this.x + (PLAYER_RADIUS * Math.cos(this.rad - (Math.PI / 2))), this.y + (PLAYER_RADIUS * Math.sin(this.rad - (Math.PI / 2))));
    context.lineTo(this.x + ((PLAYER_RADIUS / 2) * Math.cos(this.rad - (Math.PI / 4))), this.y + ((PLAYER_RADIUS / 2) * Math.sin(this.rad - (Math.PI / 4))));
    context.closePath();

    context.fillStyle = '#49D';
    context.strokeStyle = '#FB7';
    context.lineWidth = 3;

    context.fill();
    context.stroke();

    this.projectiles.forEach(function(p) {
      p.render(context);
    });
  }
}
