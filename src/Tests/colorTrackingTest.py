import sensor, time, math

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()

yellow_threshold = ((12, 28, -66, 16, 38, 6))

while True:
    clock.tick()
    img = sensor.snapshot()

    for blob in img.find_blobs([yellow_threshold], pixels_threshold=100, area_threshold=300, merge=True):
        #print("pixels:", blob.pixels(), "area:", blob.area())
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())

        img_center_x = img.width() // 2
        img_center_y = img.height() // 2
        dx = blob.cx() - img_center_x
        dy = blob.cy() - img_center_y
        angle = math.degrees(math.atan2(dy, dx))

        print("angle:", angle)
