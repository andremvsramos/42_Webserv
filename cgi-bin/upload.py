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
    try:
        with open(file_path, "wb") as file:
            # Read binary data from stdin
            data = sys.stdin.buffer.read()  
            file.write(data)
        # Notify of success
        print("HTTP/1.1 200 OK")
        print("Content-Type: text/html; charset=UTF-8")
        print("Transfer-Encoding: chunked")
        html_body = "<html><body><h1>Image received and written successfully.</h1></body></html>"
        html_body_bytes = html_body.encode("utf-8")
        chunks = [html_body_bytes[i:i + 1024] for i in range(0, len(html_body_bytes), 1024)]
        for chunk in chunks:
            print(f"{len(chunk):x}")  # print the chunk size in hexadecimal
            print(chunk.decode("utf-8"))  # print the chunk data
            print()  # print a blank line to separate chunks
        print("0")  # print the final chunk size (0) to indicate the end of the response
    except Exception as e:
        # Notify of failure
        print("HTTP/1.1 500 Internal Server Error")
        print("Content-Type: text/html; charset=UTF-8")
        print("Transfer-Encoding: chunked")
        html_body = f"<html><body><h1>Error writing file: {str(e)}</h1></body></html>"
        html_body_bytes = html_body.encode("utf-8")
        chunks = [html_body_bytes[i:i + 1024] for i in range(0, len(html_body_bytes), 1024)]
        for chunk in chunks:
            print(f"{len(chunk):x}")  # print the chunk size in hexadecimal
            print(chunk.decode("utf-8"))  # print the chunk data
            print()  # print a blank line to separate chunks
        print("0")  # print the final chunk size (0) to indicate the end of the response

if __name__ == "__main__":
    main()