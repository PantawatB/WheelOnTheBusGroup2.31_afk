#define BUZZER 2  // Pin to connect the buzzer
#define BUTTON 22  // Pin to connect the button

// Notes and their corresponding frequencies
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

// The melody of the song
int melody[] = {

  NOTE_C6, 4, NOTE_F6, 8, NOTE_F6, 8, NOTE_F6, 8, NOTE_F6, 8,
  NOTE_F6, 4, NOTE_C7, 4, NOTE_A6, 4, NOTE_F6, 4, NOTE_C6, 2, 
  NOTE_G6, 4, NOTE_G6, 4, NOTE_G6, 2, NOTE_E6, 4, NOTE_A6, 4, NOTE_C6, 2,
  NOTE_C6, 4, NOTE_F6, 8, NOTE_F6, 8, NOTE_F6, 8, NOTE_F6, 8,
  NOTE_F6, 4, NOTE_C7, 4, NOTE_A6, 4, NOTE_F6, 2, 
  // NOTE_C6, 2, 
  NOTE_G6, 2, NOTE_C6, 2, NOTE_F6, 2, REST, 2
};

int tempo = 130;
int maintempo = 130;

int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

// Variables for timer interrupt
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Variable to store the tempo of the song


// Variable to store the current speed level
int speed = 1;

void IRAM_ATTR onTimer() {
    // Toggle the state of the buzzer pin
    digitalWrite(BUZZER, !digitalRead(BUZZER));
}

void IRAM_ATTR onButton() {
    // Check the state of the button
    if (digitalRead(BUTTON) == LOW) {
        // Increase the speed level
        if(speed <= 5) speed++;
        else speed = 1;
    }
    // Change the tempo based on the current speed level
    switch (speed) {
        case 1:
            tempo = maintempo / 2;
            break;
        case 2:
            tempo = maintempo / 1.5;
            break;
        case 3:
            tempo = maintempo / 1;
            break;
        case 4:
            tempo = maintempo * 1.5;
            break;
        case 5:
            tempo = maintempo * 2;
            break;
    }
}

void setup() {
    pinMode(BUZZER, OUTPUT);
    pinMode(BUTTON, INPUT);
  
    // Set up the timer interrupt
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);

    // Set up the I/O interrupt
    attachInterrupt(BUTTON, onButton, FALLING);
    play();
}
void loop() {
  //blank
}

void play(){
  for (int i = 0; i < sizeof(melody) / sizeof(int); i += 2) {
      wholenote = (60000 * 4) / tempo;
      divider = melody[i + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
        if(melody[i] != REST){
            // Set the timer interrupt to the current note frequency
            timerAlarmWrite(timer, 1000000 / melody[i] / 2, true);
            timerAlarmEnable(timer);
        }
        // Play the note for the current duration
        delay(noteDuration*0.9);
        if(melody[i] != REST){
            // Disable the timer interrupt
            timerAlarmDisable(timer);
        }
        delay(noteDuration*0.1);
        noTone(BUZZER);
    }
}