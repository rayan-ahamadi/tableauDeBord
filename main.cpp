#include <LiquidCrystal.h>

int seconds = 0;
LiquidCrystal lcd_1(SCL, SDA, 13, 12, 11, 10);

const int led_blue = 2;
const int led_red = 3;
const int fan_motor = 4;
const int led_green = 5;
const int led_orange = 6;
const int TRIGGER_PIN = 7;
const int ECHO_PIN = 8;
const int buzzer = 9;

const int cap_temp = A0;
const int cap_hum = A1;
const int cap_lum = A2;

unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;


void lcd_data(float distance_m, float temperature, float luminosity, float humidity)
{
    // Effacer l'écran
    lcd_1.clear();

    // Afficher la température
    lcd_1.setCursor(0, 0);
    lcd_1.print("Temp: ");
    lcd_1.print(temperature);
    lcd_1.print("C");

    // Attendre 2 secondes
    delay(500);

    // Effacer l'écran
    lcd_1.clear();

    // Afficher l'humidité
    lcd_1.setCursor(0, 0);
    lcd_1.print("Humidity: ");
    lcd_1.print(humidity);
    lcd_1.print("%");

    // Attendre 2 secondes
    delay(500);

    // Effacer l'écran
    lcd_1.clear();

    // Afficher la luminosité
    lcd_1.setCursor(0, 0);
    lcd_1.print("Luminosity: ");
    lcd_1.print(luminosity);

    // Attendre 2 secondes
    delay(500);

    // Effacer l'écran
    lcd_1.clear();

    // Afficher la distance
    lcd_1.setCursor(0, 0);
    lcd_1.print("Distance: ");
    lcd_1.print(distance_m);
    lcd_1.print("m");
}

void print_data(float distance_m, float temperature, float luminosity, float humidity)
{
  Serial.print(F("Distance: "));
  Serial.print(distance_m);
  Serial.println(F("m"));
  
  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println("%");
  
  Serial.print("Luminosity : ");
  Serial.println(luminosity);
}

void setup()
{
  lcd_1.begin(16, 2);
  
  pinMode(led_blue, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(fan_motor, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_orange, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);
  
  pinMode(cap_temp, INPUT);
  pinMode(cap_hum, INPUT);
  pinMode(cap_lum, INPUT);
  
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  int cap_value = analogRead(cap_temp); // Lecture de la valeur du capteur de température
  float tension = cap_value * (5.0 / 1023.0); // Conversion de la valeur en tension
  float temperature = (tension - 0.5) * 100; // Conversion de la tension en température en degrés Celsius
  
  float humidity = analogRead(cap_hum); // Lecture de la valeur du capteur d'humidité
  humidity = map(humidity, 0, 1023, 0, 100); // Conversion de la valeur en pourcentage
  
  float luminosity = analogRead(cap_lum);
  
  
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_m = ((measure / 2.0 * SOUND_SPEED)/ 1000.0);
   
  print_data(distance_m,temperature, luminosity, humidity);
  lcd_data(distance_m,temperature, luminosity, humidity);
  
  
  if (temperature >= 10 && temperature <= 18)
  {
      digitalWrite(fan_motor, LOW);
      digitalWrite(led_red, LOW);
      digitalWrite(led_blue, HIGH);
  }
  
  if (temperature >= 19 && temperature <= 21)
  {
      digitalWrite(fan_motor, LOW);
      digitalWrite(led_blue, LOW);
      digitalWrite(led_red, HIGH);
  }
  
  if (temperature > 21)
  {
      digitalWrite(led_red, LOW);
      digitalWrite(led_blue, LOW);
  	  digitalWrite(fan_motor, HIGH);
  }
  
  if (humidity >= 40 && humidity <= 60)
  {
      digitalWrite(led_orange, LOW);
      digitalWrite(led_green, HIGH);
  }
  
  if (humidity < 40 || humidity > 60)
  {
      digitalWrite(led_green, LOW);
      digitalWrite(led_orange, HIGH);
  }
  
  if (distance_m <= 0.5)
  {
      digitalWrite(buzzer, HIGH);
  }
  else if (distance_m > 0.5)
  {
      digitalWrite(buzzer, LOW);
  }
  
}
