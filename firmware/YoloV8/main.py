# Importing OpenCV library for computer vision tasks
import cv2
# Importing YOLO model from Ultralytics for object tracking
from ultralytics import YOLO

def main():

    # Loading YOLO model(small here) with the specified weights file.
    # We can use the "yolov8l.pt" model weights but it would demand significant computational power, such as GPUs.
    model = YOLO("yolov8s.pt")

    # Looping over frames captured from the video source
    for results in model.track(source=1, show=True, stream=True):

        # Extracting the original frame from the results
        frame = results.orig_img

        # Extracted bounding box coordinates, detect classes and their ids with confidence score
        boxes = results.boxes.xyxy.tolist()
        classes = results.boxes.cls.tolist()
        names = results.names
        confidences = results.boxes.conf.tolist()

        # Iterating through each detected object
        for box, cls, conf in zip(boxes, classes, confidences):
            x1, y1, x2, y2 = box
            width = x2 - x1
            height = y2 - y1

            # Calculating center of the bounding box
            center = (x1 + int((width)/2), y1 + int((height)/2))
            cx1 = int(center[0])
            cx2 = int(center[1])
            confidence = round(conf,4)
            detected_class = cls
            name = names[int(cls)]

            # Drawing bounding box and center on the frame
            cv2.rectangle(frame, (int(x1), int(y1)),(int(x2), int(y2)), (255, 255, 0), 2)
            cv2.circle(frame, (cx1,cx2), 6, (0, 0, 255), -1)
            text = f"name: {name}, confidence: {confidence}"
            cv2.putText(frame, text, (int(x1), int(y1)),cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

            # Printing information about the detected object
            print(f"Bounding box coordinates of class {name} are {x1}, {y1}, {x2}, {y2}")
            print(f"Center coordinates of class {name} are {center}")

        cv2.imshow("frame",frame)
        if(cv2.waitKey(30)==27):
            break

if __name__ == "__main__":
    main()