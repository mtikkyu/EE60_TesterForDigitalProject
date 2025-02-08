#define BINARY_ID 0b0100110

int s[20] = { 0b100, 0b000, 0b110, 0b101,
              /*First Bit*/ 0b110, 0b101,
              0b110, 0b101,
              0b110, 0b101,
              0b110, 0b101,
              0b110, 0b101,
              0b110, 0b101,
              0b110, 0b100, 0b000, 0b000
            };

#define CS_PIN   10
#define SCLK_PIN 13
#define MOSI_PIN 11

#define CLK_PIN  2
#define PERIOD_TIME 50 //ms

unsigned long current_time = 0;
bool toggle_clk = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(CS_PIN, OUTPUT);
  pinMode(SCLK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);

  digitalWrite(CS_PIN, 1);
  digitalWrite(SCLK_PIN, 0);
  digitalWrite(MOSI_PIN, 0);
  digitalWrite(SCLK_PIN, 0);

  int s_detect[7] = {0};
  int _count_state = 0;
  for (int i = 6; i >= 0; i--) {
    s_detect[_count_state++] = (BINARY_ID >> i) & 0x01;
  }

  _count_state = 0;
  for (int j = 4; j < 17; j++) {
    if (j % 2 == 0) s[j] = s_detect[_count_state++] | 0x06;
  }

  //  for (int count = 0; count < 20; count++) {
  //    Serial.println(s[count], BIN);
  //  }

  current_time = 0;
  Serial.println("Start!");
}

unsigned short count_state = 0;
String input_string;

void loop() {
  if (millis() - current_time > PERIOD_TIME) {
    digitalWrite(CLK_PIN, toggle_clk);
    toggle_clk = !toggle_clk;
    current_time = millis();
  }

  if (Serial.available() > 0) {
    input_string = Serial.readString();
    Serial.print(count_state); Serial.print("\t");
    Serial.print((s[count_state] >> 2) & 0x01);
    Serial.print((s[count_state] >> 1) & 0x01);
    Serial.print(s[count_state] & 0x01);
    Serial.println();

    digitalWrite(CS_PIN, (s[count_state] >> 2) & 0x01);
    digitalWrite(SCLK_PIN, (s[count_state] >> 1) & 0x01);
    digitalWrite(MOSI_PIN, s[count_state] & 0x01);

    count_state++;
    if (count_state > 19) count_state = 2;
  }
}
