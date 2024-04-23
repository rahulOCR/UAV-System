import cv2
import numpy as np
import math

# Function to update HUD position based on the mouse cursor
def update_hud(event, x, y, flags, param):
    global hud_pos
    if event == cv2.EVENT_MOUSEMOVE:
        hud_pos = (x, y)


tracker = cv2.TrackerMedianFlow_create()
# Initialize the HUD position
hud_pos = (0, 0)

# Open a video capture from the camera (usually 0 for the default camera)
# cap = cv2.VideoCapture(0)
#############################################################################################
pipeline = (
    "udpsrc port=4444 ! application/x-rtp,media=video,clock-rate=90000,encoding-name=H264,payload=96 ! "
    "rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! "
    "video/x-raw,format=BGR ! videoconvert ! video/x-raw,format=BGR ! appsink"
)

# Replace 'PORT' with the port number specified in your sender's pipeline
cap = cv2.VideoCapture(0)
#cap = cv2.VideoCapture(pipeline, cv2.CAP_GSTREAMER)
#############################################################################################

# Create a window to display the camera stream
cv2.namedWindow('Camera Stream', cv2.WND_PROP_FULLSCREEN)
cv2.setWindowProperty('Camera Stream', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)

# Set the HUD color and line thickness
hud_color_unlocked = (0, 255, 0)  # Green color in BGR format
hud_color_locked = (0, 0, 255)  # Red color in BGR format
hud_thickness = 2
is_lock = False

# Main loop for updating and displaying the camera stream with HUD
cv2.setMouseCallback('Camera Stream', update_hud)
font = cv2.FONT_HERSHEY_SIMPLEX
font_scale = 1
font_thickness = 2
text_position_lock = (10, 30)  # Adjust position as needed
cross_size = 10
radius  = 100
width, height = 800, 600
center = (width // 4, height // 2)

while True:
    # Read a frame from the camera
    ret, frame = cap.read()
    for angle in range(0, 360, 45):
        xx = int(center[0] + radius * math.cos(math.radians(angle)))
        yy = int(center[1] + radius * math.sin(math.radians(angle)))
        cv2.line(frame, center, (xx, yy), (0, 255, 0), 2)
        text_position = (xx - 20, yy + 15)
        cv2.putText(frame, str(angle) + "°", text_position, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

    # Get the current mouse position
    x, y = hud_pos
    bounding_box = (x - 50, y - 50, 100, 100) 
    

    # Draw a cross marker at the current mouse position
    

    # cv2.line(frame, (x - cross_size, y), (x + cross_size, y), hud_color, hud_thickness)
    # cv2.line(frame, (x, y - cross_size), (x, y + cross_size), hud_color, hud_thickness)

    if is_lock:

        # Initialize the tracker with the first frame and bounding box
        if not tracker.init(frame, bounding_box):
            print("Tracker initialization failed!")

        # Update the tracker with the current frame
        ret, bounding_box = tracker.update(frame)

        if ret:
            # Draw the tracking result on the frame
            p1 = (int(bounding_box[0]), int(bounding_box[1]))
            p2 = (int(bounding_box[0] + bounding_box[2]), int(bounding_box[1] + bounding_box[3]))
            cv2.rectangle(frame, p1, p2, (0, 255, 0), 2, 1)


        cv2.line(frame, (0, y), (x - cross_size, y), hud_color_locked, hud_thickness)
        cv2.line(frame, (x + cross_size, y), (frame.shape[1], y), hud_color_locked, hud_thickness)
        cv2.line(frame, (x, 0), (x, y - cross_size), hud_color_locked, hud_thickness)
        cv2.line(frame, (x, y + cross_size), (x, frame.shape[0]), hud_color_locked, hud_thickness)
        
        cv2.putText(frame, "TARGET LOCKED", text_position_lock, font, font_scale, hud_color_locked, font_thickness)
    else:
        cv2.line(frame, (0, y), (x - cross_size, y), hud_color_unlocked, hud_thickness)
        cv2.line(frame, (x + cross_size, y), (frame.shape[1], y), hud_color_unlocked, hud_thickness)
        cv2.line(frame, (x, 0), (x, y - cross_size), hud_color_unlocked, hud_thickness)
        cv2.line(frame, (x, y + cross_size), (x, frame.shape[0]), hud_color_unlocked, hud_thickness)
        
        # cv2.putText(frame, "TARGET RELEASED", text_position, font, font_scale, hud_color_unlocked, font_thickness)

    # Display the frame with the HUD
    cv2.imshow('Camera Stream', frame)

    # Exit the loop when 'q' is pressed
    key = cv2.waitKey(1) & 0xFF

    if key == ord('q'):
        break

    
    # Press 'l' to toggle the lock status
    if key == ord('l'):
        is_lock = not is_lock
        # print("lock",is_lock)
    
    elif key == 171:
        cross_size += 10
        # print(cross_size)

    elif key == 173:
        cross_size -= 10
    

# Release resources and close the windows
cap.release()
cv2.destroyAllWindows()


# Shadow text
# import cv2
# import numpy as np

# # Create a blank image with a white background
# width, height = 400, 200
# image = np.ones((height, width, 3), dtype=np.uint8) * 255

# # Set the text, position, and font properties
# text = "Hello World"
# font = cv2.FONT_HERSHEY_SIMPLEX
# font_scale = 2
# font_color = (0, 0, 255)  # Red color in BGR format
# font_thickness = 2
# shadow_offset = 2  # Adjust to control the shadow offset

# # Get the size of the text
# (text_width, text_height), baseline = cv2.getTextSize(text, font, font_scale, font_thickness)

# # Calculate the position for the text and shadow
# x = (width - text_width) // 2
# y = (height + text_height) // 2

# # Calculate the shadow position
# shadow_x = x + shadow_offset
# shadow_y = y + shadow_offset

# # Draw the shadow text
# cv2.putText(image, text, (shadow_x, shadow_y), font, font_scale, (0, 0, 0), font_thickness)

# # Draw the main text
# cv2.putText(image, text, (x, y), font, font_scale, font_color, font_thickness)

# # Display the image
# cv2.imshow('Text with Shadow', image)
# cv2.waitKey(0)
# cv2.destroyAllWindows()






    






