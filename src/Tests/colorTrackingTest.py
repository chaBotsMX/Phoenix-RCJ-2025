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

yellow_threshold = (82, 100, -40, 14, 24, 77)
blue_threshold = ((32, 49, -36, 36, -87, -13))

roi = (160, 0, 160, 240)

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

    for blob in img.find_blobs([yellow_threshold, blue_threshold], pixels_threshold=1, area_threshold=1):
        if blob.area() > maxArea:
            maxArea = blob.area()
            maxBlob = blob

    if maxBlob and maxArea > 1000:
        blobX = maxBlob.cx()
        blobY = maxBlob.cy()

        # Send UART only if 100 ms passed
        if time.ticks_diff(time.ticks_ms(), last_send_time) >= 100:
            uart.write(bytes([255]))
            uart.write(bytes([blobX]))
            uart.write(bytes([blobY]))
            uart.write(bytes([254]))

            print(blobX, blobY)

            last_send_time = time.ticks_ms()  # reset timer

        img.draw_rectangle(maxBlob.rect())
        img.draw_cross(blobX, blobY)
        #print(maxArea)
