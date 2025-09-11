import cv2
import numpy as np
from picamera2 import Picamera2
import time

# Initialize PiCamera2
picam2 = Picamera2()
picam2.preview_configuration.main.size = (640, 480)
picam2.preview_configuration.main.format = "RGB888"
picam2.configure("preview")
picam2.start()
time.sleep(1)

# Define HSV color ranges for real-world colors
color_ranges = {
    "Red": [
        (np.array([0, 120, 70]), np.array([10, 255, 255])),
        (np.array([170, 120, 70]), np.array([180, 255, 255]))
    ],
    "Orange": [(np.array([10, 100, 100]), np.array([25, 255, 255]))],
    "Yellow": [(np.array([25, 100, 100]), np.array([35, 255, 255]))],
    "Green": [(np.array([36, 100, 100]), np.array([85, 255, 255]))],
    "Cyan": [(np.array([86, 100, 100]), np.array([100, 255, 255]))],
    "Blue": [(np.array([101, 100, 100]), np.array([130, 255, 255]))],
    "Purple": [(np.array([131, 100, 100]), np.array([160, 255, 255]))],
    "Brown": [(np.array([10, 100, 20]), np.array([20, 255, 200]))],
    "Skin": [(np.array([0, 30, 60]), np.array([20, 150, 255]))],
    "White": [(np.array([0, 0, 200]), np.array([180, 50, 255]))],
    "Gray": [(np.array([0, 0, 40]), np.array([180, 20, 200]))],
    "Black": [(np.array([0, 0, 0]), np.array([180, 255, 40]))]
}

print("🌈 Real-World Multi-Color Detection Started – Press 'q' to quit")

try:
    while True:
        frame = picam2.capture_array()
        hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)

        for color_name, ranges in color_ranges.items():
            mask = np.zeros(hsv.shape[:2], dtype=np.uint8)

            for lower, upper in ranges:
                mask |= cv2.inRange(hsv, lower, upper)

            contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:
                area = cv2.contourArea(cnt)
                if area > 600:  # adjust area threshold for sensitivity
                    x, y, w, h = cv2.boundingRect(cnt)
                    cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                    cv2.putText(frame, color_name, (x, y - 10),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

        cv2.imshow("All Color Detection", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    print("\n🛑 Detection stopped by user.")

finally:
    picam2.stop()
    cv2.destroyAllWindows()
