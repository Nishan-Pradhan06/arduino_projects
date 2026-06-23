// L298 Motor Driver pins
#define IN1 6
#define IN2 8
#define IN3 9
#define IN4 10

char command;

void setup()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    Serial.begin(9600);

    stopCar();

    Serial.println("Car Control Ready");
    Serial.println("F=Forward B=Backward L=Left R=Right S=Stop");
}

void loop()
{

    if (Serial.available())
    {

        command = Serial.read();

        switch (command)
        {

        case 'F':
        case 'f':
            forward();
            break;

        case 'B':
        case 'b':
            backward();
            break;

        case 'L':
        case 'l':
            left();
            break;

        case 'R':
        case 'r':
            right();
            break;

        case 'S':
        case 's':
            stopCar();
            break;
        }
    }
}

// Move forward
void forward()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

// Move backward
void backward()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

// Turn left
void left()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

// Turn right
void right()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

// Stop
void stopCar()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}