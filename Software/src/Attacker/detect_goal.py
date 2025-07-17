import sensor, time
from pyb import UART

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
#sensor.set_contrast(3)
#sensor.set_brightness(3)
#sensor.set_saturation(3)
clock = time.clock()

yellow_threshold = (39, 98, -16, 11, 40, 113)
blue_threshold = (27, 100, -52, 127, -128, -19)

roi = (0, 90, 320, 90)

blobX = 0
blobY = 0
uart = UART(3, 19200, timeout_char=200)

last_send_time = time.ticks_ms()

sensor.set_windowing(roi)

while True:
    clock.tick()
    img = sensor.snapshot()

    maxArea = 0
    maxBlob = None

    for blob in img.find_blobs([yellow_threshold, blue_threshold], pixels_threshold=5, area_threshold=5):
        if blob.area() > maxArea:
            maxArea = blob.area()
            maxBlob = blob

    if maxBlob is not None and maxArea > 100:
        blobX = maxBlob.cx()
        blobY = maxBlob.cy()

        img.draw_rectangle(maxBlob.rect())
        img.draw_cross(blobX, blobY)
    else:
        blobX = 500
        blobY = 250

    print(blobX, blobY)
    uart.write(bytes([255]))
    uart.write(bytes([int(blobX / 2)]))
    uart.write(bytes([blobY]))
