// CHRONO v.0.1
// (c) dimishpatriot, 2019 (https://github.com/dimishpatriot)
// программа для хронографа на 2 инфракарсных датчиках
// в сборе - MH-sensor-Series Flying-Fish 
// https://www.aliexpress.com/item/32607543467.html?spm=a2g0s.9042311.0.0.50e533edzbydN2

#define SENSOR_START  3
#define SENSOR_END    5

boolean first = 0;
boolean second = 0; // переменные для хранения состояния 1 и 2 датчика
int t1; // переменная для хранения времени прохождения 1 датчика
int t2; // переменная для хранения времени прохождения от 1 до 2 датчика
int len = 100; // расстояние между датчиками, мм
float sp; // переменная для скорости

void setup() 
{
  Serial.begin(9600);
  pinMode(SENSOR_START, INPUT);
}

void loop()
{
  first = !digitalRead(SENSOR_START);

  if (first)
  {
    t1 = millis();
        
    while (1)
    {
      if (second = !digitalRead(SENSOR_END))
        break;
    }
    t2 = millis() - t1;
    first = 0;
    second = 0;
    Serial.print("time: "); Serial.print(t2); Serial.print(" ms");
    Serial.println();
    sp = len / (float)t2;
    Serial.print("current speed: "); Serial.print(sp); Serial.print(" m/s");
    Serial.println();
  }
}
