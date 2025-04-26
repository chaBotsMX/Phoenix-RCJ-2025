import sensor, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.set_auto_whitebal(False)
sensor.set_contrast(2)
sensor.set_brightness(0)
sensor.set_saturation(3)
clock = time.clock()

yellow_threshold = (68, 79, -23, 22, 30, 87)

while True:
    clock.tick()
    img = sensor.snapshot()

    for blob in img.find_blobs([yellow_threshold], pixels_threshold=100, area_threshold=300, merge=True):
        #print("pixels:", blob.pixels(), "area:", blob.area())
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
