from picamera2 import Picamera2
import cv2
import numpy as np
from gpiozero import Robot, PWMOutputDevice
import time

Initialize camera

picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"size": (640, 480), "format": "RGB888"}))
picam2.start()
time.sleep(2)

Optional: disable manual tuning if needed

picam2.set_controls({
"AwbMode": 0,
"ColourGains": (1.8, 1.3),
"ExposureTime": 30000,
"AnalogueGain": 1.0,
})

Motor setup with enable pins

robot = Robot(left=(22, 27), right=(17, 23))
ena = PWMOutputDevice(24)  # Left motor enable
enb = PWMOutputDevice(25)  # Right motor enable

Speed setup

base_speed = 0.7
turn_speed = 0.5

Yellow HSV range (adjust if needed)

lower_yellow = np.array([18, 90, 90])
upper_yellow = np.array([35, 255, 255])

Frame dimensions

image_width, image_height = 640, 480
center_x, center_y = image_width // 2, image_height // 2

Center box (target zone)

box_width, box_height = 160, 120
box_left = center_x - box_width // 2
box_top = center_y - box_height // 2
box_right = center_x + box_width // 2
box_bottom = center_y + box_height // 2

print("🟡 Yellow Object Tracking with HSV Debug & Center Box Control")

try:
while True:
frame = picam2.capture_array()
hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)

# Create yellow mask  
    mask = cv2.inRange(hsv, lower_yellow, upper_yellow)  

    # Find contours  
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)  
    largest = max(contours, key=cv2.contourArea, default=None)  

    if largest is not None and 250 < cv2.contourArea(largest) < 100000:  
        x, y, w, h = cv2.boundingRect(largest)  
        obj_x, obj_y = x + w // 2, y + h // 2  

        # Get average HSV value in the object's bounding box  
        roi = hsv[y:y+h, x:x+w]  
        avg_hsv = cv2.mean(roi)[:3]  # (H, S, V)  

        print(f"📸 HSV Mean in ROI: H={int(avg_hsv[0])}, S={int(avg_hsv[1])}, V={int(avg_hsv[2])}")  

        # Draw bounding box and object center  
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)  
        cv2.circle(frame, (obj_x, obj_y), 5, (255, 0, 0), -1)  

        # Draw center box  
        cv2.rectangle(frame, (box_left, box_top), (box_right, box_bottom), (0, 255, 255), 2)  
        cv2.putText(frame, "Target Zone", (box_left, box_top - 10),  
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 1)  

        # Movement decision  
        if box_left < obj_x < box_right and box_top < obj_y < box_bottom:  
            robot.stop()  
            ena.value = enb.value = 0  
            print("✅ Object in center zone - Stop")  
        elif obj_x < box_left:  
            robot.right(turn_speed)  
            ena.value = enb.value = turn_speed  
            print("⬅️ Move Left")  
        elif obj_x > box_right:  
            robot.left(turn_speed)  
            ena.value = enb.value = turn_speed  
            print("➡️ Move Right")  
        elif obj_y < box_top:  
            robot.forward(base_speed)  
            ena.value = enb.value = base_speed  
            print("⬆️ Move Forward")  
        elif obj_y > box_bottom:  
            robot.backward(base_speed * 0.6)  
            ena.value = enb.value = base_speed * 0.6  
            print("⬇️ Move Backward")  
    else:  
        robot.stop()  
        ena.value = enb.value = 0  
        print("❌ No valid object detected")  

    # Display feed and mask (for debug)  
    cv2.imshow("Tracking View", frame)  
    cv2.imshow("Yellow Mask", mask)  

    if cv2.waitKey(1) & 0xFF == ord('q'):  
        break

finally:
robot.stop()
ena.close()
enb.close()
cv2.destroyAllWindows()
picam2.stop() change code to control using keyboard keys move forward and backward and left  right by seeing camra
