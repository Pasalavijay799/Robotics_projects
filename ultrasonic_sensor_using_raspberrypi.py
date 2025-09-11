import RPi.GPIO as GPIO
import time

TRIG = 18
ECHO = 24
LOG_FILE = "/home/pi0/distance_log.txt"

GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

def get_distance():
    GPIO.output(TRIG, False)
    time.sleep(0.01)  # small delay for stabilization

    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    timeout_start = time.time()

    # Wait for ECHO to go HIGH
    while GPIO.input(ECHO) == 0:
        if time.time() - timeout_start > 0.05:  # 50ms timeout
            return -1
    pulse_start = time.time()

    timeout_start = time.time()

    # Wait for ECHO to go LOW
    while GPIO.input(ECHO) == 1:
        if time.time() - timeout_start > 0.05:
            return -1
    pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start
    distance = round(pulse_duration * 17150, 2)  # in cm
    return distance

try:
    print("📡 Starting continuous distance measurement...\n")
    with open(LOG_FILE, "a") as f:
        for i in range(50):  # Increase number of readings
            dist = get_distance()
            timestamp = time.strftime('%Y-%m-%d %H:%M:%S')

            if dist == -1:
                log = f"{timestamp} - ❌ Timeout: No echo received\n"
            else:
                log = f"{timestamp} - ✅ Distance: {dist} cm\n"

            print(log.strip())
            f.write(log)
            # 🧠 No time.sleep here — blazing fast loop

except KeyboardInterrupt:
    print("\n🛑 Measurement stopped by user.")

finally:
    GPIO.cleanup()
    print("🔌 GPIO cleanup complete.")
