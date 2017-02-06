var HUD_MARKERS = [];
for (var i = 1; i < MAX_SPECIALS.length; i++) {
  var markerSpace = 96 / MAX_SPECIALS[i];
  var path = [];
  for (var x = markerSpace; x < 96; x += markerSpace) {
    path.push(['moveTo', x,  5]);
    path.push(['lineTo', x, 11]);
  }
  HUD_MARKERS.push(path);
}
var HUD_ICONS = [
  // Clone
  [
    ['moveTo', PLAYER_RADIUS / -2, 0],
    ['lineTo', (PLAYER_RADIUS / 4) * Math.cos(Math.PI * 3 / -4), (PLAYER_RADIUS / 4) * Math.sin(Math.PI * 3 / -4)],
    ['lineTo', 0, PLAYER_RADIUS / -2],
    ['lineTo', (PLAYER_RADIUS * 0.512) * Math.cos(Math.PI / -4), (PLAYER_RADIUS * 0.512) * Math.sin(Math.PI / -4)],
    ['lineTo', PLAYER_RADIUS / 4, 0],
    ['lineTo', (PLAYER_RADIUS * 0.512) * Math.cos(Math.PI / 4), (PLAYER_RADIUS * 0.512) * Math.sin(Math.PI / 4)],
    ['lineTo', 0, PLAYER_RADIUS / 2],
    ['lineTo', (PLAYER_RADIUS / 4) * Math.cos(Math.PI * 3 / 4), (PLAYER_RADIUS / 4) * Math.sin(Math.PI * 3 / 4)]
  ],
  // Missile
  [
    ['moveTo', -13,  0],
    ['lineTo',  -2,  3],
    ['lineTo',  -1,  3],
    ['lineTo',  -1,  5],
    ['lineTo',   5,  5],
    ['lineTo',   5,  4],
    ['lineTo',   2,  1],
    ['lineTo',   2, -1],
    ['lineTo',   5, -4],
    ['lineTo',   5, -5],
    ['lineTo',  -1, -5],
    ['lineTo',  -1, -3],
    ['lineTo',  -2, -3]
  ],
  // Beam
  [
    ['moveTo', -6,  6],
    ['lineTo', -6,  3],
    ['lineTo',  3, -6],
    ['lineTo',  6, -3],
    ['lineTo', -3,  6]
  ],
  // Shield
  [
    ['moveTo', 2, 0],
    ['arc', 0, 0, 2, 0, 2 * Math.PI],
    ['moveTo', 5, 0],
    ['arc', 0, 0, 5, 0, 2 * Math.PI],
    ['moveTo', 8, 0],
    ['arc', 0, 0, 8, 0, 2 * Math.PI],
  ],
  // Armor
  [
    ['moveTo', 8 * Math.cos(3 * Math.PI / 4), 8 * Math.sin(3 * Math.PI / 4)],
    ['lineTo', 3 * Math.cos(5 * Math.PI / 4), 3 * Math.sin(5 * Math.PI / 4)],
    ['lineTo', Math.cos(5 * Math.PI / 4), Math.sin(5 * Math.PI / 4)],
    ['lineTo', Math.cos(5 * Math.PI / 4), Math.sin(5 * Math.PI / 4)],
    ['lineTo', 8 * Math.cos(7 * Math.PI / 4), 8 * Math.sin(7 * Math.PI / 4)],
    ['lineTo', 3 * Math.cos(Math.PI / 4), 3 * Math.sin(Math.PI / 4)],
    ['lineTo', Math.cos(Math.PI / 4), Math.sin(Math.PI / 4)]
  ]
];

