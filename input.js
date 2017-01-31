gInput = {
  w: 0,
  s: 0,
  a: 0,
  d: 0,
  ' ': 0,
  mouseL: 0,
  mouseR: 0,
  up: 'w',
  down: 's',
  left: 'a',
  right: 'd',
  fire: 'mouseL',
  thrust: ' ',
  mouseX: 0,
  mouseY: 0,
  rad: 0,
  rotateCursor: true,
  cursor: 'triangle',
  validKey: function(key) { return typeof this[key.toLowerCase()] === 'number' },
  press: function(key) { this[key.toLowerCase()] = 1; },
  release: function(key) { this[key.toLowerCase()] = 0; },
  releaseAll: function() {
    this.w = 0;
    this.s = 0;
    this.a = 0;
    this.d = 0;
    this[' '] = 0;
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
  render: function() {
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
