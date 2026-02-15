import processing.serial.*;

Serial myPort;
String data="";
float roll, pitch, yaw;

void setup() {
  size (800, 480, P3D);
  myPort = new Serial(this, "/dev/cu.usbserial-1140", 115200); // starts the serial communication
  myPort.bufferUntil('\n');
}

void draw() {
  pushMatrix();
  translate(width/2, height/2, 0);
  background(20);

  rotateX(radians(-roll));
  rotateY(radians(-yaw));
  rotateZ(radians(-pitch));

  // draw main body in red
  fill(255, 0, 0, 200);
  box(10, 10, 200);
  
  // draw front-facing tip in blue
  fill(0, 0, 255, 200);
  pushMatrix();
  translate(0, 0, -120);
  rotateX(PI/2);
  popMatrix();
  
  // draw wings and tail fin in green
  fill(0, 255, 0, 200);
  beginShape(TRIANGLES);
  vertex(-100,  2, 30); vertex(0,  2, -80); vertex(100,  2, 30);  // wing top layer
  vertex(-100, -2, 30); vertex(0, -2, -80); vertex(100, -2, 30);  // wing bottom layer
  vertex(-2, 0, 98); vertex(-2, -30, 98); vertex(-2, 0, 70);  // tail left layer
  vertex( 2, 0, 98); vertex( 2, -30, 98); vertex( 2, 0, 70);  // tail right layer
  endShape();
  beginShape(QUADS);
  vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
  vertex( 100, 2, 30); vertex( 100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
  vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(100, -2,  30); vertex(100, 2,  30);
  vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2, -30, 98); vertex(-2, -30, 98);
  vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
  vertex(-2, -30, 98); vertex(2, -30, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
  endShape();
  
  popMatrix();
}

// Read data from the Serial Port
void serialEvent (Serial myPort) { 
  data = myPort.readStringUntil('\n');
  if (data == null)
    return;

  String items[] = splitTokens(data, ": XYZ");
  if (items.length != 3)
    return;
  println(items[0] + " " + items[1] + " " + items[2]);

  roll = float(items[0]);
  pitch = float(items[1]);
  yaw = float(items[2]);
}
