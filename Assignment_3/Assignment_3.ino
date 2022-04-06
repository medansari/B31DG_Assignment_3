
#define ERROR_PIN 23   // define error pins for task 8
#define Push_button 22 // define push button pin for task 2
#define SigB 21        // define signal b pin for task 1
#define SigG 15        // define signal generator pin for task 3
#define POT 14         // defining an analog potentiometer pin for task 4
#define exec_time 26   // defining execution time for task 4

const int numPOT_values = 4;  // size of the array for storing potentiometer values
int POT_Value[numPOT_values]; // readings from the POT input
int POT_index;                // POT value indexing
int POT_total = 0;            // POT total

int error_code = 0; // inititialising error_code as an integer equal to 0

unsigned long duration;      // inititialising duration as a unsigned long
unsigned long durationTotal; // inititialising durationTotal as a unsigned long

static const uint8_t queue_5_7_len = 3;
static QueueHandle_t queue_5_7;
static SemaphoreHandle_t mutex;

struct task9_values
{
  int buttonState;     // inititialising buttonState as an integer
  int freq;            // inititialising freq as a float
  int POT_average = 0; // POT average

} t9;

void setup()
{
  // assigning inputs and outputs for all the pins
  pinMode(ERROR_PIN, OUTPUT);
  pinMode(Push_button, INPUT);
  pinMode(SigB, OUTPUT);
  pinMode(exec_time, OUTPUT);
  pinMode(SigG, INPUT);
  pinMode(POT, INPUT);
  // initiallisng serial monitor at the baud rate of 9600
  Serial.begin(115200);
  // initiallising all potentiometer values to be 0
  for (int init_POT = 0; init_POT < numPOT_values; init_POT++)
  {
    POT_Value[init_POT] = 0;
  }
  mutex = xSemaphoreCreateMutex();
  xTaskCreate(
      task1,    // function name
      "Task 1", // task name
      1000,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );

  xTaskCreate(
      task2,    // function name
      "Task 2", // task name
      1000,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );

  xTaskCreate(
      task3,    // function name
      "Task 3", // task name
      2048,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );

  xTaskCreate(
      task4,    // function name
      "Task 4", // task name
      1000,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );

  xTaskCreate(
      task5,    // function name
      "Task 5", // task name
      2048,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );

  xTaskCreate(
      task6,    // function name
      "Task 6", // task name
      1000,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );
  xTaskCreate(
      task7,    // function name
      "Task 7", // task name
      1000,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );

  xTaskCreate(
      task8,    // function name
      "Task 8", // task name
      1000,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );

  xTaskCreate(
      task9,    // function name
      "Task 9", // task name
      1000,     // stack size
      NULL,     // task paramertes
      1,        // task priority
      NULL      // task handle
  );
  queue_5_7 = xQueueCreate(
      queue_5_7_len,
      sizeof(int));
}

void loop()
{
  // put your main code here, to run repeatedly:
}

void task1(void *parameters)
{
  for (;;)
  {
    digitalWrite(SigB, HIGH);             // setting signal b high
    vTaskDelay(0.5 / portTICK_PERIOD_MS); // delay of 50 microseconds
    digitalWrite(SigB, LOW);              // setting signal b low
    vTaskDelay(1);
  }
}

// implementing function for task 2
void task2(void *parameters)
{
  for (;;)
  {
    vTaskDelay(2 / portTICK_PERIOD_MS);        // adding a delay for debouncing
    t9.buttonState = digitalRead(Push_button); // read the state of the button
    vTaskDelay(2 / portTICK_PERIOD_MS);        // adding a delay for debouncing

    vTaskDelay(20);
  }
}

// implementing function for task 3
void task3(void *parameters)
{
  for (;;)
  {
    duration = pulseIn(SigG, HIGH); // reading in pulse duartion

    // if the pulse duration is equal to 0 return
    if (duration == 0)
    {
      duration = 1;
    }

    durationTotal = (duration * 2);        // calculate the total duration
    t9.freq = (1000000 / (durationTotal)); // calculate the frequency
    vTaskDelay(100);
  }
}

// implementing function for task 4
void task4(void *parameters)
{
  for (;;)
  {
    digitalWrite(exec_time, HIGH);                // checking execution time
    POT_total = POT_total - POT_Value[POT_index]; // subtracting from the pot_tatal
    POT_Value[POT_index] = analogRead(POT);       // storing the potentiometer values in the indexes
    digitalWrite(exec_time, LOW);
    vTaskDelay(4.2);
  }
}

// implementing function for task 5
void task5(void *parameters)
{
  for (;;)
  {
    // storing the total potentiometer value
    POT_total = POT_total + POT_Value[POT_index];
    POT_index = POT_index + 1;
    if (POT_index >= numPOT_values)
    {
      POT_index = 0;
    }
    t9.POT_average = POT_total / numPOT_values; // calculating POT average
    xQueueSend(queue_5_7, (void *)&t9.POT_average, 0);
    Serial.println(t9.POT_average);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    vTaskDelay(4.2);
  }
}
// implementing function for task 6
void task6(void *parameters)
{
  for (;;)
  {
    // using for loop to implement following task 1000 times
    for (int i = 0; i < 999; i++)
    {
      __asm__ __volatile__("nop");
    }
    vTaskDelay(10);
  }
}
// implementing function for task 7
void task7(void *parameters)
{
  for (;;)
  {
    xQueueReceive(queue_5_7, (void *)&t9.POT_average, 0);
    Serial.println(t9.POT_average);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // using if statements to check error
    if (t9.POT_average > (4095 / 2))
    {
      error_code = 1; // setting error code to 1
    }
    else
    {
      error_code = 0; // setting error code to 0
    }
    vTaskDelay(33);
  }
}
// implementing function for task 8
void task8(void *parameters)
{
  for (;;)
  {
    if (error_code == 1)
    {
      digitalWrite(ERROR_PIN, HIGH); // setting  led on when error code is 1
    }

    else
    {
      digitalWrite(ERROR_PIN, LOW); // setting  led off otherwise
    }
    vTaskDelay(33);
  }
}
// implementing function for task 9
void task9(void *parameters)
{
  for (;;)
  {
    if (xSemaphoreTake(mutex, 0) == pdTRUE)
    {
      // printing button state, frequency and pot average in a CSV format
      if (digitalRead(Push_button) == HIGH)
      {

        Serial.print(t9.buttonState);
        Serial.print(",");
        Serial.print(t9.freq);
        Serial.print(",");
        Serial.println(t9.POT_average);
        xSemaphoreGive(mutex);
      }
      vTaskDelay(500);
    }
  }
}
