import pyautogui
from time import sleep

# next track, nexttrack
# last track, prevtrack
# play/pause, playpause
# stop, stop


def keypress(key):
    pyautogui.keyDown(key)
    pyautogui.keyUp(key)
