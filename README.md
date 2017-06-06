# IoT-Computer-Vision-JetsonTX1-OpenCV-Google-Cloud
Embedded computer vision application in OpenCV and C++ with uploading/downloading to Google Cloud storage using Python API, modify and display downloaded images with ImageMagick Wand for Python.

Clone repository to Jetson TX1 board and one other Ubuntu 14.04 machine.

On the Jetson board, open the file "acquireModifySaveImage.cpp" and edit file path settings to match Jye etson home path.

Install IDS uEye library.

Run make to build application.

Plug in IDS uEye 1460 USB camera and test with idscameramanager.

Open terminal and type:  ./acquireModifySaveImage

You should see 2 image windows appear.

Login in to Google Cloud Storage account.

Install Google Cloud SDK.
    https://cloud.google.com/storage/docs/quickstart-gsutil

Authenticate with login credentials.
    https://googlecloudplatform.github.io/google-cloud-python/stable/google-cloud-auth.html

Open a second terminal window.

Launch upload application by typing:  ./uploadRepeatedlyIfBucketIsEmptyGCS.py

A jpeg file should appear in the Google Cloud default bucket.

On a second Ubuntu machine, repeat the Google Cloud SDK installation and authentication.

Install Wand and ImageMagick
    http://docs.wand-py.org/en/0.4.4/
    
Launch download application by typing:  ./downloadRepeatedlyIfBucketIsNotEmptyGCS.py

An ImageMagick window should pop up and a second jpeg file shoild appear in the Google Cloud default bucket.
