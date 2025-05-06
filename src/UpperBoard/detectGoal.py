import sensor, time
from pyb import UART

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.set_auto_whitebal(False)
#sensor.set_contrast(2)
#sensor.set_brightness(0)
#sensor.set_saturation(3)
clock = time.clock()

uart = UART(3, 115200)

yellow_threshold = (26, 97, -40, 80, 56, 116)
blue_threshold = (0, 49, -4, 20, -49, -15)

roi = (70, 0, 250, 240)

blobX = 0
blobY = 0

sensor.set_windowing(roi)

while True:
    clock.tick()
    img = sensor.snapshot()

    maxArea = 0
    maxBlob = None

    for blob in img.find_blobs([yellow_threshold], pixels_threshold=10, area_threshold=10):
        if blob.area() > maxArea:
            maxArea = blob.area()
            maxBlob = blob

    for blob in img.find_blobs([blue_threshold], pixels_threshold=15, area_threshold=15):
        if blob.area() > maxArea:
            maxArea = blob.area()
            maxBlob = blob

    if maxBlob:
        blobX = maxBlob.cx()
        blobY = maxBlob.cy()

        uart.write(255)
        uart.write(blobX)
        uart.write(blobY)
        uart.write(blobX + blobY)
        uart.write(244)

        img.draw_rectangle(maxBlob.rect())
        img.draw_cross(blobX, blobY)
        print(blobX, blobY)
