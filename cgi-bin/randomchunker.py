import requests
import random

url = 'http://localhost:8002/cgi-bin/chunker.py'

def random_string(length):
    letters = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
    return ''.join(random.choice(letters) for i in range(length))

def chunked_generator():
    chunk_size = 1024 * 1024
    num_chunks = 10
    for i in range(num_chunks):
        data = random_string(chunk_size)
        yield data.encode()

try:
    response = requests.post(url, data=chunked_generator(), headers={'Transfer-Encoding': 'chunked'})
    print(response.status_code)
except requests.exceptions.RequestException as e:
    print(f"Error: {str(e)}")