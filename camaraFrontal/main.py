# OpenMV H7 Plus - front camera
#
# Detects the ball and both goals and sends them to the Teensy over UART3
# (P4 = TX -> Teensy pin 0 / Serial1, P5 = RX).
#
# Message (one line per frame):
#   "<ballDist> <ballAng> <ballArea> <yellowDist> <yellowAng> <blueDist> <blueAng>\n"
#
# To run it without the IDE: Tools -> Save open script to OpenMV Cam (as main.py)

import csi
import time
import pyb
import math
from pyb import UART

EXPOSURE_US = 36664
GAIN_CEILING = 2
BRIGHTNESS = 1
CONTRAST = -3
SATURATION = 10

MIN_CLOSE_AREA = 100
MIN_FAR_AREA = 20
MIN_PIXELS = 15
BALL_MIN_CIRCULARITY = 0.3

GOAL_PIXELS_THRESHOLD = 200
GOAL_AREA_THRESHOLD = 800

REFERENCE_OFFSET_Y = 20

BALL_THRESHOLDS = [
    (30, 100, -10, 127, 15, 127),
    (30, 100, -10, 127, 15, 127),
    (30, 100, -10, 127, 15, 127),
]
YELLOW_THRESHOLDS = [
    (34, 56, 8, 24, 26, 45),
    (38, 68, -7, 27, 27, 53),
]
BLUE_THRESHOLDS = [
    (13, 14, -3, 12, -11, -8),
    (9, 17, -5, 18, -14, -3),
    (13, 19, 5, 8, -18, -3),
]

uart = UART(3, 115200)
led = pyb.LED(3)  # blue LED: blinks every frame while the script is running


def initialize_open():
    cam = csi.CSI()
    cam.reset()
    cam.pixformat(csi.RGB565)
    cam.framesize(csi.QVGA)
    cam.snapshot(time=3000)
    cam.auto_gain(False)
    cam.gainceiling(GAIN_CEILING)
    cam.auto_whitebal(False)
    cam.auto_exposure(False, exposure_us=EXPOSURE_US)
    cam.brightness(BRIGHTNESS)
    cam.contrast(CONTRAST)
    cam.saturation(SATURATION)
    cam.hmirror(True)
    cam.transpose(False)
    return cam


def reference(img):
    return img.width() // 2, img.height() - REFERENCE_OFFSET_Y


def polar(cx, cy, rx, ry):
    dx = cx - rx
    dy = ry - cy
    dist = math.sqrt(dx * dx + dy * dy)
    ang = math.degrees(math.atan2(dx, dy))
    return dist, ang


def biggest(blobs):
    return max(blobs, key=lambda b: b.area) if blobs else None


def detect_ball(img, rx, ry):
    blobs = img.find_blobs(BALL_THRESHOLDS, pixels_threshold=MIN_PIXELS,
                           area_threshold=MIN_FAR_AREA, merge=True)
    for b in sorted(blobs, key=lambda b: b.area, reverse=True):
        if b.roundness <= BALL_MIN_CIRCULARITY:
            continue
        img.draw_rectangle(b.rect, color=(255, 255, 255))
        img.draw_cross((b.cx, b.cy), color=(255, 255, 255))
        img.draw_line((rx, ry, b.cx, b.cy), color=(255, 0, 0))
        dist, ang = polar(b.cx, b.cy, rx, ry)
        return dist, ang, b.area
    return 0, 0, 0


def detect_goal(img, thresholds, color, rx, ry):
    b = biggest(img.find_blobs(thresholds,
                               pixels_threshold=GOAL_PIXELS_THRESHOLD,
                               area_threshold=GOAL_AREA_THRESHOLD,
                               merge=True))
    if not b:
        return 0, 0
    img.draw_rectangle(b.rect, color=color)
    img.draw_cross((b.cx, b.cy), color=color)
    return polar(b.cx, b.cy, rx, ry)


def main():
    cam = initialize_open()
    clock = time.clock()
    while True:
        led.toggle()
        clock.tick()
        img = cam.snapshot()
        rx, ry = reference(img)

        d_ball, a_ball, area_ball = detect_ball(img, rx, ry)
        d_y, a_y = detect_goal(img, YELLOW_THRESHOLDS, (0, 255, 0), rx, ry)
        d_b, a_b = detect_goal(img, BLUE_THRESHOLDS, (0, 0, 255), rx, ry)

        img.draw_cross((rx, ry), color=(255, 255, 255), size=10, thickness=2)

        data = "{:.1f} {:.1f} {} {:.1f} {:.1f} {:.1f} {:.1f}\n".format(
            d_ball, a_ball, area_ball, d_y, a_y, d_b, a_b)
        uart.write(data)
        print(clock.fps(), "fps |", data)
        pyb.delay(20)


main()
