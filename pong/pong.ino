#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>

//pin configuration
const int redPin = 4;
const int greenPin = 2;
const int bluePin = 15;
const int hsyncPin = 14;
const int vsyncPin = 12;

//VGA Device
VGA3Bit vga;

//sensors
#define TRIG1 5
#define ECHO1 16
#define TRIG2 26
#define ECHO2 27
#define TRIG3 ???
#define ECHO3 ???
#define TRIG4 ???
#define ECHO4 ???


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
    case 3:
      trig = TRIG3;
      echo = ECHO3;
      break;
    case 4:
      trig = TRIG4;
      echo = ECHO4;
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


void draw_paddles(int paddle_coord[], boolean eliminated[]) {
  int paddle_width = 75;
  int paddle_height = 20;

  // If player is eliminated, create a wall for the ball to bounce on
  
  //Player 1
  if (!eliminated[0]) {
    vga.fillRect(120, paddle_coord, paddle_height, paddle_width, vga.RGB(255,255,255));
  }
  else {
    vga.fillRect(80, 0, 20, 600, vga.RGB(255,255,255));
  }
  
  //Player 2
  if (!eliminated[1]) {
    vga.fillRect(660, paddle_coord, paddle_height, paddle_width, vga.RGB(255,255,255));
  }
  else {
    vga.fillRect(700, 0, 20, 600, vga.RGB(255,255,255));
  }
  
  //Player 3
  if (!eliminated[2]) {
    vga.fillRect(paddle_coord, 20, paddle_height, paddle_width, vga.RGB(255,255,255));
  }
  else {
    vga.fillRect(100, 0, 600, 20, vga.RGB(255,255,255));
  }
    
  //Player 4
  if (!eliminated[3]) {
    vga.fillRect(paddle_coord, 560, paddle_height, paddle_width, vga.RGB(255,255,255));
  }
  else {
    vga.fillRect(100, 620, 600, 20, vga.RGB(255,255,255));
  }
    
}

void draw_ball() {

}

int num_index = 10;
int smoothing_index[4] = {0};
float distance_readings[4][num_index];

float* average_smoothing(float player_dist[]) {

  for (int i = 0; i < 4; i++) {
    distance_readings[i][smoothing_index[i]++] = player_dist[i]; 
    smoothing_index[i] = (smoothing_index[i]) % numIndex;
  }

  float sum[4];
  for (int i = 0; i < 4; i++) {
    for int(j = 0; j < num_index; j++) {
      sum[i] += distance_readings[i][j];
    }
    player_dist[i] = sum[i]/10;
  }
  return player_dist;
}

// Converts distance to coordinate on playing field
int* convert_distance(float player_dist[]) {
  //TODO Nathan
}

boolean four_player = true; // Current state of gamemode
boolean eliminated[4] = {0}; // Track who is eliminated (Players 3 & 4 eliminated if four_player = false)

void setup()
{
  vga.init(vga.MODE800x600, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(Font6x8);

  if (!four_player) {
      eliminated[2] = 1;
      eliminated[3] = 2;
  }
  
  //TODO for Natasha:
    // Originally 4 player, button click changes to 2 player
    // Ask user if they are ready with the option to alternate between 2/4 player
    // Print on screen the current mode (2 or 4) 
    // Helpful functions you should use: vga.setCursor(x,y) and vga.print("text") 

}



void loop()
{
  vga.clear(vga.RGB(0));
  delay(10);
  float player_dist[4]; // [0...n-1] where index i is Player i + 1
  for (int i = 0; i < 4; i++) {
    player_dist[i] = read_distance(i + 1);
  }
  float smooth_player_dist[] = average_smoothing(player_dist);
  draw_paddles(smooth_player_dist);
  vga.show();
}
