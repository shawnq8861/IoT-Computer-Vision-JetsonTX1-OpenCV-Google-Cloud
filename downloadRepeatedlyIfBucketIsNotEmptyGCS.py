#! /usr/bin/env python
#
# connect to Google Cloud Storage
# create a storage client
# locate the default storage bucket
# check for image file to download
# download file, delete after downloading
#

from google.cloud import storage
from google.cloud.storage import Blob
import time
from wand.image import Image
from wand.display import display

def main():
	#
	# initial greeting...
	#
	print("Hello Google Cloud Storage!")
	#
	# create a client
	#
	print("creating client...")
	client = storage.Client()
	index = 0
	print("indexing over bucket list...")
	for bucket in client.list_buckets():
		print(bucket)
		print("index = " + str(index))
		if index == 0:
			defaultBucket = bucket
		index += 1
	print("")
	print("chosen bucket is: " + str(defaultBucket))
	blob = Blob("raw_image.jpg", defaultBucket)
	quit = False
	imageFilePath = "/home/shawn/Desktop/raw_image_download.jpg"
	while quit == False:
		blobCount = 0
		for blobItem in defaultBucket.list_blobs():
			blobCount += 1
		if blobCount == 0:
			print("empty...")
		else:
			print("downloading...")
			with open(imageFilePath, "wb") as imageFile:
				blob.download_to_file(imageFile)
			with Image(filename=imageFilePath) as img:
				print(img.size)
				print("blurring...")
				img.gaussian_blur(9, 1)
				imageFilePath = "/home/shawn/Desktop/blurred_image.jpg"
				print("saving...")
				img.save(filename=imageFilePath)
			with Image(filename=imageFilePath) as img:
				blob = Blob("blurred_image.jpg", defaultBucket)
				print("uploading...")
				with open("/home/shawn/Desktop/blurred_image.jpg", "rb") as imageFile:
					blob.upload_from_file(imageFile)
				display(img)
		time.sleep(1.0)
	#
	# final greeting...
	#
	print("Goodbye Google Cloud Storage!")
	
main()
