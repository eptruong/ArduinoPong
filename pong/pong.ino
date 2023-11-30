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
#define TRIG2 26
#define ECHO2 27

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


void draw_paddles() {
  int paddle_width = 30;
  int paddle_height = 10;
  int player_coord[2];
  
  //Convert distance to coords for Player 1
  player_coord[0] = int((16 - player_dist[0]) / (16 - 4) * (300 - 30));
  vga.fillRect(20, paddle_coord[0], paddle_height, paddle_width, vga.RGB(255,255,255));
  
  //Player 2
  player_coord[1] = int((16 - player_dist[1]) / (16 - 4) * (300 - 30));
  vga.fillRect(360, paddle_coord[1], paddle_height, paddle_width, vga.RGB(255,255,255));
    
}

int num_index = 10;
int smoothing_index[2] = {0};
float distance_readings[2][num_index];

void average_smoothing() {
  for (int i = 0; i < 2; i++) {
    distance_readings[i][smoothing_index[i]++] = player_dist[i]; 
    smoothing_index[i] = (smoothing_index[i]) % numIndex;
  }

  float sum[2];
  for (int i = 0; i < 2; i++) {
    for int(j = 0; j < num_index; j++) {
      sum[i] += distance_readings[i][j];
    }
    player_dist[i] = sum[i]/10;
  }
}

// Converts distance to coordinate on playing field
int* convert_distance(float player_dist[]) {
  //TODO Nathan
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

//void new_round() {
//  
//}


boolean started = false;
int player_lives[2] = {5};

void setup()
{
  vga.setFrameBufferCounter(2);
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
    vga.clear(vga.RGB(0));
    delay(10);
    float player_dist[2];
    for (int i = 0; i < 2; i++) {
      player_dist[i] = read_distance(i + 1);
    }
    
    average_smoothing();
    draw_paddles(convert_distance(player_dist));
    
  
      
    vga.show();
  }

}
