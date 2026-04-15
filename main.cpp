#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
#include <avr/wdt.h> // Watchdog için

// --- HAL (Hardware Abstraction Layer) ---
#define LED_PIN 13
#define EMERGENCY_BUTTON_PIN 2 // Shutdown Pin
#define EMERGENCY_LED 3

void handleEmergency(){
    digitalWrite(EMERGENCY_LED, HIGH);
    digitalWrite(LED_PIN, LOW);
    vTaskSuspendAll();
}

void HAL_Init(){
    pinMode(LED_PIN, OUTPUT);
    pinMode(EMERGENCY_BUTTON_PIN, INPUT_PULLUP);
    pinMode(EMERGENCY_LED, OUTPUT);
    Serial.begin(9600);

    attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON_PIN), handleEmergency, FALLING);
}

void LED_State(bool state){
    digitalWrite(LED_PIN, state);
}

//RTOS Objects
SemaphoreHandle_t xSerialMutex;
QueueHandle_t xDataQueue;

void vSensorTask(void *pvParameters){
    int sensorValue = 0;
    for(;;){
        sensorValue = analogRead(A0);//Potentiometer Value
        xQueueSend(xDataQueue, &sensorValue, 10);
        LED_State(!digitalRead(LED_PIN));
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void vReporterTask(void *pvParameters){
    int recevedValue = 0;
    for(;;){
        if(xQueueReceive(xDataQueue, &recevedValue, portMAX_DELAY)){
            //Lock Serial Port
            if(xSemaphoreTake(xSerialMutex, portMAX_DELAY)){
                Serial.print("Sensor Value: ");
                Serial.println(recevedValue);
                xSemaphoreGive(xSerialMutex);//Unlock Serial Port
            }
        }
    }
}

void vWatchdogTask(void *pvParameters){
    for(;;){
        wdt_reset();//Send reset signal to watchdog
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup(){
    HAL_Init();

    //Mutex ve Queue create
    xSerialMutex = xSemaphoreCreateMutex();
    xDataQueue = xQueueCreate(5, sizeof(int));

    if(xSerialMutex != NULL && xDataQueue != NULL){
        //Görevler (Task, Name, Stack, Parameter, Priority, Handle)
        xTaskCreate(vSensorTask, "SENSOR", 128, NULL, 2, NULL);
        xTaskCreate(vReporterTask, "REPORTER", 128, NULL, 2, NULL);
        xTaskCreate(vWatchdogTask, "WATCHDOG", 128, NULL, 2, NULL);  
    }

    wdt_enable(WDTO_2S); // 2 saniyelik watchdog
}

void loop(){
    //Empty
}