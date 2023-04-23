#!/usr/bin/env python3

import os
import subprocess
import urllib.request
import shutil

# Define the URLs for the header files
header_urls = {
    "raymath.h": "https://raw.githubusercontent.com/raysan5/raylib/master/src/raymath.h",
    "raygui.h": "https://raw.githubusercontent.com/raysan5/raygui/master/src/raygui.h",
    "raylib.h": "https://raw.githubusercontent.com/raysan5/raylib/master/src/raylib.h"
}

# Define the target directory for the headers
header_dir = "include"

# Create the directory if it doesn't exist
os.makedirs(header_dir, exist_ok=True)

# Download the header files
for header, url in header_urls.items():
    print(f"Downloading {header}...")
    target_file = os.path.join(header_dir, header)
    urllib.request.urlretrieve(url, target_file)

# Set the environment variable
os.environ['MACOSX_DEPLOYMENT_TARGET'] = '10.9'

# Install Xcode tools
print("Installing Xcode tools...")
subprocess.run(["xcode-select", "--install"])

# Clone the raylib repository
print("Cloning raylib repository...")
subprocess.run(["git", "clone", "https://github.com/raysan5/raylib.git"])

# Build raylib
print("Building raylib...")
os.chdir("raylib/src")
subprocess.run(["make"])

# Copy the library to your project's root folder
print("Copying libraylib.a to the project's root folder...")
subprocess.run(["cp", "libraylib.a", "./libs/"])

# Delete the raylib folder
print("Deleting the raylib folder...")
os.chdir("../..")
shutil.rmtree("raylib")

print("Completed!")
