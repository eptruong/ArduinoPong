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

int paddle_coord1;
int paddle_coord2;

void draw_paddles(float player1_dist, float player2_dist) {
  int paddle_width = 30;
  int paddle_height = 10;
  
  //Convert distance to coords for Player 1
  paddle_coord1 = int((16 - player1_dist) / (16 - 4) * (300 - 30));
  vga.fillRect(20, paddle_coord1, paddle_height, paddle_width, vga.RGB(255,255,255));

  //Convert distance to coords for Player 2
  paddle_coord2 = int((16 - player2_dist) / (16 - 4) * (300 - 30));
  vga.fillRect(370, paddle_coord2, paddle_height, paddle_width, vga.RGB(255,255,255));

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


int a = 6; 
int x = 200;
int y = 150;
int delta = 2;
int dx = delta;
int dy = delta;
void animate() {
  // Ball touches left border
  if (x<=0) {
//    player_lives[0]--;
//    new_round();
    dx = delta;
  }
  
  // Ball touches right border
  if(x+a>=400) {
//    player_lives[1]--;
//    new_round();
    dx = -delta;
  }

  // Ball touches top border
  if(y+a>=300) {
    dy = -delta; 
  }

  // Ball touches bottom border
  if(y<=0) {
    dy = delta;
  }

  // Ball touches Player 1 paddle
  if (x <= 30 && x >= 20 && y >= paddle_coord1 && y <= paddle_coord1 + 30) {
    dx = delta;
  }

  // Ball touches Player 2 paddle
  if (x+a >= 370 && x+a <= 380 && y >= paddle_coord2 && y <= paddle_coord2 + 30) {
    dx = -delta;
  }

  
  x+=dx;
  y+=dy;
  vga.fillRect(x, y, a, a, vga.RGB(255, 255, 255));
}

void new_round() {

}


boolean started = true;

// This variable will be set to true if the user is ready to start the game
boolean ready_to_start = false;

// Function to handle button presses
void handle_button_press(int button_pin) {
 int button_state = digitalRead(button_pin);

 if (button_state == HIGH) {
    // If the Start button is pressed, start the game
    if (button_pin == START_BUTTON_PIN) {
      started = true;
    }

    // If the user is ready to start, set the ready_to_start variable to true
    if (button_pin == READY_BUTTON_PIN) {
      ready_to_start = true;
    }
 }
}



void setup()
{
  pinMode(TRIG1, OUTPUT); // set up distance sensor pins
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE400x300, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(Font6x8);

   // Initialize the button pins
   pinMode(START_BUTTON_PIN, INPUT_PULLUP);
   pinMode(READY_BUTTON_PIN, INPUT_PULLUP);
  
   // Display the initial message
   vga.setCursor(50, 150);
   vga.print("Press READY when you are ready to start");
   vga.show();

  int lives = 5;
  
  // Function to handle the player losing a life
  void lose_life() {
   lives--;
   vga.setCursor(50, 150);
   vga.print("Lives: ");
   vga.print(lives);
   vga.show();
  
   // If the player has no lives left, end the game
   if (lives <= 0) {
      started = false;
      ready_to_start = false;
      vga.setCursor(50, 150);
      vga.print("Game Over");
      vga.show();
   }
  }
}




void loop() {
 // Handle button presses
 handle_button_press(START_BUTTON_PIN);
 handle_button_press(READY_BUTTON_PIN);

 if (!started) {
    // Check if the user is ready to start
    if (ready_to_start) {
      // Clear the message
      vga.clear(0);
      vga.show();

      // Ask the user to press the Start button
      vga.setCursor(50, 150);
      vga.print("Press START to begin");
      vga.show();
    }
 }
 else {
    vga.clear(0);

    player1_dist = read_distance(1);
    player2_dist = read_distance(2);
    average_smoothing(player1_dist, player2_dist);
    draw_paddles(player1_dist, player2_dist);
    animate();
    vga.show();
    if (/* player missed ball and ball touches edge */) {
    lose_life();
    }
  }
}
