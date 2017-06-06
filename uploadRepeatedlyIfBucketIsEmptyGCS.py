#! /usr/bin/env python
#
# connect to Google Cloud Storage
# create a storage client
# locate the default storage bucket
# check if image file already uploaded
# upload if file not found
#

from google.cloud import storage
from google.cloud.storage import Blob
import time

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
	while quit == False:
		blobCount = 0
		for blobItem in defaultBucket.list_blobs():
			blobCount += 1
		if blobCount == 0:
			print("uploading...")
			with open("/home/ubuntu/Desktop/raw_image.jpg", "rb") as imageFile:
				blob.upload_from_file(imageFile)
		else:
			print("not empty...")
		time.sleep(1.0)
	#
	# final greeting...
	#
	print("Goodbye Google Cloud Storage!")
	
main()
