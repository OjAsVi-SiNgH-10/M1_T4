#define Moisture_sensor 2
#define PIR_1 3
#define PIR_2 4

#define Moisture_LED 8
#define PIR_LED_2 10
#define timer_interrupt_LED 6

volatile bool PIR_LED_1_state = false;
volatile bool Moisture_LED_state = false;
volatile bool PIR_LED_2_state = false;
volatile bool timer_interrupt_LED_state = false;

int i = 2;

void starttimmerfunc()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 31249;

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{

    Serial.print("Led Timmer: ");
    Serial.print(i);
    Serial.println(" milli seconds");

    digitalWrite(timer_interrupt_LED, digitalRead(timer_interrupt_LED) ^ 1);

    i = i + 2;
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(Moisture_LED, OUTPUT);
    pinMode(PIR_LED_2, OUTPUT);
    pinMode(timer_interrupt_LED, OUTPUT);

    pinMode(PIR_1, INPUT);
    pinMode(PIR_2, INPUT);
    pinMode(Moisture_sensor, INPUT);

    Serial.begin(9600);
    starttimmerfunc();

    noInterrupts();
    PCIFR |= bit(PCIE2);
    PCICR |= bit(PCIE2);
    PCMSK2 |= bit(PCINT18);
    PCMSK2 |= bit(PCINT19);
    PCMSK2 |= bit(PCINT20);
    interrupts();
}

void loop()
{
}

ISR(PCINT2_vect)
{
    if (digitalRead(Moisture_sensor) == HIGH)
    {

        Moisture_LED_state = !Moisture_LED_state;
        digitalWrite(Moisture_LED, Moisture_LED_state);
        Serial.println("Soil Moisture Detected");
    }
    if (digitalRead(PIR_1) == HIGH)
    {

        PIR_LED_1_state = !PIR_LED_1_state;
        digitalWrite(LED_BUILTIN, PIR_LED_1_state);
        Serial.println("PIR-1 Motion Detected ");
    }
    if (digitalRead(PIR_2) == HIGH)
    {
        PIR_LED_2_state = !PIR_LED_2_state;
        digitalWrite(PIR_LED_2, PIR_LED_2_state);
        Serial.println("PIR-2 Motion Detected ");
    }
}