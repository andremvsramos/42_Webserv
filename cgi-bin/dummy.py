#!/usr/bin/env python3
import sys
import os


def main():
    #with open("Data/output.txt", "w+") as f:
        print("<html><body><h1>\n")
        print(os.environ.get("AUTH_MODE", "") + "\n")
        print(os.environ.get("CONTENT_LENGTH", "") + "\n")
        print(os.environ.get("CONTENT_TYPE", "") + "\n")
        print(os.environ.get("GATEWAY_INTERFACE", "") + "\n")
        print(os.environ.get("PATH_INFO", "") + "\n")
        print(os.environ.get("PATH_TRANSLATED", "") + "\n")
        print(os.environ.get("QUERY_STRING", "") + "\n")
        print(os.environ.get("REMOTE_ADDR", "") + "\n")
        print(os.environ.get("REMOTE_HOST", "") + "\n")
        print(os.environ.get("REMOTE_IDENT", "") + "\n")
        print(os.environ.get("REMOTE_USER", "") + "\n")
        print(os.environ.get("_METHOD", "") + "\n")
        print(os.environ.get("SCRIPT_NAME", "") + "\n")
        print(os.environ.get("SERVER_NAME", "") + "\n")
        print(os.environ.get("SERVER_PORT", "") + "\n")
        print(os.environ.get("SERVER_PROTOCOL", "") + "\n")
        print(os.environ.get("SERVER_SOFTWARE", "") + "\n")
        print("</h1></body></html>\n")

if __name__ == "__main__":
    main()
