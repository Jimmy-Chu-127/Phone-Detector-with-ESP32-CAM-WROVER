from ECE16Lib.Communication import Communication
import cv2
import time
import numpy as np
import urllib.request


thres = 0.45            # Threshold to detect object
nms_threshold = 0.2     # Threshold to not duplicate
detection = -1

# cap = cv2.VideoCapture(0)
# cap.set(3,1280)
# cap.set(4,720)
# cap.set(10,150)

# getting video feed
# url = 'http://192.168.0.24/640x480.jpg'
url = 'http://192.168.4.1/640x480.jpg'

# setting up Bluetooth Communication
comms = Communication("COM7", 115200)
comms.clear()

classNames = []
classFile = 'coco.names'
# trackerPath = 'tracker.txt'

with open(classFile,'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')

# trackerFile = open(trackerPath, 'r+')

# print(classNames)
configPath = 'ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
weightsPath = 'frozen_inference_graph.pb'

net = cv2.dnn_DetectionModel(weightsPath, configPath)
net.setInputSize(320, 320)
net.setInputScale(1.0 / 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)

try:
    while True:
        # getting img
        imgRespond = urllib.request.urlopen(url)
        imgnp = np.array(bytearray(imgRespond.read()), dtype=np.uint8)
        img = cv2.imdecode(imgnp, -1)

        classIds, confs, bbox = net.detect(img, confThreshold=thres)
        bbox = list(bbox)
        confs = list(np.array(confs).reshape(1, -1)[0])
        confs = list(map(float, confs))
        # print(type(confs[0]))
        # print(confs)

        indices = cv2.dnn.NMSBoxes(bbox, confs, thres, nms_threshold)
        # print(indices)

        for i in indices:
            box = bbox[i]
            box = list(box)

            x, y, w, h = int(box[0]), int(box[1]), int(box[2]), int(box[3])
            cv2.rectangle(img, (x, y), (x + w, h + y), color=(0, 255, 0), thickness=2)

            # print(classNames)
            # print(type(classNames))

            classIds = list(classIds)
            # print(classIds)

            cv2.putText(img, classNames[classIds[i] - 1], (x + 10, y + 30),
                        cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)

        for i in indices:
            if(classNames[classIds[i] - 1] == "cell phone"):
                detection = 1
            else:
                pass

        if(detection == 1):
            comms.send_message("detected")
            print("detected")
            detection = -1

            # trackerNum = int(trackerFile.read(), base = 10)
            # trackerNum = trackerNum + 1
            # print(trackerNum)
            #
            # with open(trackerPath, 'w') as file:
            #     pass
            #
            # trackerFile.write(str(trackerNum))

        elif(detection == -1):
            comms.send_message("undetected")
            print("undetected")

        cv2.imshow("Output", img)

        k = cv2.waitKey(30) & 0xff
        if k == 27:
            break


except(Exception, KeyboardInterrupt) as e:
    print(e)
finally:
    print("Closing Connection.")
    comms.close()
    cv2.destroyAllWindows()