// -------------------------------------------------------------
// CHRONO v.0.1
// (c) dimishpatriot, 2019 (https://github.com/dimishpatriot)
// программа для хронографа на 2 инфракарсных датчиках
// в сборе - MH-sensor-Series Flying-Fish
// https://www.aliexpress.com/item/32607543467.html?spm=a2g0s.9042311.0.0.50e533edzbydN2
// -------------------------------------------------------------

#include <LiquidCrystal_I2C.h>  // LCD I2C библиотека

#define SENSOR_START 3          // пин второго сенсора
#define SENSOR_END 4            // пин первого сенсора
#define SDA A4                  // пин SDA дисплея
#define SCL A5                  // пин SCL дисплея
#define LED 13                  // сигнальный пин
#define LEN 100                 // расстояние между датчиками, мм
#define MAX_TIME 2              // максимальное время замера, с

byte count = 0;                 // счетчик замеров
float mid_sp;                   // переменная для средней скорости, м/с
float last_sp = 0;              // переменная для предыдущего значения скорости, м/с
unsigned short mass = 560;      // переменная для массы объекта, микрограммы (далее будет возможность настройки)

// -------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2); // инициализация LCD 1602 I2C

void com_intro () {
  Serial.print("---------------------------------------\n");
  Serial.print("| ShortGun measuring programm (v.0.1) |\n");
  Serial.print("| (c) dimishpatriot, 2019             |\n");
  Serial.print("---------------------------------------\n");
  Serial.print("+ bullet mass "); Serial.print(mass * 1000000); Serial.print(" kg\n");
  Serial.print("+ measuring lenght ");  Serial.print(LEN); Serial.print(" mm\n");
  Serial.println();
  lcd.init();
  lcd.backlight();
}

void lcd_intro() {
  animation();
  lcd.setCursor(0, 0);
  lcd.printstr("- CHRONO v.0.1 -");
  lcd.setCursor(0, 1);
  lcd.printstr("(c)dimishpatriot");
  delay(1500);
  animation();
  lets_shoot();
}

void animation() {
  lcd.clear();
  for (int i = 0, j = 14; i < 16; i++, j--) {
    lcd.setCursor(i, 0);
    lcd.printstr("->");
    lcd.setCursor(j, 1);
    lcd.printstr("<-");
    delay(70);
  }
  lcd.clear();
}

void lets_shoot() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printstr("| LET'S        |");
  lcd.setCursor(0, 1);
  lcd.printstr("|       SHOOT! |");
}

void slow_bullet() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printstr("| It's veeeery |");
  lcd.setCursor(0, 1);
  lcd.printstr("| SLOW bullet! |");
  delay(1500);
}

void result_lcd(float *sp, float *energy) {
  char sp_str[5];
  char mid_str[4];
  char en_str[5];
  char count_str[3];

  // скорость, м/с
  dtostrf(*sp, 3, 1, sp_str);
  lcd.setCursor(0, 0);
  lcd.printstr("s "); lcd.printstr(sp_str);

  // средняя скорость, м/с
  dtostrf(mid_sp, 3, 0, mid_str);
  lcd.setCursor(10, 0);
  lcd.printstr("m "); lcd.printstr(mid_str);

  // энергия, Дж
  dtostrf(*energy, 2, 2, en_str);
  lcd.setCursor(0, 1);
  lcd.printstr("E "); lcd.printstr(en_str); lcd.printstr(" J");

  // номер замера
  lcd.setCursor(12, 1);
  lcd.printstr("[");

  itoa(count, count_str, 10);
  if (!count_str[1])
    lcd.setCursor(14, 1);
  else
    lcd.setCursor(13, 1);

  lcd.printstr(count_str); lcd.printstr("]");
}

void result_com (float *sp, float *energy, unsigned long *t2) {
  Serial.print("["); Serial.print(count); Serial.print("] RESULT\n");
  Serial.print("time:\t\t"); Serial.print((float)*t2 / 1000); Serial.print(" ms\n");
  Serial.print("current speed:\t"); Serial.print(*sp); Serial.print(" m/s\n");
  Serial.print("mid speed:\t"); Serial.print(mid_sp); Serial.print(" m/s\n");
  Serial.print("energy:\t\t"); Serial.print(*energy); Serial.print(" J\n");
  Serial.println();
}

// -------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // вывод заставки
  com_intro();
  lcd_intro();

  // инициализация пинов
  pinMode(SENSOR_START, INPUT);
  pinMode(SENSOR_END, INPUT);
  pinMode(LED, OUTPUT);
}

// -------------------------------------------------------------
void loop() {
  boolean first, second;  // переменные для хранения состояния 1 и 2 датчика
  unsigned long t1;       // переменная для хранения времени прохождения 1 датчика, микросекунда
  unsigned long t2;       // переменная для хранения времени прохождения от 1 до 2 датчика, микросекунда
  float sp;               // переменная для текущей скорости, м/с
  float energy;           // переменная для кинетической энергии объекта, Дж

  first = second = 0;

  digitalWrite(LED, 0);

  if (first = !digitalRead(SENSOR_START)) {
    t1 = micros();

    while (1) {
      if (second = !digitalRead(SENSOR_END)) {
        t2 = micros() - t1; // для точности лучше отметку времени получить сразу

        if (t2 < MAX_TIME * 1000000) // проверка времени замера в микросекундах
        {
          pinMode(LED, 1); // включение сигнального пина

          sp = LEN / (float)(t2 / 1000);
          if (!last_sp)
            mid_sp = sp;
          else
            mid_sp = (sp + last_sp) / 2;

          energy = ((float)mass / 1000000) * (sp * sp) / 2;

          count++;
          if (count == 99)
            count = 1;

          // вывод результатов
          result_lcd(&sp, &energy);
          result_com(&sp, &energy, &t2);

          last_sp = sp; // обновление последнего записанного значения скорости

        } else
        {
          Serial.print("Very-very slow bullet :) Shoot again!\n");
          slow_bullet();
          lets_shoot();
        }

        delay(500);       // пауза
        pinMode(LED, 0);  // выключение сигнального пина
        break;            // выход на начало loop()
      }
    }
  }
}