gInput = {
  w: 0,
  s: 0,
  a: 0,
  d: 0,
  ' ': 0,
  shift: 0,
  mouseL: 0,
  mouseR: 0,
  up: 'w',
  down: 's',
  left: 'a',
  right: 'd',
  cycle: 'shift',
  fire: 'mouseL',
  special: 'mouseR',
  thrust: ' ',
  mouseX: 0,
  mouseY: 0,
  rad: 0,
  rotateCursor: true,
  cursor: 'triangle',
  validKey: function(key) { return typeof this[key.toLowerCase()] === 'number' },
  press: function(key) {
    var key = key.toLowerCase();
    this[key] = 1;
    if (this.thrust == key) gPlayer.speed = THRUSTER_SPEED;
  },
  release: function(key) {
    var key = key.toLowerCase();
    this[key] = 0;
    if (this.thrust == key) gPlayer.speed = PLAYER_SPEED;
  },
  releaseAll: function() {
    this.w = 0;
    this.s = 0;
    this.a = 0;
    this.d = 0;
    this[' '] = 0;
    this.shift = 0;
    this.mouseL = 0;
    this.mouseR = 0;
  },
  mousedown: function(event) {
    if (event.button === 0) this.mouseL = 1;
    else if (event.button === 2) this.mouseR = 1;
  },
  mouseup: function(event) {
    if (event.button === 0) this.mouseL = 0;
    else if (event.button === 2) this.mouseR = 0;
  },
  mouseleave: function(event) { this.mouseL = 0; this.mouseR = 0; },
  isActive: function(command) { return typeof this[command] === 'string' && this[this[command]] === 1 },
  setMousePos: function(event) {
    this.mouseX = event.layerX * (gRenderer.context.canvas.width / canvas.offsetWidth);
    this.mouseY = event.layerY * (gRenderer.context.canvas.height / canvas.offsetHeight);
  },
  renderAmmo: function(type, x) {
    var x = typeof x === 'number' ? x : 0;
    var special, markers;
    var lineWidth = type == 4 ? 1 : 2;
    switch (type) {
      case 1: special = gPlayer.clones.length; break;
      case 2: special = gPlayer.missiles; break;
      case 3: special = gPlayer.beam; break;
      case 4: special = gPlayer.shield; break;
      case 5: special = gPlayer.armor; break;
    }
    var hasWeapon = gPlayer.hasWeapon(type);
    // Icon
    renderPath(HUD_ICONS[type - 1], type !== 4, x, 12);
    fill(hasWeapon ? POWERUP_COLORS[type][1] : '#CCC7');
    stroke(hasWeapon ? POWERUP_COLORS[type][2] : '#AAA7', lineWidth);
    x += 12;
    // Ammo Bar
    if (special > 0) {
      var path = [
        ['moveTo', 0, 8],
        ['lineTo', 3, 5]
      ];
      if (special < MAX_SPECIALS[type]) {
        var markerSpace = 96 / MAX_SPECIALS[type];
        path.push(['lineTo', markerSpace * special,  5]);
        path.push(['lineTo', markerSpace * special, 19]);
      }
      else {
        path.push(['lineTo', 93,  5]);
        path.push(['lineTo', 96,  8]);
        path.push(['lineTo', 96, 16]);
        path.push(['lineTo', 93, 19]);
      }
      path.push(['lineTo',  3, 19]);
      path.push(['lineTo',  0, 16]);
      renderPath(path, true, x);
      fill(POWERUP_COLORS[type][0]);
    }
    // Outline
    renderPath(HUD_MARKERS[type - 1], false, x);
    stroke('#000', 2);
    renderPath([
      ['moveTo',  0,  8],
      ['lineTo',  3,  5],
      ['lineTo', 93,  5],
      ['lineTo', 96,  8],
      ['lineTo', 96, 16],
      ['lineTo', 93, 19],
      ['lineTo',  3, 19],
      ['lineTo',  0, 16]
    ], true, x);
    stroke(gPlayer.special == type ? '#FFF' : '#888', 4);
    stroke('#000', 2);
  },
  render: function() {
    if (gScene.scene == 'hangar' || gScene.scene == 'game') {
      fillRect(0, 0, SCREEN_WIDTH, 24, '#000');
      // Ammo Bars
      this.renderAmmo(1,  24); // Clone Ammo
      this.renderAmmo(2, 168); // Missile Ammo
      this.renderAmmo(3, 312); // Beam Ammo
      this.renderAmmo(4, 456); // Shield Ammo
      this.renderAmmo(5, 600); // Armor Ammo
    }
    // Mouse cursor
    var rad = this.rotateCursor ? this.rad : gPlayer.rad + Math.PI;
    if (this.rotateCursor) this.rad += Math.PI / 512;

    switch (this.cursor) {
      case 'crosshairs':
        renderPath([
          ['moveTo', this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad))],
          ['lineTo', this.mouseX + (16 * Math.cos(rad)), this.mouseY + (16 * Math.sin(rad))],
          ['moveTo', this.mouseX + (4 * Math.cos(rad + (Math.PI / 2))), this.mouseY + (4 * Math.sin(rad + (Math.PI / 2)))],
          ['lineTo', this.mouseX + (16 * Math.cos(rad + (Math.PI / 2))), this.mouseY + (16 * Math.sin(rad + (Math.PI / 2)))],
          ['moveTo', this.mouseX + (4 * Math.cos(rad + Math.PI)), this.mouseY + (4 * Math.sin(rad + Math.PI))],
          ['lineTo', this.mouseX + (16 * Math.cos(rad + Math.PI)), this.mouseY + (16 * Math.sin(rad + Math.PI))],
          ['moveTo', this.mouseX + (4 * Math.cos(rad - (Math.PI / 2))), this.mouseY - (4 * Math.sin(rad + (Math.PI / 2)))],
          ['lineTo', this.mouseX + (16 * Math.cos(rad - (Math.PI / 2))), this.mouseY - (16 * Math.sin(rad + (Math.PI / 2)))],
          ['moveTo', this.mouseX + (12 * Math.cos(rad + (Math.PI / 16))), this.mouseY + (12 * Math.sin(rad + (Math.PI / 16)))],
          ['arc', this.mouseX, this.mouseY, 12, rad + (Math.PI / 16), rad + (Math.PI * 7 / 16)],
          ['moveTo', this.mouseX + (12 * Math.cos(rad + (Math.PI * 9 / 16))), this.mouseY + (12 * Math.sin(rad + (Math.PI * 9 / 16)))],
          ['arc', this.mouseX, this.mouseY, 12, rad + (Math.PI * 9 / 16), rad + (Math.PI * 15 / 16)],
          ['moveTo', this.mouseX + (12 * Math.cos(rad + (Math.PI * 17 / 16))), this.mouseY + (12 * Math.sin(rad + (Math.PI * 17 / 16)))],
          ['arc', this.mouseX, this.mouseY, 12, rad + (Math.PI * 17 / 16), rad - (Math.PI * 9 / 16)],
          ['moveTo', this.mouseX + (12 * Math.cos(rad - (Math.PI * 7 / 16))), this.mouseY + (12 * Math.sin(rad - (Math.PI * 7 / 16)))],
          ['arc', this.mouseX, this.mouseY, 12, rad - (Math.PI * 7 / 16), rad - (Math.PI / 16)],
        ]);

        stroke('#2D2', 2);
        strokeRect(this.mouseX - 0.5, this.mouseY - 0.5, 1, 1, null, 1);
        break;
      case 'triangle':
        renderPath([
          ['moveTo', this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad))],
          ['lineTo', this.mouseX + (18 * Math.cos(rad - (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad - (Math.PI / 8)))],
          ['lineTo', this.mouseX + (14 * Math.cos(rad)), this.mouseY + (14 * Math.sin(rad))],
          ['lineTo', this.mouseX + (18 * Math.cos(rad + (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad + (Math.PI / 8)))],
        ], true);
        stroke('rgba(0,0,0,0.6)', 2);
        fill('#DD2');
        rad += Math.PI * 2 / 3;
        renderPath([
          ['moveTo', this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad))],
          ['lineTo', this.mouseX + (18 * Math.cos(rad - (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad - (Math.PI / 8)))],
          ['lineTo', this.mouseX + (14 * Math.cos(rad)), this.mouseY + (14 * Math.sin(rad))],
          ['lineTo', this.mouseX + (18 * Math.cos(rad + (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad + (Math.PI / 8)))],
        ], true);
        stroke();
        fill("#2D2");
        rad += Math.PI * 2 / 3;
        renderPath([
          ['moveTo', this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad))],
          ['lineTo', this.mouseX + (18 * Math.cos(rad - (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad - (Math.PI / 8)))],
          ['lineTo', this.mouseX + (14 * Math.cos(rad)), this.mouseY + (14 * Math.sin(rad))],
          ['lineTo', this.mouseX + (18 * Math.cos(rad + (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad + (Math.PI / 8)))],
        ], true);
        stroke();
        fill();
        break;
    }
  }
}

function handleKeyDown(event) {
  if (!event.repeat && gInput.validKey(event.key)) {
    event.preventDefault();
    gInput.press(event.key);
  }
}
function handleKeyUp(event) {
  if (typeof gInput[event.key.toLowerCase()] === 'number') gInput.release(event.key);
}

function handleMouseDown(event) { gInput.mousedown(event) }
function handleMouseUp(event) { gInput.mouseup(event) }
function handleMouseMove(event) { gInput.setMousePos(event) }
function handleMouseLeave(event) { gInput.mouseleave(event) }
