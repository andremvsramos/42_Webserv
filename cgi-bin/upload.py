#!/usr/bin/env python3
import sys
import os

def main():

    #create firectory if doesnt exist
    directory = "Data"
    if not os.path.exists(directory):
        os.makedirs(directory)

    filename = os.environ.get("FILENAME", "")

    file_path = os.path.join(directory, filename)

    # Open file in binary write mode
    with open(file_path, "wb") as file:
        # Read binary data from stdin
        data = sys.stdin.buffer.read()
        file.write(data)

    # Notify of success
    print("<html><body><h1>Image received and written successfully.</h1></body></html>")

if __name__ == "__main__":
    main()
