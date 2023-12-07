#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#define BUTTON0 34

//pin configuration
const int redPin = 15;
const int greenPin = 2;
const int bluePin = 4;
const int hsyncPin = 14;
const int vsyncPin = 12;

//VGA Device
VGA3Bit vga;

//sensors
#define TRIG1 5
#define ECHO1 16
#define TRIG2 19
#define ECHO2 18

// Returns distance (cm) from distance sensor
float read_distance(int sensor) {
  int trig;
  int echo;
  switch (sensor) {
    case 1:
      trig = TRIG1;
      echo = ECHO1;
      break;
    case 2:
      trig = TRIG2;
      echo = ECHO2;
      break;
  }
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long timeout = micros() + 26233L;
  while ((digitalRead(echo) == LOW) && (micros() < timeout));
  unsigned long start_time = micros();
  timeout = start_time + 26233L;
  while ((digitalRead(echo) == HIGH) && (micros() < timeout));
  unsigned long lapse = micros() - start_time;
  float distance = lapse * 0.01716f;
  return (distance > 16) ? 16 : (distance < 4) ? 4 : distance;
}

int paddle_coord1;
int paddle_coord2;

void draw_paddles(float player1_dist, float player2_dist) {
  int paddle_width = 30;
  int paddle_height = 10;

  //Convert distance to coords for Player 1
  paddle_coord1 = int((16 - player1_dist) / (16 - 4) * (300 - 30));
  vga.fillRect(20, paddle_coord1, paddle_height, paddle_width, vga.RGB(255, 255, 255));

  //Convert distance to coords for Player 2
  paddle_coord2 = int((16 - player2_dist) / (16 - 4) * (300 - 30));
  vga.fillRect(370, paddle_coord2, paddle_height, paddle_width, vga.RGB(255, 255, 255));

}

const int num_index = 20;
int smoothing_index[2] = {0};
float distance_readings[2][num_index] = {0};
float player1_dist, player2_dist;

void average_smoothing(float dist1, float dist2) {
  distance_readings[0][smoothing_index[0]] = dist1;
  distance_readings[1][smoothing_index[1]] = dist2;

  for (int i = 0; i < 2; i++) {
    smoothing_index[i] = (smoothing_index[i] + 1) % num_index;
  }

  float sum[2] = {0};
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < num_index; j++) {
      sum[i] += distance_readings[i][j];
    }

  }
  player1_dist = sum[0] / num_index;
  player2_dist = sum[1] / num_index;
}


int a = 6;
int x = 195;
int y = 150;
int delta = 2;
int dx = delta;
int dy = delta;

int p1_score = 0;
int p2_score = 0;
boolean freeze = false;
boolean started = false;

void animate() {
  // Ball touches left border
  if (x <= 0) {
    p2_score++;
    new_round(2);
  }

  // Ball touches right border
  if (x + a >= 400) {
    p1_score++;
    new_round(1);
  }

  // Ball touches top border
  if (y + a >= 300) {
    dy = -delta;
  }

  // Ball touches bottom border
  if (y <= 0) {
    dy = delta;
  }

  // Ball touches Player 1 paddle
  if (x <= 30 && x >= 20 && y >= paddle_coord1 && y <= paddle_coord1 + 30) {
    dx = delta;
  }

  // Ball touches Player 2 paddle
  if (x + a >= 370 && x + a <= 380 && y >= paddle_coord2 && y <= paddle_coord2 + 30) {
    dx = -delta;
  }

  x += dx;
  y += dy;
  vga.fillRect(x, y, a, a, vga.RGB(255, 255, 255));
}

void new_round(int scorer) {
  if (p1_score == 5) {
    vga.clear(0);   
    vga.setCursor(150, 150);
    vga.print("GAME OVER");
    vga.show();
    delay(1500);
    vga.clear(0);
    vga.setCursor(150, 150);
    vga.print("Player 1 wins!");
    vga.show();
    x = 195;
    y = 150;
    reset();
  }
  else if (p2_score == 5) {
    vga.clear(0);   
    vga.setCursor(150, 150);
    vga.print("GAME OVER");
    vga.show();
    delay(1500);
    vga.clear(0);
    vga.setCursor(150, 150);
    vga.print("Player 2 wins!");
    vga.show();
    x = 195;
    y = 150;
    reset();
  }
  else if (scorer == 1) {
    freeze = false; 
    delay(1500);
    x = 195;
    y = 150;
    dx = delta;
  }
  else {
    freeze = false; 
    delay(1500);
    x = 195;
    y = 150;
    dx = -delta;
  }
}

void reset() {
  p1_score = 0;
  p2_score = 0;
  started = false;
  freeze = false;
  delay(1500);  
}

void setup()
{
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(TRIG1, OUTPUT); // set up distance sensor pins
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(Font6x8);
}

void loop()
{
  if (!started) {
    // Ask user
    if (digitalRead(BUTTON0) == LOW) {
      started = true;
      freeze = true; 
    }
    
    vga.clear(0);
    // Ask the user to press the Start button
    vga.setCursor(150, 150);
    vga.print("Press START to begin");
    vga.show();
  }

  else {
    
    vga.clear(0);
    vga.line(0, 0, 0, 300, vga.RGB(255, 255, 255));
    vga.line(399, 0, 399, 300, vga.RGB(255, 255, 255));
    for (int i = 0; i <= 299; i += 20) {
      vga.line(200, i, 200, i + 10, vga.RGB(255, 255, 255));
    }
    vga.setCursor(80, 20);
    vga.print(p1_score);
    vga.setCursor(280, 20);
    vga.print(p2_score);
    player1_dist = read_distance(1);
    player2_dist = read_distance(2);
    average_smoothing(player1_dist, player2_dist);
    draw_paddles(player1_dist, player2_dist);
    animate();
    vga.show();
    if (freeze == true) { 
      delay(1500);
      freeze = false;
    }

  }

}
