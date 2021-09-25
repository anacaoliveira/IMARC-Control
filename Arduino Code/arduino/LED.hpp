class LED {
    private:
    int pin;
    bool state;

    public:
    LED(int pin) {
        this->pin = pin;
        pinMode(pin, OUTPUT);
        state = false;
    }
    void on() {
        digitalWrite(this->pin, HIGH);
        state = true;
    }
    void off() {
        digitalWrite(this->pin, LOW);
        state = false;
    }
    void toggle(bool on) {
        if (on) this->on();
        else this->off();
    }
    bool isOn() {
        return state;
    }
    void percent(float p) {
        analogWrite(this->pin, (int) (255.0 * p));
    }

};