static const char _SERIAL_MANAGER_SKIP_CHARACTERS[] = { '\r' };

template<int T>
class InputManager {
    char buffer[T];
    int currentBufferIndex = 0;

protected:
    virtual void onSetup() {}
    virtual void onLoop() {}

    virtual int availableCharsCount() = 0;
    virtual char readChar() = 0;

    virtual void print(const char* data) = 0;
    virtual void print(int n) = 0;

private:
    void println(const char* data) {
        print(data);
        print("\n");
    }

    void println(int n) {
        print(n);
        print("\n");
    }

    static bool isCharSkipped(char c) {
        for(uint i = 0; i < sizeof(_SERIAL_MANAGER_SKIP_CHARACTERS); i++) {
            if(_SERIAL_MANAGER_SKIP_CHARACTERS[i] == c) {
                return true;
            }
        }

        return false;
    }

public:

    void resetBuffer() {
        for(int i = 0; i < T; i++) {
            buffer[i] = 0;
        }
        currentBufferIndex = 0;
    }

    void setup() {
        resetBuffer();
        onSetup();
    }

    void loop() {
        int availableToRead = availableCharsCount();
        if(availableToRead > 0) {
            for(int i = 0; i < availableToRead; i++) {
                // T - 1 to leave space for the null terminator
                if (currentBufferIndex >= T - 1) {
                    print("ERROR: Packet too large for the Serial buffer. Max size: ");
                    println(T);
                    resetBuffer();
                }

                char c = readChar();
                if(c == '\n') {
                    Parser::ParseCommand(buffer);
                    resetBuffer();
                }
                else {
                    if(!isCharSkipped(c)) {
                        buffer[currentBufferIndex++] = c;
                    }
                }


            }
        }
    }
};