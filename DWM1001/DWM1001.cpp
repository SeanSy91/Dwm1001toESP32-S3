#include "DWM1001.h"


DWM1001::DWM1001(HardwareSerial& port) : serialPort(port), point_Count(0) {}

void DWM1001::begin(long baudRate) {
    serialPort.begin(baudRate);
    vTaskDelay(3000);
    Serial.println("DWM1001 Serial Initialized");
}

void DWM1001::InitialStep(){
        uint32_t count = millis();
        Serial.println("Initial Step Start");

        serialPort.write(0x0d);
        while(millis() - count < 100);

        serialPort.write(0x0d);
        count = millis();
        while(millis() - count < 1000);

        serialPort.write('l');
        count = millis();
        while(millis() - count < 100);

        serialPort.write('e');
        count = millis();
        while(millis() - count < 100);

        serialPort.write('s');
        count = millis();
        while(millis() - count < 100);

        serialPort.write(0x0d);
        count = millis();
        while(millis() - count < 100);

        Serial.println("Initial Step End");
}

int DWM1001::extractData(const char* input) {
    int count = 0;
    const char* ptr = input;

    while (count < MAX_DATA) {
        if (sscanf(ptr, "%9[^[][%f,%f,%f]=%f", 
                   Point[count].id, &Point[count].x, &Point[count].y, 
                   &Point[count].z, &Point[count].d) == 5) {
            count++;
        }
        ptr = strstr(ptr, "]="); 
        if (!ptr) break;
        ptr = strstr(ptr, " ");
        if (!ptr) break;
        ptr += 1; // "]=" 다음으로 이동
    }
    point_Count = count;
    return count;
}
bool DWM1001::isValidEstBlock(const char* start, const char* end) {
    if (!start || !end || start >= end) return false;
    for (const char* ptr = start; ptr < end; ptr++) {
        if ((*ptr < '0' || *ptr > '9') && *ptr != '.' && *ptr != ',') {
            return false;
        }
    }
    return true;
}

int DWM1001::parseEstValues(const char* input, double* values, int* count) {
    const char* start = strstr(input, "est[");
    while (start) {
        start += 4; // Move past "est["
        const char* end = strchr(start, ']');
        if (!end || !isValidEstBlock(start, end)) {
            start = strstr(start, "est[");
            continue;
        }

        char buffer[256];
        strncpy(buffer, start, end - start);
        buffer[end - start] = '\0';

        char* token = strtok(buffer, ",");
        *count = 0;
        while (token) {
            values[(*count)++] = atof(token);
            token = strtok(NULL, ",");
        }
        start = strstr(end, "est[");
    }
    return 1;
}

int DWM1001::readSerialData() {
        int count = 0, count1 = 0  ;
    if (serialPort.available()) {
        char byte = serialPort.read();
        buffer += byte;
        if (byte == '\n') {
                // count = extractData(buffer.c_str());
                count1 = parseEstValues(buffer.c_str(),Point->value, &count);
                buffer = "";
        }
    }
    return count1;
}

void DWM1001::printData() {
    Serial.printf("X : %.2f, Y : %.2f, Z : %.2f, accuracy : %.f\n", Point->value[0], Point->value[1], Point->value[2], Point->value[3]);
}

int DWM1001::CountData(){
        return point_Count;
}
