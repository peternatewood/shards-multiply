gScene = {
  scene: 'title',
  level: 0,
  targets: [],
  titleX: -776,
  titleY: 128,
  title: [],
  selectProg: 0,
  inTransition: false,
  transitionProg: 0,
  change: function(scene) {
    switch (scene) {
      case 'hangar':
        this.targets = [
          new Target(144, 506),
          new Target(400, 506)
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
  update: function() {
    if (!this.inTransition) {
      switch (this.scene) {
        case 'title':
          if (gInput.isActive('fire')) this.change('hangar');
          break;
        case 'hangar':
        case 'game':
          gPlayer.update();
          break;
        case 'levelTitle':
          if (gInput.isActive('fire')) this.change('game');
          break;
      }
    }
  },
  renderTitle: function(context) {
    context.lineWidth = 4;
    context.strokeStyle = '#FD8';

    var grad = context.createLinearGradient(448, 100, 352, 278);
    grad.addColorStop(0, '#FB6');
    grad.addColorStop(1, '#B61');
    context.fillStyle = grad;

    context.beginPath();
    // Shards Multiply
    // S
    context.moveTo(this.titleX + 24, this.titleY + 0);
    context.lineTo(this.titleX + 96, this.titleY + 0);
    context.lineTo(this.titleX + 93, this.titleY + 24);
    context.lineTo(this.titleX + 34, this.titleY + 24);
    context.lineTo(this.titleX + 32, this.titleY + 36);
    context.lineTo(this.titleX + 76, this.titleY + 36);
    context.lineTo(this.titleX + 87, this.titleY + 66);
    context.lineTo(this.titleX + 73, this.titleY + 96);
    context.lineTo(this.titleX + 0, this.titleY + 96);
    context.lineTo(this.titleX + 3, this.titleY + 72);
    context.lineTo(this.titleX + 63, this.titleY + 72);
    context.lineTo(this.titleX + 65, this.titleY + 60);
    context.lineTo(this.titleX + 20, this.titleY + 60);
    context.lineTo(this.titleX + 9, this.titleY + 30);
    context.lineTo(this.titleX + 24, this.titleY + 0);
    // H
    context.moveTo(this.titleX + 108, this.titleY + 0);
    context.lineTo(this.titleX + 132, this.titleY + 0);
    context.lineTo(this.titleX + 128, this.titleY + 36);
    context.lineTo(this.titleX + 163, this.titleY + 36);
    context.lineTo(this.titleX + 168, this.titleY + 0);
    context.lineTo(this.titleX + 192, this.titleY + 0);
    context.lineTo(this.titleX + 180, this.titleY + 96);
    context.lineTo(this.titleX + 156, this.titleY + 96);
    context.lineTo(this.titleX + 160, this.titleY + 60);
    context.lineTo(this.titleX + 125, this.titleY + 60);
    context.lineTo(this.titleX + 120, this.titleY + 96);
    context.lineTo(this.titleX + 96, this.titleY + 96);
    context.lineTo(this.titleX + 108, this.titleY + 0);
    // A
    context.moveTo(this.titleX + 192, this.titleY + 96);
    context.lineTo(this.titleX + 237, this.titleY + 0);
    context.lineTo(this.titleX + 261, this.titleY + 0);
    context.lineTo(this.titleX + 282, this.titleY + 96);
    context.lineTo(this.titleX + 258, this.titleY + 96);
    context.lineTo(this.titleX + 244, this.titleY + 36);
    context.lineTo(this.titleX + 244, this.titleY + 36);
    context.lineTo(this.titleX + 216, this.titleY + 96);
    context.lineTo(this.titleX + 192, this.titleY + 96);
    // R
    context.moveTo(this.titleX + 306, this.titleY + 0);
    context.lineTo(this.titleX + 378, this.titleY + 0);
    context.lineTo(this.titleX + 387, this.titleY + 24);
    context.lineTo(this.titleX + 370, this.titleY + 50);
    context.lineTo(this.titleX + 355, this.titleY + 50);
    context.lineTo(this.titleX + 381, this.titleY + 96);
    context.lineTo(this.titleX + 357, this.titleY + 96);
    context.lineTo(this.titleX + 334, this.titleY + 50);
    context.lineTo(this.titleX + 336, this.titleY + 36);
    context.lineTo(this.titleX + 363, this.titleY + 36);
    context.lineTo(this.titleX + 365, this.titleY + 24);
    context.lineTo(this.titleX + 330, this.titleY + 24);
    context.lineTo(this.titleX + 318, this.titleY + 96);
    context.lineTo(this.titleX + 294, this.titleY + 96);
    context.lineTo(this.titleX + 306, this.titleY + 0);
    // D
    context.moveTo(this.titleX + 402, this.titleY + 0);
    context.lineTo(this.titleX + 462, this.titleY + 0);
    context.lineTo(this.titleX + 483, this.titleY + 24);
    context.lineTo(this.titleX + 477, this.titleY + 72);
    context.lineTo(this.titleX + 450, this.titleY + 96);
    context.lineTo(this.titleX + 423, this.titleY + 96);
    context.lineTo(this.titleX + 428, this.titleY + 60);
    context.lineTo(this.titleX + 459, this.titleY + 60);
    context.lineTo(this.titleX + 462, this.titleY + 36);
    context.lineTo(this.titleX + 423, this.titleY + 36);
    context.lineTo(this.titleX + 414, this.titleY + 96);
    context.lineTo(this.titleX + 390, this.titleY + 96);
    context.lineTo(this.titleX + 402, this.titleY + 0);
    // S
    context.moveTo(this.titleX + 511, this.titleY + 0);
    context.lineTo(this.titleX + 583, this.titleY + 0);
    context.lineTo(this.titleX + 580, this.titleY + 24);
    context.lineTo(this.titleX + 521, this.titleY + 24);
    context.lineTo(this.titleX + 519, this.titleY + 36);
    context.lineTo(this.titleX + 563, this.titleY + 36);
    context.lineTo(this.titleX + 574, this.titleY + 66);
    context.lineTo(this.titleX + 560, this.titleY + 96);
    context.lineTo(this.titleX + 487, this.titleY + 96);
    context.lineTo(this.titleX + 489, this.titleY + 72);
    context.lineTo(this.titleX + 550, this.titleY + 72);
    context.lineTo(this.titleX + 552, this.titleY + 60);
    context.lineTo(this.titleX + 507, this.titleY + 60);
    context.lineTo(this.titleX + 496, this.titleY + 30);
    context.lineTo(this.titleX + 511, this.titleY + 0);
    // M
    context.moveTo(this.titleX + 12, this.titleY + 112);
    context.lineTo(this.titleX + 36, this.titleY + 112);
    context.lineTo(this.titleX + 50, this.titleY + 148);
    context.lineTo(this.titleX + 72, this.titleY + 112);
    context.lineTo(this.titleX + 96, this.titleY + 112);
    context.lineTo(this.titleX + 84, this.titleY + 208);
    context.lineTo(this.titleX + 60, this.titleY + 208);
    context.lineTo(this.titleX + 64, this.titleY + 172);
    context.lineTo(this.titleX + 46, this.titleY + 184);
    context.lineTo(this.titleX + 30, this.titleY + 172);
    context.lineTo(this.titleX + 24, this.titleY + 208);
    context.lineTo(this.titleX + 0, this.titleY + 208);
    context.lineTo(this.titleX + 12, this.titleY + 112);
    // U
    context.moveTo(this.titleX + 108, this.titleY + 112);
    context.lineTo(this.titleX + 132, this.titleY + 112);
    context.lineTo(this.titleX + 126, this.titleY + 168);
    context.lineTo(this.titleX + 136, this.titleY + 186);
    context.lineTo(this.titleX + 160, this.titleY + 166);
    context.lineTo(this.titleX + 168, this.titleY + 112);
    context.lineTo(this.titleX + 192, this.titleY + 112);
    context.lineTo(this.titleX + 180, this.titleY + 208);
    context.lineTo(this.titleX + 156, this.titleY + 208);
    context.lineTo(this.titleX + 157, this.titleY + 196);
    context.lineTo(this.titleX + 144, this.titleY + 208);
    context.lineTo(this.titleX + 120, this.titleY + 208);
    context.lineTo(this.titleX + 99, this.titleY + 184);
    context.lineTo(this.titleX + 108, this.titleY + 112);
    // L
    context.moveTo(this.titleX + 204, this.titleY + 112);
    context.lineTo(this.titleX + 228, this.titleY + 112);
    context.lineTo(this.titleX + 219, this.titleY + 184);
    context.lineTo(this.titleX + 264, this.titleY + 184);
    context.lineTo(this.titleX + 261, this.titleY + 208);
    context.lineTo(this.titleX + 192, this.titleY + 208);
    context.lineTo(this.titleX + 204, this.titleY + 112);
    // T
    context.moveTo(this.titleX + 250, this.titleY + 112);
    context.lineTo(this.titleX + 345, this.titleY + 112);
    context.lineTo(this.titleX + 342, this.titleY + 136);
    context.lineTo(this.titleX + 305, this.titleY + 136);
    context.lineTo(this.titleX + 296, this.titleY + 208);
    context.lineTo(this.titleX + 272, this.titleY + 208);
    context.lineTo(this.titleX + 281, this.titleY + 136);
    context.lineTo(this.titleX + 247, this.titleY + 136);
    context.lineTo(this.titleX + 250, this.titleY + 112);
    // I
    context.moveTo(this.titleX + 357, this.titleY + 112);
    context.lineTo(this.titleX + 417, this.titleY + 112);
    context.lineTo(this.titleX + 414, this.titleY + 136);
    context.lineTo(this.titleX + 378, this.titleY + 136);
    context.lineTo(this.titleX + 372, this.titleY + 184);
    context.lineTo(this.titleX + 408, this.titleY + 184);
    context.lineTo(this.titleX + 405, this.titleY + 208);
    context.lineTo(this.titleX + 308, this.titleY + 208);
    context.lineTo(this.titleX + 311, this.titleY + 184);
    context.lineTo(this.titleX + 348, this.titleY + 184);
    context.lineTo(this.titleX + 357, this.titleY + 112);
    // P
    context.moveTo(this.titleX + 429, this.titleY + 112);
    context.lineTo(this.titleX + 501, this.titleY + 112);
    context.lineTo(this.titleX + 510, this.titleY + 137);
    context.lineTo(this.titleX + 493, this.titleY + 162);
    context.lineTo(this.titleX + 457, this.titleY + 162);
    context.lineTo(this.titleX + 459, this.titleY + 148);
    context.lineTo(this.titleX + 486, this.titleY + 148);
    context.lineTo(this.titleX + 488, this.titleY + 136);
    context.lineTo(this.titleX + 453, this.titleY + 136);
    context.lineTo(this.titleX + 441, this.titleY + 208);
    context.lineTo(this.titleX + 417, this.titleY + 208);
    context.lineTo(this.titleX + 429, this.titleY + 112);
    // L
    context.moveTo(this.titleX + 526, this.titleY + 112);
    context.lineTo(this.titleX + 550, this.titleY + 112);
    context.lineTo(this.titleX + 543, this.titleY + 184);
    context.lineTo(this.titleX + 586, this.titleY + 184);
    context.lineTo(this.titleX + 583, this.titleY + 208);
    context.lineTo(this.titleX + 514, this.titleY + 208);
    context.lineTo(this.titleX + 526, this.titleY + 112);
    // Y
    context.moveTo(this.titleX + 583, this.titleY + 112);
    context.lineTo(this.titleX + 607, this.titleY + 112);
    context.lineTo(this.titleX + 604, this.titleY + 136);
    context.lineTo(this.titleX + 634, this.titleY + 148);
    context.lineTo(this.titleX + 639, this.titleY + 112);
    context.lineTo(this.titleX + 665, this.titleY + 112);
    context.lineTo(this.titleX + 653, this.titleY + 208);
    context.lineTo(this.titleX + 595, this.titleY + 208);
    context.lineTo(this.titleX + 598, this.titleY + 184);
    context.lineTo(this.titleX + 627, this.titleY + 184);
    context.lineTo(this.titleX + 629, this.titleY + 174);
    context.lineTo(this.titleX + 578, this.titleY + 148);
    context.lineTo(this.titleX + 583, this.titleY + 112);

    context.stroke();
    context.fill();
    context.closePath();

    if (this.titleX < 56) this.titleX += 52;
    else {
      context.textAlign = 'center';
      context.strokeStyle = '#FED';
      context.lineWidth = 2;
      context.strokeText('Click mouse button to start', 400, this.titleY + 260);
    }
  },
  renderLevelTitle: function(context) {
    context.font = '32px courier';
    context.fillStyle = '#FFF';
    context.fillText('Level ' + this.level, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    context.strokeStyle = '#FFF';
    context.lineWidth = 2;
    context.strokeText('Level ' + this.level, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  },
  renderTransition: function(context) {
    context.fillStyle = 'rgba(0, 0, 0, ' + (this.transitionProg / 32) + ')';
    context.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    this.transitionProg++;
    if (this.transitionProg >= 32) {
      this.change('levelTitle');
      this.inTransition = false;
      this.transitionProg = 0;
      this.selectProg = 0;
    }
  },
  renderBack: function(context) {
    context.beginPath();
    context.moveTo(72, 24);
    context.lineTo(728, 24);
    context.lineTo(752, 48);
    context.lineTo(752, 552);
    context.lineTo(728, 576);
    context.lineTo(72, 576);
    context.lineTo(48, 552);
    context.lineTo(48, 48);
    context.closePath();
    context.strokeStyle = '#444';
    context.stroke();
    context.fillStyle = "#777";
    context.fill();

    this.renderCommand('fire', 180, 160, context);
    this.renderCommand('up', 398, 160, context);
    this.renderCommand('down', 398, 372, context);
    this.renderCommand('left', 180, 372, context);
    this.renderCommand('right', 616, 372, context);
    // Start square
    if (gScene.scene == 'hangar') {
      if (this.selectProg < 32) {
        if (gPlayer.x > 544 && gPlayer.x < 688 && gPlayer.y > 64 && gPlayer.y < 208) this.selectProg++;
        else if (this.selectProg > 0) this.selectProg = Math.max(0, this.selectProg - 2);
      }
      if (!this.inTransition && this.selectProg == 32) this.transition(1);

      context.beginPath();
      context.moveTo(682, 64);
      context.lineTo(688, 70);
      context.lineTo(688, 70 + (132 * Math.min(1, this.selectProg / 16)));
      if (this.selectProg >= 16) {
        context.lineTo(682, 208);
        context.lineTo(682 - (132 * Math.min(1, (this.selectProg - 16) / 16)), 208);
        context.lineTo(544, 70 + (132 * Math.min(1, (this.selectProg - 16) / 16)));
        context.lineTo(544, 70);
      }
      context.lineTo(682 - (132 * Math.min(1, this.selectProg / 16)), 64);
      context.closePath();
      context.fillStyle = '#4B4';
      context.fill();
    }
    context.beginPath();
    context.moveTo(544, 70);
    context.lineTo(550, 64);
    context.lineTo(682, 64);
    context.lineTo(688, 70);
    context.lineTo(688, 202);
    context.lineTo(682, 208);
    context.lineTo(550, 208);
    context.lineTo(544, 202);
    context.closePath();
    context.strokeStyle = '#444';
    context.lineWidth = 6;
    context.stroke();
    context.strokeStyle = '#000';
    context.lineWidth = 2;
    context.stroke();
    // Here to Start
    context.font = '24px helvetica';
    context.textBaseline = 'middle';
    context.strokeStyle = '#333';
    context.fillStyle = '#000';
    context.lineWidth = 2;
    context.strokeText('HERE', 616, 112);
    context.fillText('HERE', 616, 112);
    context.strokeText('TO', 616, 141);
    context.fillText('TO', 616, 141);
    context.strokeText('START', 616, 170);
    context.fillText('START', 616, 170);

    context.beginPath();
    context.moveTo(108, 476);
    context.lineTo(116, 468);
    context.lineTo(684, 468);
    context.lineTo(692, 476);
    context.lineTo(692, 534);
    context.lineTo(684, 542);
    context.lineTo(116, 542);
    context.lineTo(108, 534);
    context.closePath();
    context.strokeStyle = '#444';
    context.lineWidth = 6;
    context.stroke();
    context.strokeStyle = '#000';
    context.lineWidth = 2;
    context.stroke();
  },
  renderKey: function(command, x, y, context) {
    var offset = gInput.isActive(command) ? 3 : 0;

    context.strokeText(gInput[command].toUpperCase(), x + offset, y + 12 + offset);
    context.fillText(gInput[command].toUpperCase(), x + offset, y + 12 + offset);
    if (gInput.isActive(command)) {
      context.moveTo(x - 17, y + 30);
      context.lineTo(x - 20, y + 27);
      context.lineTo(x - 20, y - 7);
      context.lineTo(x - 17, y - 10);
      context.lineTo(x + 17, y - 10);
      context.lineTo(x + 20, y - 7);
      context.lineTo(x + 20, y - 4);
      context.lineTo(x - 11, y - 4);
      context.lineTo(x - 14, y - 1);
      context.lineTo(x - 14, y + 30);
      context.closePath();
      context.fillStyle = '#222';
      context.fill();
    }

    context.beginPath();
    context.moveTo(x - 17, y + 30);
    context.lineTo(x - 20, y + 27);
    context.lineTo(x - 20, y - 7);
    context.lineTo(x - 17, y - 10);
    context.lineTo(x + 17, y - 10);
    context.lineTo(x + 20, y - 7);
    context.lineTo(x + 20, y + 27);
    context.lineTo(x + 17, y + 30);
    context.closePath();
    context.strokeStyle = '#000';
    context.lineWidth = 3;
    context.stroke();
  },
  renderCommand: function(command, x, y, context) {
    context.beginPath();
    context.moveTo(x - 72, y - 90);
    context.lineTo(x - 66, y - 96);
    context.lineTo(x + 66, y - 96);
    context.lineTo(x + 72, y - 90);
    context.lineTo(x + 72, y + 42);
    context.lineTo(x + 66, y + 48);
    context.lineTo(x - 66, y + 48);
    context.lineTo(x - 72, y + 42);
    context.closePath();
    context.strokeStyle = '#444';
    context.lineWidth = 6;
    context.stroke();
    context.strokeStyle = '#000';
    context.lineWidth = 2;
    context.stroke();

    context.font = '24px helvetica';
    context.textBaseline = 'middle';
    context.fillStyle = '#000';
    context.strokeStyle = '#333';
    context.lineWidth = 2;
    context.strokeText(command.toUpperCase(), x, y - 48);
    context.fillText(command.toUpperCase(), x, y - 48);
    context.beginPath();
    switch (gInput[command]) {
      case 'mouseL':
        context.moveTo(x - 24, y + 9);
        context.lineTo(x - 24, y - 18);
        context.lineTo(x, y - 24);
        context.lineTo(x, y);
        context.lineTo(x - 24, y - 3);
        if (gInput.isActive(command)) {
          context.fillStyle = '#444';
          context.fill();
        }
        context.moveTo(x, y - 24);
        context.lineTo(x + 24, y - 18);
        context.lineTo(x + 24, y + 9);
        context.strokeStyle = '#444';
        context.lineWidth = 4;
        context.stroke();
        context.strokeStyle = '#000';
        context.lineWidth = 2;
        context.stroke();
        context.closePath();
        break;
      case 'mouseR':
        context.moveTo(x + 24, y + 9);
        context.lineTo(x + 24, y - 18);
        context.lineTo(x, y - 24);
        context.lineTo(x, y);
        context.lineTo(x + 24, y - 3);
        if (gInput.isActive(command)) {
          context.fillStyle = '#444';
          context.fill();
        }
        context.moveTo(x, y - 24);
        context.lineTo(x - 24, y - 18);
        context.lineTo(x - 24, y + 9);
        context.strokeStyle = '#444';
        context.lineWidth = 4;
        context.stroke();
        context.strokeStyle = '#000';
        context.lineWidth = 2;
        context.stroke();
        context.closePath();
        break;
      default:
        this.renderKey(command, x, y, context);
        break;
    }
  },
  renderFore: function(context) {
    this.targets.forEach(function(t) { t.render(context) });
  }
}
