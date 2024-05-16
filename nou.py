import cv2
import zxingcpp
from PIL import Image
import os
import shutil
def decode_barcodes_in_folder(folder_path):
    # Read all files in the directory
    image_files = [os.path.join(folder_path, f) for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]

    # Read barcodes from each image
    for image_path in image_files:
        # Open the image file
        img = Image.open(image_path)

        # Read barcodes from the image
        barcodes = zxingcpp.read_barcodes(img)

        # Print barcode information
        if len(barcodes) != 0:
            # Iterate through each barcode found
            for barcode in barcodes:
                # Extract the text from the barcode
                barcode_text = barcode.text

                # Remove the "Project_" prefix if present
                if barcode_text.startswith("Project_"):
                    barcode_text = barcode_text[len("Project_"):]

                # Remove the surrounding double quotes if present
                barcode_text = barcode_text.strip('"')

                # Print the modified barcode text
                print(barcode_text)

            return True  # Return True if barcode is successfully decoded

    return False  # Return False if no barcode is successfully decoded

def capture_frames_and_decode(output_dir):
    # Create the output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)

    # Open the default camera (0 represents the default camera)
    cap = cv2.VideoCapture(0)

    # Check if the camera is opened successfully
    if not cap.isOpened():
        print("Error: Failed to open camera.")
        return

   # print("Camera opened successfully. Press 'q' to quit.")

    # Counter for saved frames
    frame_count = 0

    # Read and save frames from the camera
    while cap.isOpened():
        ret, frame = cap.read()  # Read a frame from the camera

        # Check if frame is read successfully
        if not ret:
            print("Error: Failed to read frame.")
            break

        # Save the frame to the output directory
        frame_path = os.path.join(output_dir, f"frame_{frame_count}.jpg")
        cv2.imwrite(frame_path, frame)
        #print(f"Saved frame {frame_count}")

        # Increment frame count
        frame_count += 1

        # Call the function to decode barcodes in the specified folder
        if decode_barcodes_in_folder(output_dir):
            break  # Exit the loop if barcode is successfully decoded

        # Display the frame
        cv2.imshow('Camera', frame)

        # Check for 'q' key to quit the loop
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the camera and close OpenCV windows
    cap.release()
    cv2.destroyAllWindows()

    # Delete all images in the folder
    delete_images(output_dir)

def delete_images(folder_path):
    # Remove all files in the directory
    for filename in os.listdir(folder_path):
        file_path = os.path.join(folder_path, filename)
        try:
            if os.path.isfile(file_path):
                os.unlink(file_path)
        except Exception as e:
            print(f"Error deleting file {file_path}: {e}")

# Directory to save captured frames
output_dir = r"C:\Users\karim\OneDrive\Bureau\document\projet C++\2a2-unityforce-integration-finale\qrprojet"

# Call the function to capture frames and decode barcodes
capture_frames_and_decode(output_dir)
