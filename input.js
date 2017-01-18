gInput = {
  w: 0,
  s: 0,
  a: 0,
  d: 0,
  mouseL: 0,
  mouseR: 0,
  up: 'w',
  down: 's',
  left: 'a',
  right: 'd',
  fire: 'mouseL',
  mouseX: 0,
  mouseY: 0,
  rad: 0,
  rotateCursor: true,
  cursor: 'triangle',
  validKey: function(key) { return typeof this[key.toLowerCase()] === 'number' },
  press: function(key) { this[key.toLowerCase()] = 1; },
  release: function(key) { this[key.toLowerCase()] = 0; },
  mousedown: function(event) {
    if (event.button === 0) this.mouseL = 1;
    else if (event.button === 2) this.mouseR = 1;
  },
  mouseup: function(event) {
    if (event.button === 0) this.mouseL = 0;
    else if (event.button === 2) this.mouseR = 0;
  },
  isActive: function(command) { return typeof this[command] === 'string' && this[this[command]] === 1 },
  setMousePos: function(event) { this.mouseX = event.layerX; this.mouseY = event.layerY },
  render: function(context) {
    var rad = this.rotateCursor ? this.rad : gPlayer.rad + Math.PI;
    if (this.rotateCursor) this.rad += Math.PI / 512;

    context.beginPath();
    switch (this.cursor) {
      case 'crosshairs':
        context.moveTo(this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad)));
        context.lineTo(this.mouseX + (16 * Math.cos(rad)), this.mouseY + (16 * Math.sin(rad)));
        context.moveTo(this.mouseX + (4 * Math.cos(rad + (Math.PI / 2))), this.mouseY + (4 * Math.sin(rad + (Math.PI / 2))));
        context.lineTo(this.mouseX + (16 * Math.cos(rad + (Math.PI / 2))), this.mouseY + (16 * Math.sin(rad + (Math.PI / 2))));
        context.moveTo(this.mouseX + (4 * Math.cos(rad + Math.PI)), this.mouseY + (4 * Math.sin(rad + Math.PI)));
        context.lineTo(this.mouseX + (16 * Math.cos(rad + Math.PI)), this.mouseY + (16 * Math.sin(rad + Math.PI)));
        context.moveTo(this.mouseX + (4 * Math.cos(rad - (Math.PI / 2))), this.mouseY - (4 * Math.sin(rad + (Math.PI / 2))));
        context.lineTo(this.mouseX + (16 * Math.cos(rad - (Math.PI / 2))), this.mouseY - (16 * Math.sin(rad + (Math.PI / 2))));
        context.moveTo(this.mouseX + (12 * Math.cos(rad + (Math.PI / 16))), this.mouseY + (12 * Math.sin(rad + (Math.PI / 16))));
        context.arc(this.mouseX, this.mouseY, 12, rad + (Math.PI / 16), rad + (Math.PI * 7 / 16));
        context.moveTo(this.mouseX + (12 * Math.cos(rad + (Math.PI * 9 / 16))), this.mouseY + (12 * Math.sin(rad + (Math.PI * 9 / 16))));
        context.arc(this.mouseX, this.mouseY, 12, rad + (Math.PI * 9 / 16), rad + (Math.PI * 15 / 16));
        context.moveTo(this.mouseX + (12 * Math.cos(rad + (Math.PI * 17 / 16))), this.mouseY + (12 * Math.sin(rad + (Math.PI * 17 / 16))));
        context.arc(this.mouseX, this.mouseY, 12, rad + (Math.PI * 17 / 16), rad - (Math.PI * 9 / 16));
        context.moveTo(this.mouseX + (12 * Math.cos(rad - (Math.PI * 7 / 16))), this.mouseY + (12 * Math.sin(rad - (Math.PI * 7 / 16))));
        context.arc(this.mouseX, this.mouseY, 12, rad - (Math.PI * 7 / 16), rad - (Math.PI / 16));
        context.strokeStyle = '#2D2';
        context.lineWidth = 2;
        context.stroke();
        context.closePath();
        context.lineWidth = 1;
        context.strokeRect(this.mouseX - 0.5, this.mouseY - 0.5, 1, 1);
        break;
      case 'triangle':
        context.moveTo(this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad)));
        context.lineTo(this.mouseX + (18 * Math.cos(rad - (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad - (Math.PI / 8))));
        context.lineTo(this.mouseX + (14 * Math.cos(rad)), this.mouseY + (14 * Math.sin(rad)));
        context.lineTo(this.mouseX + (18 * Math.cos(rad + (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad + (Math.PI / 8))));
        context.strokeStyle = 'rgba(0,0,0,0.6)';
        context.lineWidth = 2;
        context.closePath();
        context.stroke();
        context.fillStyle = "#DD2";
        context.fill();
        context.fillStyle = "#2D2";
        context.beginPath();
        rad += Math.PI * 2 / 3;
        context.moveTo(this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad)));
        context.lineTo(this.mouseX + (18 * Math.cos(rad - (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad - (Math.PI / 8))));
        context.lineTo(this.mouseX + (14 * Math.cos(rad)), this.mouseY + (14 * Math.sin(rad)));
        context.lineTo(this.mouseX + (18 * Math.cos(rad + (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad + (Math.PI / 8))));
        context.closePath();
        context.stroke();
        context.fill();
        rad += Math.PI * 2 / 3;
        context.moveTo(this.mouseX + (4 * Math.cos(rad)), this.mouseY + (4 * Math.sin(rad)));
        context.lineTo(this.mouseX + (18 * Math.cos(rad - (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad - (Math.PI / 8))));
        context.lineTo(this.mouseX + (14 * Math.cos(rad)), this.mouseY + (14 * Math.sin(rad)));
        context.lineTo(this.mouseX + (18 * Math.cos(rad + (Math.PI / 8))), this.mouseY + (18 * Math.sin(rad + (Math.PI / 8))));
        context.closePath();
        context.stroke();
        context.fillStyle = "#2D2";
        context.fill();
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
  if (typeof gInput[event.key] === 'number') gInput.release(event.key);
}

function handleMouseDown(event) { gInput.mousedown(event) }
function handleMouseUp(event) { gInput.mouseup(event) }
function handleMouseMove(event) { gInput.setMousePos(event) }
