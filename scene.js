gScene = {
  scene: 'title',
  level: 0,
  targets: [],
  shards: [new Shard(40, 40, 0)],
  titleX: -812,
  titleY: 192,
  title: [],
  selectProg: 0,
  inTransition: false,
  transitionProg: 0,
  grad: createLinearGradient(448, 100, 352, 278, ['#FB6', '#B61']),
  bounds: {
    l: 0, t: 0, r: 704, b: 552,
    collide: function(p) {
      if (p.x - p.size < this.l || p.y - p.size < this.t || p.x + p.size > this.r || p.y + p.size > this.b) return true;
      return false;
    }
  },
  change: function(scene) {
    gInput.releaseAll();
    switch (scene) {
      case 'hangar':
        this.setBounds();
        this.targets = [
          new Target(this.bounds.l + 102, this.bounds.t + 485),
          new Target(this.bounds.l + 362, this.bounds.t + 485)
        ];
      case 'game':
      case 'levelTitle':
        gInput.rotateCursor = false;
        break;
      case 'title':
        gInput.rotateCursor = true;
        break;
      default: return;
    }
    this.scene = scene;
  },
  transition: function(levelNum) {
    this.inTransition = true;
    this.level = levelNum;
  },
  setBounds: function() {
    if (LEVELS[this.level]) {
      this.bounds.l = LEVELS[this.level].l;
      this.bounds.t = LEVELS[this.level].t;
      this.bounds.r = LEVELS[this.level].r;
      this.bounds.b = LEVELS[this.level].b;
    }
  },
  update: function() {
    if (!this.inTransition) {
      switch (this.scene) {
        case 'title':
          if (gInput.isActive('fire')) this.change('hangar');
          break;
        case 'game':
          gCamera.update();
          var liveShards = [];
          this.shards.forEach(function(s) {
            s.update();
            if (gCamera.isInView(s)) s.render();
            if (s.life > 0) liveShards.push(s);
          });
          this.shards = liveShards;
        case 'hangar':
          gPlayer.update();
          break;
        case 'levelTitle':
          if (gInput.isActive('fire')) this.change('game');
          break;
      }
    }
  },
  renderTitle: function() {
    // Shards Multiply
    renderPath([
      // S
      ['moveTo', 24, 0],
      ['lineTo', 96, 0],
      ['lineTo', 93, 24],
      ['lineTo', 34, 24],
      ['lineTo', 32, 36],
      ['lineTo', 76, 36],
      ['lineTo', 87, 66],
      ['lineTo', 73, 96],
      ['lineTo', 0, 96],
      ['lineTo', 3, 72],
      ['lineTo', 63, 72],
      ['lineTo', 65, 60],
      ['lineTo', 20, 60],
      ['lineTo', 9, 30],
      ['lineTo', 24, 0],
      // H
      ['moveTo', 108, 0],
      ['lineTo', 132, 0],
      ['lineTo', 128, 36],
      ['lineTo', 163, 36],
      ['lineTo', 168, 0],
      ['lineTo', 192, 0],
      ['lineTo', 180, 96],
      ['lineTo', 156, 96],
      ['lineTo', 160, 60],
      ['lineTo', 125, 60],
      ['lineTo', 120, 96],
      ['lineTo', 96, 96],
      ['lineTo', 108, 0],
      // A
      ['moveTo', 192, 96],
      ['lineTo', 237, 0],
      ['lineTo', 261, 0],
      ['lineTo', 282, 96],
      ['lineTo', 258, 96],
      ['lineTo', 244, 36],
      ['lineTo', 244, 36],
      ['lineTo', 216, 96],
      ['lineTo', 192, 96],
      // R
      ['moveTo', 306, 0],
      ['lineTo', 378, 0],
      ['lineTo', 387, 24],
      ['lineTo', 370, 50],
      ['lineTo', 355, 50],
      ['lineTo', 381, 96],
      ['lineTo', 357, 96],
      ['lineTo', 334, 50],
      ['lineTo', 336, 36],
      ['lineTo', 363, 36],
      ['lineTo', 365, 24],
      ['lineTo', 330, 24],
      ['lineTo', 318, 96],
      ['lineTo', 294, 96],
      ['lineTo', 306, 0],
      // D
      ['moveTo', 402, 0],
      ['lineTo', 462, 0],
      ['lineTo', 483, 24],
      ['lineTo', 477, 72],
      ['lineTo', 450, 96],
      ['lineTo', 423, 96],
      ['lineTo', 428, 60],
      ['lineTo', 459, 60],
      ['lineTo', 462, 36],
      ['lineTo', 423, 36],
      ['lineTo', 414, 96],
      ['lineTo', 390, 96],
      ['lineTo', 402, 0],
      // S
      ['moveTo', 511, 0],
      ['lineTo', 583, 0],
      ['lineTo', 580, 24],
      ['lineTo', 521, 24],
      ['lineTo', 519, 36],
      ['lineTo', 563, 36],
      ['lineTo', 574, 66],
      ['lineTo', 560, 96],
      ['lineTo', 487, 96],
      ['lineTo', 489, 72],
      ['lineTo', 550, 72],
      ['lineTo', 552, 60],
      ['lineTo', 507, 60],
      ['lineTo', 496, 30],
      ['lineTo', 511, 0],
      // M
      ['moveTo', 12, 112],
      ['lineTo', 36, 112],
      ['lineTo', 50, 148],
      ['lineTo', 72, 112],
      ['lineTo', 96, 112],
      ['lineTo', 84, 208],
      ['lineTo', 60, 208],
      ['lineTo', 64, 172],
      ['lineTo', 46, 184],
      ['lineTo', 30, 172],
      ['lineTo', 24, 208],
      ['lineTo', 0, 208],
      ['lineTo', 12, 112],
      // U
      ['moveTo', 108, 112],
      ['lineTo', 132, 112],
      ['lineTo', 126, 168],
      ['lineTo', 136, 186],
      ['lineTo', 160, 166],
      ['lineTo', 168, 112],
      ['lineTo', 192, 112],
      ['lineTo', 180, 208],
      ['lineTo', 156, 208],
      ['lineTo', 157, 196],
      ['lineTo', 144, 208],
      ['lineTo', 120, 208],
      ['lineTo', 99, 184],
      ['lineTo', 108, 112],
      // L
      ['moveTo', 204, 112],
      ['lineTo', 228, 112],
      ['lineTo', 219, 184],
      ['lineTo', 264, 184],
      ['lineTo', 261, 208],
      ['lineTo', 192, 208],
      ['lineTo', 204, 112],
      // T
      ['moveTo', 250, 112],
      ['lineTo', 345, 112],
      ['lineTo', 342, 136],
      ['lineTo', 305, 136],
      ['lineTo', 296, 208],
      ['lineTo', 272, 208],
      ['lineTo', 281, 136],
      ['lineTo', 247, 136],
      ['lineTo', 250, 112],
      // I
      ['moveTo', 357, 112],
      ['lineTo', 417, 112],
      ['lineTo', 414, 136],
      ['lineTo', 378, 136],
      ['lineTo', 372, 184],
      ['lineTo', 408, 184],
      ['lineTo', 405, 208],
      ['lineTo', 308, 208],
      ['lineTo', 311, 184],
      ['lineTo', 348, 184],
      ['lineTo', 357, 112],
      // P
      ['moveTo', 429, 112],
      ['lineTo', 501, 112],
      ['lineTo', 510, 137],
      ['lineTo', 493, 162],
      ['lineTo', 457, 162],
      ['lineTo', 459, 148],
      ['lineTo', 486, 148],
      ['lineTo', 488, 136],
      ['lineTo', 453, 136],
      ['lineTo', 441, 208],
      ['lineTo', 417, 208],
      ['lineTo', 429, 112],
      // L
      ['moveTo', 526, 112],
      ['lineTo', 550, 112],
      ['lineTo', 543, 184],
      ['lineTo', 586, 184],
      ['lineTo', 583, 208],
      ['lineTo', 514, 208],
      ['lineTo', 526, 112],
      // Y
      ['moveTo', 583, 112],
      ['lineTo', 607, 112],
      ['lineTo', 604, 136],
      ['lineTo', 634, 148],
      ['lineTo', 639, 112],
      ['lineTo', 665, 112],
      ['lineTo', 653, 208],
      ['lineTo', 595, 208],
      ['lineTo', 598, 184],
      ['lineTo', 627, 184],
      ['lineTo', 629, 174],
      ['lineTo', 578, 148],
      ['lineTo', 583, 112],
    ], false, this.titleX, this.titleY);

    stroke('#FD8', 4);
    fill(this.grad);

    if (this.titleX < 176) this.titleX += 52;
    else {
      strokeText('Click mouse button to start', SCREEN_WIDTH / 2, this.titleY + 260, '#FED', 2, { font: '32px courier', textAlign: 'center' });
      fillText('Click mouse button to start', SCREEN_WIDTH / 2, this.titleY + 260, '#FED');
    }
  },
  renderLevelTitle: function() {
    fillText('Level ' + this.level, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, '#FFF', { font: '32px courier', textAlign: 'center' });
    strokeText('Level ' + this.level, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, '#FFF', 2);
  },
  renderTransition: function() {
    fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 'rgba(0, 0, 0, ' + (this.transitionProg / 32) + ')');
    this.transitionProg++;
    if (this.transitionProg >= 32) {
      this.change('levelTitle');
      this.inTransition = false;
      this.transitionProg = 0;
      this.selectProg = 0;
      this.level = 1;
      this.setBounds();
    }
  },
  renderBack: function() {
    if (gScene.scene == 'hangar') {
      // Floor
      renderPath([
        ['moveTo', this.bounds.l + 24, this.bounds.t],
        ['lineTo', this.bounds.r - 24, this.bounds.t],
        ['lineTo', this.bounds.r, this.bounds.t + 24],
        ['lineTo', this.bounds.r, this.bounds.b - 24],
        ['lineTo', this.bounds.r - 24, this.bounds.b],
        ['lineTo', this.bounds.l + 24, this.bounds.b],
        ['lineTo', this.bounds.l, this.bounds.b - 24],
        ['lineTo', this.bounds.l, this.bounds.t + 24],
      ], true);
      stroke('#444');
      fill("#777");

      this.renderCommand('fire', this.bounds.l + 138, this.bounds.t + 136);
      this.renderCommand('up', this.bounds.l + 356, this.bounds.t + 136);
      this.renderCommand('down', this.bounds.l + 356, this.bounds.t + 348);
      this.renderCommand('left', this.bounds.l + 138, this.bounds.t + 348);
      this.renderCommand('right', this.bounds.l + 574, this.bounds.t + 348);
      // Start square
      if (this.selectProg < 32) {
        if (gPlayer.x > this.bounds.l + 508 && gPlayer.x < this.bounds.l + 640 && gPlayer.y > this.bounds.t + 46 && gPlayer.y < this.bounds.t + 178) this.selectProg++;
        else if (this.selectProg > 0) this.selectProg = Math.max(0, this.selectProg - 2);
      }
      if (!this.inTransition && this.selectProg == 32) this.transition(1);
      // Selection overlay
      var path = [
        ['moveTo', 640, 40],
        ['lineTo', 646, 46],
        ['lineTo', 646, 46 + (132 * Math.min(1, this.selectProg / 16))],
      ];
      if (this.selectProg >= 16) {
        path.push(['lineTo', 646, 184]);
        path.push(['lineTo', 640 - (132 * Math.min(1, (this.selectProg - 16) / 16)), 184]);
        path.push(['lineTo', 502, 46 + (132 * Math.min(1, (this.selectProg - 16) / 16))]);
        path.push(['lineTo', 502, 46]);
      }
      path.push(['lineTo', 640 - (132 * Math.min(1, this.selectProg / 16)), 40]);
      renderPath(path, true, this.bounds.l, this.bounds.t);
      fill('#4B4');
      // Start square outline
      renderPath([
        ['moveTo', 502, 46],
        ['lineTo', 508, 40],
        ['lineTo', 640, 40],
        ['lineTo', 646, 46],
        ['lineTo', 646, 178],
        ['lineTo', 640, 184],
        ['lineTo', 508, 184],
        ['lineTo', 502, 178],
      ], true, this.bounds.l, this.bounds.t);
      stroke('#444', 6);
      stroke('#000', 2);
      // Here to Start
      strokeText('HERE', this.bounds.l + 574, this.bounds.t + 88, '#333', 2, { font: '24px helvetica', textAlign: 'center', textBaseline: 'middle' });
      fillText('HERE', this.bounds.l + 574, this.bounds.t + 88, '#000');
      strokeText('TO', this.bounds.l + 574, this.bounds.t + 117);
      fillText('TO', this.bounds.l + 574, this.bounds.t + 117);
      strokeText('START', this.bounds.l + 574, this.bounds.t + 146);
      fillText('START', this.bounds.l + 574, this.bounds.t + 146);

      renderPath([
        ['moveTo', 66, 456],
        ['lineTo', 72, 448],
        ['lineTo', 640, 448],
        ['lineTo', 646, 456],
        ['lineTo', 646, 514],
        ['lineTo', 640, 522],
        ['lineTo', 72, 522],
        ['lineTo', 66, 514],
      ], true, this.bounds.l, this.bounds.t);
      stroke('#444', 6);
      stroke('#000', 2);
    }
    else {
      switch (this.level) {
        case 1:
          for (var x = this.bounds.l; x < this.bounds.r; x += 64) {
            for (var y = this.bounds.t; y < this.bounds.b; y += 64) {
              if (gCamera.isInView({ x: x, y: y, size: 64 })) {
                fillRect(x - gCamera.x + 2, y - gCamera.y + 12, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 8, y - gCamera.y + 4, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 14, y - gCamera.y + 16, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 28, y - gCamera.y + 48, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 24, y - gCamera.y + 28, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 32, y - gCamera.y + 34, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 51, y - gCamera.y + 8, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 7, y - gCamera.y + 31, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 62, y - gCamera.y + 4, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 35, y - gCamera.y + 53, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 44, y - gCamera.y + 60, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 61, y - gCamera.y + 33, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 21, y - gCamera.y + 61, 1, 1, '#FFF');
                fillRect(x - gCamera.x + 60, y - gCamera.y + 55, 1, 1, '#FFF');
              }
            }
          }
          break;
      }
    }
  },
  renderKey: function(command, x, y) {
    var offset = gInput.isActive(command) ? 3 : 0;

    strokeText(gInput[command].toUpperCase(), x + offset, y + 12 + offset);
    fillText(gInput[command].toUpperCase(), x + offset, y + 12 + offset);
    if (gInput.isActive(command)) {
      renderPath([
        ['moveTo', x - 17, y + 30],
        ['lineTo', x - 20, y + 27],
        ['lineTo', x - 20, y - 7],
        ['lineTo', x - 17, y - 10],
        ['lineTo', x + 17, y - 10],
        ['lineTo', x + 20, y - 7],
        ['lineTo', x + 20, y - 4],
        ['lineTo', x - 11, y - 4],
        ['lineTo', x - 14, y - 1],
        ['lineTo', x - 14, y + 30],
      ], true);
      fill('#222');
    }

    renderPath([
      ['moveTo', x - 17, y + 30],
      ['lineTo', x - 20, y + 27],
      ['lineTo', x - 20, y - 7],
      ['lineTo', x - 17, y - 10],
      ['lineTo', x + 17, y - 10],
      ['lineTo', x + 20, y - 7],
      ['lineTo', x + 20, y + 27],
      ['lineTo', x + 17, y + 30],
    ], true);
    stroke('#000', 3);
  },
  renderCommand: function(command, x, y) {
    renderPath([
      ['moveTo', x - 72, y - 90],
      ['lineTo', x - 66, y - 96],
      ['lineTo', x + 66, y - 96],
      ['lineTo', x + 72, y - 90],
      ['lineTo', x + 72, y + 42],
      ['lineTo', x + 66, y + 48],
      ['lineTo', x - 66, y + 48],
      ['lineTo', x - 72, y + 42],
    ], true);
    stroke('#444', 6);
    stroke('#000', 2);

    strokeText(command.toUpperCase(), x, y - 48, '#333', 2, { font: '24px helvetica', textBaseline: 'middle', textAlign: 'center' });
    fillText(command.toUpperCase(), x, y - 48, '#000');
    switch (gInput[command]) {
      case 'mouseL':
        renderPath([
          ['moveTo', x - 24, y + 9],
          ['lineTo', x - 24, y - 18],
          ['lineTo', x, y - 24],
          ['lineTo', x, y],
          ['lineTo', x - 24, y - 3],
        ]);
        if (gInput.isActive(command)) fill('#444');
        stroke('#444', 4);
        stroke('#000', 2);

        renderPath([
          ['moveTo', x, y - 24],
          ['lineTo', x + 24, y - 18],
          ['lineTo', x + 24, y + 9],
        ]);
        stroke('#444', 4);
        stroke('#000', 2);
        break;
      case 'mouseR':
        renderPath([
          ['moveTo', x + 24, y + 9],
          ['lineTo', x + 24, y - 18],
          ['lineTo', x, y - 24],
          ['lineTo', x, y],
          ['lineTo', x + 24, y - 3],
        ]);
        if (gInput.isActive(command)) fill('#444');
        stroke('#444', 4);
        stroke('#000', 4);

        renderPath([
          ['moveTo', x, y - 24],
          ['lineTo', x - 24, y - 18],
          ['lineTo', x - 24, y + 9],
        ]);
        stroke('#444', 4);
        stroke('#000', 4);
        break;
      default:
        this.renderKey(command, x, y);
        break;
    }
  },
  renderFore: function() {
    this.targets.forEach(function(t) { t.render() });
    if (this.scene == 'game') this.shards.forEach(function(s) { if (gCamera.isInView(s)) s.render(); });
  }
}
