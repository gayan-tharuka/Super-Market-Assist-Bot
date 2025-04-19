"""
Python Communication Middleware
- Receives camera stream from ESP32-CAM
- Detects and decodes QR codes
- Calculates bill total
- Sends information to ESP32 board for LCD display
"""

import cv2
import numpy as np
import pyzbar.pyzbar as pyzbar
import urllib.request
import socket

# Font for display
font = cv2.FONT_HERSHEY_PLAIN

# ESP32-CAM stream URL
url = 'http://192.168.1.29/'  # Update with your ESP32-CAM IP
cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)

# ESP32 Board connection details
ESP32_IP = '192.168.1.30'  # Update with your ESP32 board IP
PORT = 80

def send_message(message):
    """Send message to ESP32 board over TCP socket"""
    try:
        # Create a socket object
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Connect to ESP32
        client_socket.connect((ESP32_IP, PORT))
        print("Connected to ESP32")
        
        # Send message to ESP32
        client_socket.sendall((message + '\n').encode('utf-8'))
        print(f"Sent: {message}")
        
        # Receive acknowledgment from ESP32
        response = client_socket.recv(1024).decode('utf-8')
        print(f"ESP32 Response: {response}")
        
    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Close the socket
        client_socket.close()

# Variables for QR code processing
prev = ""  # Stores previous QR code to avoid duplicates
pres = ""  # Stores current QR code
item_value = ""  # Decoded QR value

# Product inventory - [Product Code, Price]
inventory = [
    ["ABC", 200],
    ["ACB", 300],
    ["BAC", 450],
    ["BCA", 350],
    ["CAB", 550],
    ["CBA", 600]
]

# Valid QR code items
items = [
    "ABC00123", "ABC00132", "ABC00213", "ABC00231", "ABC00312", "ABC00321",
    "ACB00123", "ACB00132", "ACB00213", "ACB00231", "ACB00312", "ACB00321",
    "BAC00123", "BAC00132", "BAC00213", "BAC00231", "BAC00312", "BAC00321",
    "BCA00123", "BCA00132", "BCA00213", "BCA00231", "BCA00312", "BCA00321",
    "CAB00123", "CAB00132", "CAB00213", "CAB00231", "CAB00312", "CAB00321", "CAB001324",
    "CBA00123", "CBA00132", "CBA00213", "CBA00231", "CBA00312", "CBA00321"
]            

bill = []  # Stores items in current bill
tot = 0    # Running total
st_tot = ""  # String representation of total

while True:
    try:
        # Retrieve image from ESP32-CAM
        img_resp = urllib.request.urlopen(url + 'cam-hi.jpg', timeout=20)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        frame = cv2.imdecode(imgnp, -1)
        
        # Skip if frame is empty
        if frame is None:
            print("Failed to retrieve the image. Skipping frame.")
            continue

        # Decode QR codes in frame
        decodedObjects = pyzbar.decode(frame)
        for obj in decodedObjects:
            pres = obj.data
            item_value = obj.data.decode("utf-8")
            
            # Only process if new QR code detected
            if prev == pres:
                pass
            else:
                prev = pres  # Update previous code
                print(item_value)  # Debug print
                
                # Extract product code (first 3 chars) and ID
                item = item_value[:3]
                itemId = item_value[5:]

                # Check if item is valid
                if item_value in items:
                    if item_value not in bill:
                        # Add new item to bill
                        bill.append(item_value)
                        
                        # Find item in inventory and add to total
                        for i in range(len(inventory)):
                            if inventory[i][0] == item:
                                add = int(inventory[i][1])
                                tot += add 
                                print(f"Total value: {tot}")
                                st_tot = str(tot)
                                
                                # Send total to ESP32 board
                                send_message("Total: Rs."+st_tot)
                                break
                    else:
                        print("Item already in the bill")
                        send_message("Item already in the bill")
                else:
                    print("Item not found")
                    send_message("Item not found")

            # Display QR code text on frame
            cv2.putText(frame, str(obj.data), (50, 50), font, 2,
                        (255, 0, 0), 3)

        # Display live stream
        cv2.imshow("live transmission", frame)

    except urllib.error.URLError as e:
        print(f"Failed to connect to the server: {e.reason}")
        break
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        break

    # Exit on ESC key
    key = cv2.waitKey(1)
    if key == 27:
        break

# Clean up
cv2.destroyAllWindows()
