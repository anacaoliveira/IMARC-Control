class Fans {
    private:
    int pin;
    bool state;

    public:
    Fans(int pin) {
        this->pin = pin;
        pinMode(pin, OUTPUT);
        state = false;
    }
    void on() {
        digitalWrite(this->pin, HIGH);
        state = true;
        if (DEBUG) {
            Serial.println("• On");
        }
    }
    void off() {
        digitalWrite(this->pin, LOW);
        state = false;
        if (DEBUG) {
            Serial.println("• Off");
        }
    }
    void toggle(bool on) {
        if (on) this->on();
        else this->off();
    }
    bool isOn() {
        return state;
    }

};