// CHRONO v.0.1
// (c) dimishpatriot, 2019 (https://github.com/dimishpatriot)
// программа для хронографа на 2 инфракарсных датчиках
// в сборе - MH-sensor-Series Flying-Fish 
// https://www.aliexpress.com/item/32607543467.html?spm=a2g0s.9042311.0.0.50e533edzbydN2

#define SENSOR_START  3
#define SENSOR_END    5

boolean first = 0;
boolean second = 0;     // переменные для хранения состояния 1 и 2 датчика
unsigned long t1;       // переменная для хранения времени прохождения 1 датчика, микросекунда
unsigned long t2;       // переменная для хранения времени прохождения от 1 до 2 датчика, микросекунда
byte len = 100;         // расстояние между датчиками, мм
unsigned int count = 0; // счетчик замеров
float sp;               // переменная для текущей скорости, м/с
float mid_sp;           // переменная для средней скорости, м/с
float last_sp = 0;      // переменная для предыдущего значения скорости, м/с
float mass = 0.00054;   // переменная для массы объекта, кг
float energy;           // переменная для кинетической энергии объекта, Дж

void setup() 
{
  Serial.begin(9600);
  pinMode(SENSOR_START, INPUT);
  pinMode(SENSOR_END, INPUT);
  Serial.print("---------------------------------------\n");
  Serial.print("| ShortGun measuring programm (v.0.1) |\n");
  Serial.print("| (c) dimishpatriot, 2019             |\n");
  Serial.print("---------------------------------------\n");
}

void loop()
{
  if (first = !digitalRead(SENSOR_START))
  {
    t1 = micros();
        
    while (1)
    {
      if (second = !digitalRead(SENSOR_END))
        break;
    }

    count++;
    t2 = micros() - t1;
    sp = len / (float)(t2 / 1000);
    if (!last_sp)
      mid_sp = sp;
    else
      mid_sp = (sp + last_sp) / 2;
      
    energy = mass * (sp * sp) / 2;
      
    Serial.print("["); Serial.print(count); Serial.print("] RESULT\n");
       
    Serial.print("time:\t\t"); Serial.print(t2); Serial.print(" mks\n");
        
    Serial.print("current speed:\t"); Serial.print(sp); Serial.print(" m/s\n");
        
    Serial.print("mid speed:\t"); Serial.print(mid_sp); Serial.print(" m/s\n");

    Serial.print("energy:\t\t"); Serial.print(energy); Serial.print(" J\n");
    Serial.println();
    
    first = 0;
    second = 0;
    last_sp = sp;
  }
}
