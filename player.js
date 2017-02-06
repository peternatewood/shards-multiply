gPlayer = {
  x: 0,
  y: 0,
  xVel: 0,
  yVel: 0,
  rad: 0,
  size: 20,
  speed: PLAYER_SPEED,
  allowFire: true,
  firePower: 0,
  projectiles: [],
  life: PLAYER_LIFE,
  animation: '',
  frame: 0,
  hasClones: false,
  clones: [],
  hasMissiles: false,
  missiles: 0,
  hasBeam: false,
  beam: 0,
  hasShield: false,
  shield: 0,
  hasArmor: false,
  armor: 0,
  special: 0,
  hasWeapon: function(type) {
    switch (type) {
      case 1: return this.hasClones;
      case 2: return this.hasMissiles;
      case 3: return this.hasBeam;
      case 4: return this.hasShield;
      case 5: return this.hasArmor;
    }
  },
  cycleSpecial: function() {
    var special = Math.max(1, (this.special + 1) % 6);
    for (var i = 0; !this.hasWeapon(special) && i < 6; i++) {
      special = Math.max(1, (special + 1) % 6);
    }
    this.special = this.hasWeapon(special) ? special : 0;
  },
  fire: function() {
    if (this.allowFire) {
      switch (this.firePower) {
        case 0:
          this.projectiles.push(new Bolt(this));
          break;
        case 1:
          var source = {
            x: this.x + (6 * Math.cos(this.rad + (Math.PI / 2))), y: this.y + (6 * Math.sin(this.rad + (Math.PI / 2))),
            xVel: this.xVel, yVel: this.yVel,
            rad: this.rad
          }
          this.projectiles.push(new Bolt(source));
          source.x = this.x + (6 * Math.cos(this.rad - (Math.PI / 2)));
          source.y = this.y + (6 * Math.sin(this.rad - (Math.PI / 2)));
          this.projectiles.push(new Bolt(source));
          break;
        case 2:
          this.projectiles.push(new Bolt(this));
          var source = {
            x: this.x + (14 * Math.cos(this.rad + (Math.PI * 2 / 3))), y: this.y + (14 * Math.sin(this.rad + (Math.PI * 2 / 3))),
            xVel: this.xVel, yVel: this.yVel,
            rad: this.rad
          }
          this.projectiles.push(new Bolt(source));
          source.x = this.x + (14 * Math.cos(this.rad - (Math.PI * 2 / 3)));
          source.y = this.y + (14 * Math.sin(this.rad - (Math.PI * 2 / 3)));
          this.projectiles.push(new Bolt(source));
          break;
      }
      this.allowFire = false;
      gAudio.startBolt();
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
    if (this.life >= PLAYER_LIFE) {
      var xAcc = 0;
      var yAcc = 0;
      this.xVel *= FRICTION;
      this.yVel *= FRICTION;
      if (gInput.isActive('thrust')) {
        xAcc = Math.cos(this.rad);
        yAcc = Math.sin(this.rad);
      }
      else {
        if (gInput.isActive('left')) xAcc--;
        if (gInput.isActive('right')) xAcc++;
        if (gInput.isActive('up')) yAcc--;
        if (gInput.isActive('down')) yAcc++;
      }
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
      if (!collision) {
        for (var i in gScene.powerups) {
          if (gScene.powerups[i].collide()) {
            switch (gScene.powerups[i].getType()) {
              case 'bolt': if (this.firePower < 2) this.firePower++; break;
              case 'clone':
                this.hasClones = true;
                break;
              case 'missile':
                this.hasMissiles = true;
                this.missiles = MAX_SPECIALS[gScene.powerups[i].type];
                break;
              case 'beam':
                this.hasBeam = true;
                break;
              case 'shield':
                this.hasShield = true;
                break;
              case 'armor':
                this.hasArmor = true;
                break;
            }
            this.frame++;
            this.animation = 'powerup';
            gScene.powerups[i].frame++;
            break;
          }
        }
      }

      if (Math.abs(this.xVel) < 0.01) this.xVel = 0;
      if (Math.abs(this.yVel) < 0.01) this.yVel = 0;

      this.x = Math.min(Math.min(gCamera.x + SCREEN_WIDTH, gScene.bounds.r) - 20, Math.max(Math.max(gCamera.x, gScene.bounds.l) + 20, this.x + this.xVel * this.speed));
      this.y = Math.min(Math.min(gCamera.y + SCREEN_HEIGHT, gScene.bounds.b) - 20, Math.max(Math.max(gCamera.y + 24, gScene.bounds.t) + 20, this.y + this.yVel * this.speed));

      this.rad = Math.atan2(gInput.mouseY - (this.y - gCamera.y), gInput.mouseX - (this.x - gCamera.x));

      if (gInput.isActive('fire')) this.fire();
    }
    else this.life--;
    // Loop through projectiles and only keep the ones still in the level bounds
    var liveProjectiles = [];
    this.projectiles.forEach(function(p) {
      var collision;
      for (var i in gScene.targets) {
        collision = p.collide(gScene.targets[i]);
        if (collision) break;
      }
      if (!collision) {
        for (var i in gScene.shards) {
          if (p.collide(gScene.shards[i])) {
            if (gScene.shards[i].life == SHARD_LIFE) gScene.shards[i].life--;
            collision = true;
            break;
          }
        }
      }
      if (!collision && gScene.bounds.collide(p)) p.dying = true;
      p.update();
      if (p.life > 0) liveProjectiles.push(p);
    });
    this.projectiles = liveProjectiles;
  },
  render: function() {
    var opacity = decToHex(255 * (this.life / PLAYER_LIFE));

    renderPath([
      ['moveTo', PLAYER_RADIUS * Math.cos(this.rad), PLAYER_RADIUS * Math.sin(this.rad)],
      ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(this.rad + (Math.PI / 4)), (PLAYER_RADIUS / 2) * Math.sin(this.rad + (Math.PI / 4))],
      ['lineTo', PLAYER_RADIUS * Math.cos(this.rad + (Math.PI / 2)), PLAYER_RADIUS * Math.sin(this.rad + (Math.PI / 2))],
      ['lineTo', (PLAYER_RADIUS * 1.25) * Math.cos(this.rad + (Math.PI * 3 / 4)), (PLAYER_RADIUS * 1.25) * Math.sin(this.rad + (Math.PI * 3 / 4))],
      ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(this.rad + Math.PI), (PLAYER_RADIUS / 2) * Math.sin(this.rad + Math.PI)],
      ['lineTo', (PLAYER_RADIUS * 1.25) * Math.cos(this.rad - (Math.PI * 3 / 4)), (PLAYER_RADIUS * 1.25) * Math.sin(this.rad - (Math.PI * 3 / 4))],
      ['lineTo', PLAYER_RADIUS * Math.cos(this.rad - (Math.PI / 2)), PLAYER_RADIUS * Math.sin(this.rad - (Math.PI / 2))],
      ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(this.rad - (Math.PI / 4)), (PLAYER_RADIUS / 2) * Math.sin(this.rad - (Math.PI / 4))],
    ], true, this.x - gCamera.x, this.y - gCamera.y);

    fill('#4499DD' + opacity);
    stroke('#FFBB77' + opacity, 3);

    switch (this.animation) {
      case 'powerup':
        var overlay = '#FFFFFF' + decToHex(Math.min(255, 255 * ((Powerup.lifetime - this.frame) / Powerup.lifetime)));
        fill(overlay);
        stroke(overlay);
        break;
    }

    if (this.life < PLAYER_LIFE) {
      var radius = Math.log10(Math.max(1, 10 * (1 - (this.life / PLAYER_LIFE)))) * PLAYER_LIFE;
      renderPath([
        ['moveTo', 0, radius],
        ['arc', 0, 0, radius, 0, 2 * Math.PI],
        ['moveTo', 3, 7 + (2 * radius / 3)],
        ['arc', 3, 7, 2 * radius / 3, 0, 2 * Math.PI],
        ['moveTo', -12, -3 + (2 * radius / 3)],
        ['arc', -12, -3, 2 * radius / 3, 0, 2 * Math.PI],
        ['moveTo', 10, -6 + (2 * radius / 3)],
        ['arc', 10, -6, 2 * radius / 3, 0, 2 * Math.PI],
      ], true, this.x - gCamera.x, this.y - gCamera.y);
      fill(this.life < PLAYER_LIFE / 2 ? '#CCCCCC' + decToHex(255 * (this.life / (PLAYER_LIFE / 2))) : '#EEE');
    }
    if (this.frame > 0 && this.frame < Powerup.lifetime) this.frame++;
    else if (this.frame >= Powerup.lifetime) {
      this.frame = 0;
      this.animation = '';
    }
  }
}
