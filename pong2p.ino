#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>

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
  switch(sensor) {
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


void draw_paddles(float player1_dist, float player2_dist) {
  int paddle_width = 30;
  int paddle_height = 10;
  
  //Convert distance to coords for Player 1
  int paddle_coord1 = int((16 - player1_dist) / (16 - 4) * (300 - 30));
  vga.fillRect(20, paddle_coord1, paddle_height, paddle_width, vga.RGB(255,255,255));

  //Convert distance to coords for Player 2
  int paddle_coord2 = int((16 - player2_dist) / (16 - 4) * (300 - 30));
  vga.fillRect(360, paddle_coord2, paddle_height, paddle_width, vga.RGB(255,255,255));

}

const int num_index = 10;
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
  player1_dist = sum[0]/10;
  player2_dist = sum[1]/10;
}


int a = 20; 
//int x = 100 + 
int y = 300;
int delta = 2;
void animate() {
  // Ball touches left border
  if (x<=100) {
    if (eliminated[0] {
      dx = delta;
    }
    else {
      player_lives[0]--;
      new_round();
    }
  }

  // Ball touches Player 1 paddle
  if (x_range contains x + a && y_range contains y)

  // Ball touches right border
  if(x+a>=700) {
    if (eliminated[1]) {
      dx=-delta;
    }
    else {
      player_lives[1]--;
      new_round();
    }
  }



  // Ball touches top border
  if(y+a>=600) {
    dy = -delta; 
  }

  // Ball touches bottom border
  if(y<=0) {
    dy = delta;
  }

  x+=dx;  y+=dy;
  vga.fillRect(x, y, 20, 20, vga.RGB(255, 255, 255));
}

void new_round() {

}


boolean started = true;


void setup()
{
  pinMode(TRIG1, OUTPUT); // set up distance sensor pins
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(Font6x8);

  //TODO for Natasha:
  // Ask user if they are ready 
  // Helpful functions you should use: vga.setCursor(x,y) and vga.print("text") 
}



void loop()
{
  if (!started) {
    // Ask user 

  }
  else {
    vga.clear(0);

    player1_dist = read_distance(1);
    player2_dist = read_distance(2);
    average_smoothing(player1_dist, player2_dist);
    draw_paddles(player1_dist, player2_dist);

    vga.show();
  }

}
