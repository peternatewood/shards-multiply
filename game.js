var gBounds = {
  0: { l: 48, t: 24, r: 752, b: 576 },
  collide: function(p) {
    if (p.x - p.size < this[0].l || p.y - p.size < this[0].t || p.x + p.size > this[0].r || p.y + p.size > this[0].b) return true;
    return false;
  }
}

var PLAYER_RADIUS = 12;
var PLAYER_SPEED = 4;
var FRICTION = 0.8;

var Target = function(x, y) {
  this.x = x;
  this.y = y;
  this.life = 11;
  return this;
}
Target.prototype.collide = function(other) {
  var collide = Math.sqrt(Math.pow(other.x - this.x, 2) + Math.pow(other.y - this.y, 2)) < other.size;
  if (this.life > 1 && collide && other.life == 4) this.life--;
  if (collide) {
    var rad = Math.atan2(other.y - this.y, other.x - this.x);
    return { x: this.x + ((other.size + 1) * Math.cos(rad)), y: this.y + ((other.size + 1) * Math.sin(rad)) }
  }
  else return false;
}
Target.prototype.render = function(context) {
  context.beginPath();
  context.arc(this.x + 2, this.y + 2, 13, 0, Math.PI * 2);
  context.fillStyle = '#222';
  context.fill();
  context.closePath();

  context.beginPath();
  context.arc(this.x, this.y, 12, 0, Math.PI * 2);
  context.fillStyle = 'rgb(' + Math.round(255 * (this.life / 12)) + ',' + Math.round(255 * ((12 - this.life) / 12)) + ',' + 0x22 + ')';
  context.fill();
  context.closePath();
}
var optionsTargets = [
  new Target(144, 506),
  new Target(400, 506)
];

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

function update() {
  gScene.update();
}

var gRenderer = {
  frame: 0,
  render: function() {
    this.context.fillStyle = '#000';
    this.context.fillRect(0, 0, canvas.width, canvas.height);

    switch (gScene.scene) {
      case 'title':
        gScene.renderTitle(this.context);
        break;
      case 'hangar':
      case 'game':
        gScene.renderBack(this.context);
        gPlayer.render(this.context);
        break;
    }
    gInput.render(this.context);
    // Debug display
    // this.context.font = '16px monospace';
    // this.context.fillStyle = '#FFF';
    // this.context.fillText('Mouse: ' + gInput.mouseX + ', ' + gInput.mouseY + ' Player rad: ' + gPlayer.rad, 12, 12);
    this.frame = (this.frame + 1) % 60;
  }
}

function step(timestamp) {
  gRenderer.render();
  // Only request next frame if user is still in this tab
  if (!start) var start = timestamp;
  var progress = timestamp - start;
  if (progress < 2000) window.requestAnimationFrame(step);
}

function init() {
  gRenderer.canvas = document.getElementById('canvas');
  gRenderer.context = gRenderer.canvas.getContext('2d');
  gRenderer.context.font = '32px courier';
  gPlayer.x = (gRenderer.canvas.width / 2) - PLAYER_RADIUS;
  gPlayer.y = (gRenderer.canvas.height * 0.75) - PLAYER_RADIUS;

  // Key listeners only seem to work on the document
  document.addEventListener('keydown', handleKeyDown);
  document.addEventListener('keyup', handleKeyUp);
  // Set mouse listeners on the canvas
  gRenderer.canvas.addEventListener('mousemove', handleMouseMove);
  gRenderer.canvas.addEventListener('mousedown', handleMouseDown);
  gRenderer.canvas.addEventListener('mouseup', handleMouseUp);
  gRenderer.canvas.addEventListener('contextmenu', function(event) { event.preventDefault(); event.stopPropagation() });

  setInterval(update, 25);
  window.requestAnimationFrame(step);
}

init();
